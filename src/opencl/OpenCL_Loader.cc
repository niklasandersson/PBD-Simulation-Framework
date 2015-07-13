#include "opencl/OpenCL_Loader.h"


OpenCL_Loader::~OpenCL_Loader() {

  for(auto& p : kernels_) {
    if( p.second ) {
      delete p.second;
    }
  }

}


Kernel* OpenCL_Loader::access_kernel(const std::string& name) {

  auto it = kernels_.find(name);

  if( it == std::end(kernels_) ) {
    throw std::invalid_argument{ report_error( "The Kernel '" << name << "' which you are trying to access is not loaded." ) };
  }

  return kernels_[name];

}


void OpenCL_Loader::load_kernels(cl_context context, cl_device_id device_id, const std::string& file, std::ostream& os) {

  std::string kernels;

  try {
    kernels = kernels_parser_.parseFile(file); 
  } catch(const std::invalid_argument& e) {
    print_error(e);
    std::exit(EXIT_FAILURE);
  }

  std::string path = file.substr(0, file.rfind('/')+1);   

  std::istringstream is{kernels};

  std::string name;

  while( is >> std::ws >> name ) {
    load_kernel(context, device_id, name, path, os);
  }

}


void OpenCL_Loader::load_kernel(cl_context context, cl_device_id device_id, const std::string& name, const std::string& path, std::ostream& os) {

  auto it = kernels_.find(name);
  if( it != std::end(kernels_) ) {
    throw std::invalid_argument{ report_error( "The Kernel '" << name << "' is already loaded." ) };
  } 

  cl_int error_code = CL_SUCCESS;
  cl_program program;

  std::string source;

  try {
    source = code_parser_.parseFile(path + name); 
  } catch(const std::invalid_argument& e) {
    print_error(e);
    std::exit(EXIT_FAILURE);
  }

  const size_t source_size = source.size();

  try {
    Parser fileParser;

    std::ostringstream oldSourceFileName{};
    oldSourceFileName << "bin/opencl/source/" << name << ".clsource";
    std::string oldSource = fileParser.parseFile(oldSourceFileName.str());

    if( (source_size != oldSource.size()) || (source != oldSource) ) {
      throw std::domain_error{ report_error( "OpenCL kernel source '" << name << "' differs from old source" ) }; 
    }

    std::ostringstream binaryFileName{};
    binaryFileName << "bin/opencl/kernel/" << name << ".clbinary";
    std::string binary = fileParser.parseFile(binaryFileName.str());

    std::istringstream binaryIs{binary};
    std::string line;
    getline(binaryIs, line);

    std::istringstream lineIs{line};
    size_t binary_size;;
    lineIs >> binary_size;

    std::istreambuf_iterator<char> eos;
    std::string code(std::istreambuf_iterator<char>(binaryIs), eos);

    size_t string_size = std::min(code.size(), binary_size);

    unsigned char storage[binary_size];
    memcpy(storage, (const unsigned char*)code.c_str(), string_size);

    const unsigned char* bin = storage;
    
    #define UINT_NUM_DEVICES 0
    #define DEVICE_LIST NULL
    #define OPTIONS NULL
    #define NOTIFICATION_CALL_BACK_FUNCTION NULL
    #define ARGUMENT_TO_CALL_BACK_FUNCTION NULL

    program = clCreateProgramWithBinary(context, 1, &device_id, &binary_size, &bin, NULL, &error_code);

    if( error_code != CL_SUCCESS ) {
      throw std::invalid_argument{ report_error( "Failed creating program for OpenCL Kernel '" << name << "', "
                                        << OpenCL_Error_Message(error_code) << "." ) };
    }

    error_code = clBuildProgram(program, 1,  &device_id, OPTIONS, NOTIFICATION_CALL_BACK_FUNCTION, ARGUMENT_TO_CALL_BACK_FUNCTION);

    if( error_code != CL_SUCCESS ) {

      size_t build_log_length;
      clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &build_log_length);

      char build_log[build_log_length];
      #define PARAM_VALUE_SIZE NULL
      clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(build_log), build_log, PARAM_VALUE_SIZE);

      throw std::invalid_argument{ report_error( "Failed building program for OpenCL Kernel '" << name << "', " 
                                        << OpenCL_Error_Message(error_code) << ", " 
                                        << build_log << std::endl) };
    } 

  } catch( const std::exception& e ) {

    // print_error(e); // No need to see this print out, as the error is sorted out below

    Log source_log;
    source_log.disablePrintToCout();

    source_log << source;

    std::ostringstream sourceLogFileName{};
    sourceLogFileName << "bin/opencl/source/" << name << ".clsource";
    source_log.write(sourceLogFileName.str());

    program = clCreateProgramWithSource(context, 1, (const char **)&source, &source_size, &error_code);
    if( error_code != CL_SUCCESS ) {
      throw std::invalid_argument{ report_error( "Failed creating program for OpenCL Kernel '" << name << "', "
                                        << OpenCL_Error_Message(error_code) << "." ) };
    }

    error_code = clBuildProgram(program, UINT_NUM_DEVICES, DEVICE_LIST, OPTIONS, NOTIFICATION_CALL_BACK_FUNCTION, ARGUMENT_TO_CALL_BACK_FUNCTION);

    if( error_code != CL_SUCCESS ) {

      size_t build_log_length;
      clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &build_log_length);

      char build_log[build_log_length];
      #define PARAM_VALUE_SIZE NULL
      clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(build_log), build_log, PARAM_VALUE_SIZE);

      throw std::invalid_argument{ report_error( "Failed building program for OpenCL Kernel '" << name << "', " 
                                        << OpenCL_Error_Message(error_code) << ", " 
                                        << build_log << std::endl) };
    } 

    size_t binary_size;
    clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES, sizeof(size_t), &binary_size, NULL);
    unsigned char storage[binary_size+1];
    unsigned char* binary = storage;
    clGetProgramInfo(program, CL_PROGRAM_BINARIES, binary_size, &binary, NULL);

    Log binary_log;
    binary_log.disablePrintToCout();
    
    binary_log << binary_size << std::endl;
    binary[binary_size] = '\0';
    binary_log << binary;

    std::ostringstream fileName{};
    fileName << "bin/opencl/kernel/" << name << ".clbinary";
    binary_log.write(fileName.str());

  }

  cl_kernel kernel = clCreateKernel(program, "main", &error_code);

  if( error_code != CL_SUCCESS ) {
    throw std::invalid_argument{ report_error( "Failed creating OpenCL Kernel '" << name << "', "
                                      << OpenCL_Error_Message(error_code) << "." ) };
  }
  
  os << "Kernel: " << '\'' << name << '\'' << " compile status: " << "\'OK\'" << std::endl;

  kernels_[name] = new Kernel(kernel, program);

}


void OpenCL_Loader::print_available_kernels(std::ostream& os) {
  
  for(const auto& p : kernels_) {
    os << "\t" << p.first << std::endl;
  }

}
