#include "opencl/OpenCL_Utilities.h"


const char* OpenCL_Error_Message(const cl_int& error_code)
{

  switch ( error_code )
  {
    case CL_SUCCESS                                     :   return "CL_SUCCESS"                                     ;
    case CL_DEVICE_NOT_FOUND                            :   return "CL_DEVICE_NOT_FOUND"                            ;
    case CL_DEVICE_NOT_AVAILABLE                        :   return "CL_DEVICE_NOT_AVAILABLE"                        ;

#if !(defined(CL_PLATFORM_NVIDIA) && CL_PLATFORM_NVIDIA == 0x3001)
    case CL_COMPILER_NOT_AVAILABLE                      :   return "CL_COMPILER_NOT_AVAILABLE"                      ;
#endif

    case CL_MEM_OBJECT_ALLOCATION_FAILURE               :   return "CL_MEM_OBJECT_ALLOCATION_FAILURE"               ;
    case CL_OUT_OF_RESOURCES                            :   return "CL_OUT_OF_RESOURCES"                            ;
    case CL_OUT_OF_HOST_MEMORY                          :   return "CL_OUT_OF_HOST_MEMORY"                          ;
    case CL_PROFILING_INFO_NOT_AVAILABLE                :   return "CL_PROFILING_INFO_NOT_AVAILABLE"                ;
    case CL_MEM_COPY_OVERLAP                            :   return "CL_MEM_COPY_OVERLAP"                            ;
    case CL_IMAGE_FORMAT_MISMATCH                       :   return "CL_IMAGE_FORMAT_MISMATCH"                       ;
    case CL_IMAGE_FORMAT_NOT_SUPPORTED                  :   return "CL_IMAGE_FORMAT_NOT_SUPPORTED"                  ;
    case CL_BUILD_PROGRAM_FAILURE                       :   return "CL_BUILD_PROGRAM_FAILURE"                       ;
    case CL_MAP_FAILURE                                 :   return "CL_MAP_FAILURE"                                 ;
    case CL_INVALID_VALUE                               :   return "CL_INVALID_VALUE"                               ;
    case CL_INVALID_DEVICE_TYPE                         :   return "CL_INVALID_DEVICE_TYPE"                         ;
    case CL_INVALID_PLATFORM                            :   return "CL_INVALID_PLATFORM"                            ;
    case CL_INVALID_DEVICE                              :   return "CL_INVALID_DEVICE"                              ;
    case CL_INVALID_CONTEXT                             :   return "CL_INVALID_CONTEXT"                             ;
    case CL_INVALID_QUEUE_PROPERTIES                    :   return "CL_INVALID_QUEUE_PROPERTIES"                    ;
    case CL_INVALID_COMMAND_QUEUE                       :   return "CL_INVALID_COMMAND_QUEUE"                       ;
    case CL_INVALID_HOST_PTR                            :   return "CL_INVALID_HOST_PTR"                            ;
    case CL_INVALID_MEM_OBJECT                          :   return "CL_INVALID_MEM_OBJECT"                          ;
    case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR             :   return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR"             ;
    case CL_INVALID_IMAGE_SIZE                          :   return "CL_INVALID_IMAGE_SIZE"                          ;
    case CL_INVALID_SAMPLER                             :   return "CL_INVALID_SAMPLER"                             ;
    case CL_INVALID_BINARY                              :   return "CL_INVALID_BINARY"                              ;
    case CL_INVALID_BUILD_OPTIONS                       :   return "CL_INVALID_BUILD_OPTIONS"                       ;
    case CL_INVALID_PROGRAM                             :   return "CL_INVALID_PROGRAM"                             ;
    case CL_INVALID_PROGRAM_EXECUTABLE                  :   return "CL_INVALID_PROGRAM_EXECUTABLE"                  ;
    case CL_INVALID_KERNEL_NAME                         :   return "CL_INVALID_KERNEL_NAME"                         ;
    case CL_INVALID_KERNEL_DEFINITION                   :   return "CL_INVALID_KERNEL_DEFINITION"                   ;
    case CL_INVALID_KERNEL                              :   return "CL_INVALID_KERNEL"                              ;
    case CL_INVALID_ARG_INDEX                           :   return "CL_INVALID_ARG_INDEX"                           ;
    case CL_INVALID_ARG_VALUE                           :   return "CL_INVALID_ARG_VALUE"                           ;
    case CL_INVALID_ARG_SIZE                            :   return "CL_INVALID_ARG_SIZE"                            ;
    case CL_INVALID_KERNEL_ARGS                         :   return "CL_INVALID_KERNEL_ARGS"                         ;
    case CL_INVALID_WORK_DIMENSION                      :   return "CL_INVALID_WORK_DIMENSION"                      ;                   
    case CL_INVALID_WORK_GROUP_SIZE                     :   return "CL_INVALID_WORK_GROUP_SIZE"                     ;
    case CL_INVALID_WORK_ITEM_SIZE                      :   return "CL_INVALID_WORK_ITEM_SIZE"                      ;
    case CL_INVALID_GLOBAL_OFFSET                       :   return "CL_INVALID_GLOBAL_OFFSET"                       ;
    case CL_INVALID_EVENT_WAIT_LIST                     :   return "CL_INVALID_EVENT_WAIT_LIST"                     ;
    case CL_INVALID_EVENT                               :   return "CL_INVALID_EVENT"                               ;
    case CL_INVALID_OPERATION                           :   return "CL_INVALID_OPERATION"                           ;
    case CL_INVALID_GL_OBJECT                           :   return "CL_INVALID_GL_OBJECT"                           ;
    case CL_INVALID_BUFFER_SIZE                         :   return "CL_INVALID_BUFFER_SIZE"                         ;
    case CL_INVALID_MIP_LEVEL                           :   return "CL_INVALID_MIP_LEVEL"                           ;

#if defined(cl_khr_gl_sharing) && (cl_khr_gl_sharing >= 1)
    case CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR         :   return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR"         ;
#endif

#ifdef CL_VERSION_1_1
    case CL_MISALIGNED_SUB_BUFFER_OFFSET                :   return "CL_MISALIGNED_SUB_BUFFER_OFFSET"                ;
    case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST   :   return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST"   ;
    case CL_INVALID_GLOBAL_WORK_SIZE                    :   return "CL_INVALID_GLOBAL_WORK_SIZE"                    ;
#endif

  }

  return "INVALID_OR_UNKNOWN_ERROR";
}



void OpenCL_Print_String_Properties(const std::map<cl_device_info, String_Property>& string_properties, std::ostream& os) {
    for( auto& pair : string_properties ) {
        OpenCL_Print_String_Property(pair.second, os);
    }
}

void OpenCL_Print_String_Property(const String_Property& string_property, std::ostream& os) {
    os << string_property.name_ << ": " << string_property.value_ << std::endl;
}



void OpenCL_Print_Integer_Properties(const std::map<cl_device_info, Integer_Property>& integer_properties, std::ostream& os) {
    for( auto& pair : integer_properties ) {
        OpenCL_Print_Integer_Property(pair.second, os);
    }
}

void OpenCL_Print_Integer_Property(const Integer_Property& integer_property, std::ostream& os) {
    os << integer_property.name_ << ": " << integer_property.value_ << std::endl;
}



void OpenCL_Print_Hex_Properties(const std::map<cl_device_info, Hex_Property>& hex_properties, std::ostream& os) {
    for( auto& pair : hex_properties ) {
        OpenCL_Print_Hex_Property(pair.second, os);
    }
}

void OpenCL_Print_Hex_Property(const Hex_Property& hex_property, std::ostream& os) {
    os << hex_property.name_ << ": 0x" << std::hex << hex_property.value_ << std::dec << std::endl;;
}



std::map<cl_device_info, String_Property> OpenCL_Get_String_Properties(const cl_device_id& device_id) {

    std::map<cl_device_info, String_Property> string_properties = 
    {
        { CL_DEVICE_NAME       , String_Property{ "NAME"           , "" } } ,
        { CL_DEVICE_VENDOR     , String_Property{ "VENDOR"         , "" } } ,
        { CL_DRIVER_VERSION    , String_Property{ "DRIVER_VERSION" , "" } } ,
        { CL_DEVICE_PROFILE    , String_Property{ "PROFILE"        , "" } } ,
        { CL_DEVICE_VERSION    , String_Property{ "VERSION"        , "" } } ,
        { CL_DEVICE_EXTENSIONS , String_Property{ "EXTENSIONS"     , "" } } 
    }; 
   
    // Load String Properties
    cl_int error_code = CL_SUCCESS;
    size_t size = 0;
    char temp_buffer[65536];
    for( auto& string_property : string_properties ) {
        error_code = clGetDeviceInfo(device_id, string_property.first, sizeof(temp_buffer), temp_buffer, &size);

        if( error_code != CL_SUCCESS ) {
          string_property.second.value_ = "UNABLE TO GET VALUE";
          continue;
        }

        if( size > sizeof(temp_buffer) ) {
          string_property.second.value_ = std::string{"TRUNCATED VALUE: "} + std::string{std::move(temp_buffer)};
          // std::cout << "Large '" << string_property.second.name_ << "' (" << size << " bytes), truncating to '" << sizeof(temp_buffer) << "'" << std::endl;
        }

        string_property.second.value_ = std::move(temp_buffer);
    }

    return std::move(string_properties);
}



std::map<cl_device_info, Integer_Property> OpenCL_Get_Integer_Properties(const cl_device_id& device_id) {

    std::map<cl_device_info, Integer_Property > integer_properties = 
    { 
        { CL_DEVICE_TYPE                          , Integer_Property{ "TYPE"                          , 0 } } ,
        { CL_DEVICE_EXECUTION_CAPABILITIES        , Integer_Property{ "EXECUTION_CAPABILITIES"        , 0 } } ,
        { CL_DEVICE_GLOBAL_MEM_CACHE_TYPE         , Integer_Property{ "GLOBAL_MEM_CACHE_TYPE"         , 0 } } ,
        { CL_DEVICE_LOCAL_MEM_TYPE                , Integer_Property{ "LOCAL_MEM_TYPE"                , 0 } } ,
        { CL_DEVICE_VENDOR_ID                     , Integer_Property{ "VENDOR_ID"                     , 0 } } ,
        { CL_DEVICE_MAX_COMPUTE_UNITS             , Integer_Property{ "MAX_COMPUTE_UNITS"             , 0 } } ,
        { CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS      , Integer_Property{ "MAX_WORK_ITEM_DIMENSIONS"      , 0 } } ,
        { CL_DEVICE_MAX_WORK_GROUP_SIZE           , Integer_Property{ "MAX_WORK_GROUP_SIZE"           , 0 } } ,
        { CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR   , Integer_Property{ "PREFERRED_VECTOR_WIDTH_CHAR"   , 0 } } ,
        { CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT  , Integer_Property{ "PREFERRED_VECTOR_WIDTH_SHORT"  , 0 } } ,
        { CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT    , Integer_Property{ "PREFERRED_VECTOR_WIDTH_INT"    , 0 } } ,
        { CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG   , Integer_Property{ "PREFERRED_VECTOR_WIDTH_LONG"   , 0 } } ,
        { CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT  , Integer_Property{ "PREFERRED_VECTOR_WIDTH_FLOAT"  , 0 } } ,
        { CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE , Integer_Property{ "PREFERRED_VECTOR_WIDTH_DOUBLE" , 0 } } ,
        { CL_DEVICE_MAX_CLOCK_FREQUENCY           , Integer_Property{ "MAX_CLOCK_FREQUENCY"           , 0 } } ,
        { CL_DEVICE_ADDRESS_BITS                  , Integer_Property{ "ADDRESS_BITS"                  , 0 } } ,
        { CL_DEVICE_MAX_MEM_ALLOC_SIZE            , Integer_Property{ "MAX_MEM_ALLOC_SIZE"            , 0 } } ,
        { CL_DEVICE_IMAGE_SUPPORT                 , Integer_Property{ "IMAGE_SUPPORT"                 , 0 } } ,
        { CL_DEVICE_MAX_READ_IMAGE_ARGS           , Integer_Property{ "MAX_READ_IMAGE_ARGS"           , 0 } } ,
        { CL_DEVICE_MAX_WRITE_IMAGE_ARGS          , Integer_Property{ "MAX_WRITE_IMAGE_ARGS"          , 0 } } ,
        { CL_DEVICE_IMAGE2D_MAX_WIDTH             , Integer_Property{ "IMAGE2D_MAX_WIDTH"             , 0 } } ,
        { CL_DEVICE_IMAGE2D_MAX_HEIGHT            , Integer_Property{ "IMAGE2D_MAX_HEIGHT"            , 0 } } ,
        { CL_DEVICE_IMAGE3D_MAX_WIDTH             , Integer_Property{ "IMAGE3D_MAX_WIDTH"             , 0 } } ,
        { CL_DEVICE_IMAGE3D_MAX_HEIGHT            , Integer_Property{ "IMAGE3D_MAX_HEIGHT"            , 0 } } ,
        { CL_DEVICE_IMAGE3D_MAX_DEPTH             , Integer_Property{ "IMAGE3D_MAX_DEPTH"             , 0 } } ,
        { CL_DEVICE_MAX_SAMPLERS                  , Integer_Property{ "MAX_SAMPLERS"                  , 0 } } ,
        { CL_DEVICE_MAX_PARAMETER_SIZE            , Integer_Property{ "MAX_PARAMETER_SIZE"            , 0 } } ,
        { CL_DEVICE_MEM_BASE_ADDR_ALIGN           , Integer_Property{ "MEM_BASE_ADDR_ALIGN"           , 0 } } ,
        { CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE      , Integer_Property{ "MIN_DATA_TYPE_ALIGN_SIZE"      , 0 } } ,
        { CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE     , Integer_Property{ "GLOBAL_MEM_CACHELINE_SIZE"     , 0 } } ,
        { CL_DEVICE_GLOBAL_MEM_CACHE_SIZE         , Integer_Property{ "GLOBAL_MEM_CACHE_SIZE"         , 0 } } ,
        { CL_DEVICE_GLOBAL_MEM_SIZE               , Integer_Property{ "GLOBAL_MEM_SIZE"               , 0 } } ,
        { CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE      , Integer_Property{ "MAX_CONSTANT_BUFFER_SIZE"      , 0 } } ,
        { CL_DEVICE_MAX_CONSTANT_ARGS             , Integer_Property{ "MAX_CONSTANT_ARGS"             , 0 } } ,
        { CL_DEVICE_LOCAL_MEM_SIZE                , Integer_Property{ "LOCAL_MEM_SIZE"                , 0 } } ,
        { CL_DEVICE_ERROR_CORRECTION_SUPPORT      , Integer_Property{ "ERROR_CORRECTION_SUPPORT"      , 0 } } ,
        { CL_DEVICE_PROFILING_TIMER_RESOLUTION    , Integer_Property{ "PROFILING_TIMER_RESOLUTION"    , 0 } } ,
        { CL_DEVICE_ENDIAN_LITTLE                 , Integer_Property{ "ENDIAN_LITTLE "                , 0 } } ,
        { CL_DEVICE_AVAILABLE                     , Integer_Property{ "AVAILABLE"                     , 0 } } ,
        { CL_DEVICE_COMPILER_AVAILABLE            , Integer_Property{ "COMPILER_AVAILABLE"            , 0 } } 
    };

    // Load Integer Properties
    cl_int error_code = CL_SUCCESS;
    long long value = 0;
    for( auto& integer_property : integer_properties ) {
        error_code = clGetDeviceInfo(device_id, integer_property.first, sizeof(value), &value, nullptr);

        if( error_code != CL_SUCCESS ) {
            integer_property.second.value_ = -1;
            continue;
        }

        integer_property.second.value_ = std::move(value);
    }

    return std::move(integer_properties);
}



std::map<cl_device_info, Hex_Property> OpenCL_Get_Hex_Properties(const cl_device_id& device_id) {
    
    std::map<cl_device_info, Hex_Property> hex_properties = 
    {   
        { CL_DEVICE_SINGLE_FP_CONFIG , Hex_Property{ "SINGLE_FP_CONFIG", 0 } } ,
        { CL_DEVICE_QUEUE_PROPERTIES , Hex_Property{ "QUEUE_PROPERTIES", 0 } } 
    };

    // Load Hex Properties
    cl_int error_code = CL_SUCCESS;
    long long value = 0;
    for( auto& hex_property : hex_properties ) {
        error_code = clGetDeviceInfo(device_id, hex_property.first, sizeof(value), &value, nullptr);

        if( error_code != CL_SUCCESS ) {
          hex_property.second.value_ = -1;
        }

        hex_property.second.value_ = std::move(value);
    }

    return std::move(hex_properties);
}


std::string OpenCL_Get_Device_Type(const cl_device_id& device_id) {
  cl_int error_code = CL_SUCCESS;
  cl_device_type value = 0;
  error_code = clGetDeviceInfo(device_id, CL_DEVICE_TYPE, sizeof(value), &value, nullptr);
  std::ostringstream os{};

  if( error_code == CL_SUCCESS ) {
  
    if( value & CL_DEVICE_TYPE_DEFAULT ) {
      value &= ~CL_DEVICE_TYPE_DEFAULT;
      os << "Default ";
    }
    
    if( value & CL_DEVICE_TYPE_CPU ) {
      value &= ~CL_DEVICE_TYPE_CPU;
      os << "CPU ";
    }

    if( value & CL_DEVICE_TYPE_GPU ) {
      value &= ~CL_DEVICE_TYPE_GPU;
      os << "GPU ";
    }
    
    if( value & CL_DEVICE_TYPE_ACCELERATOR ) {
      value &= ~CL_DEVICE_TYPE_ACCELERATOR;
      os << "Accelerator ";
    }

    if( value != 0 ) {
      os << "Unknown 0x" << std::hex << value << std::dec;
    }

  } else {
      return "Unable to get DEVICE_TYPE";
  }

  return os.str().c_str();

}



std::string OpenCL_Get_Device_Execution_Capabilities(const cl_device_id& device_id) {
  cl_int error_code = CL_SUCCESS;
  long long value = 0;
  error_code = clGetDeviceInfo(device_id, CL_DEVICE_EXECUTION_CAPABILITIES, sizeof(value), &value, nullptr);
  std::ostringstream os{};

  if( error_code == CL_SUCCESS ) {

    if( value & CL_EXEC_KERNEL ) {
      value &= ~CL_EXEC_KERNEL;
      os << "Kernel ";
    }
    
    if( value & CL_EXEC_NATIVE_KERNEL ) {
      value &= ~CL_EXEC_NATIVE_KERNEL;
      os << "Native ";
    }

    if( value ) {
     os << "Unknown 0x" << std::hex << value << std::dec;
    }

  } else {
    return "Unable to get EXECUTION_CAPABILITIES";
  }

  return os.str();

}



std::string OpenCL_Get_Device_Global_Mem_Cache_Type(const cl_device_id& device_id) {
  cl_int error_code = CL_SUCCESS;
  long long value = 0;
  error_code = clGetDeviceInfo(device_id, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, sizeof(value), &value, nullptr);
  std::ostringstream os{};

  if( error_code == CL_SUCCESS ) {
    static const char *cache_types[] = { "None", "Read-Only", "Read-Write" };
    static int num_types = sizeof cache_types / sizeof cache_types[0];
    os << (value < num_types ? cache_types[value] : "???");

  } else {
    return "Unable to get GLOBAL_MEM_CACHE_TYPE";
  }

  return os.str();
}



std::string OpenCL_Get_Device_Local_Mem_Type(const cl_device_id& device_id) {
  cl_int error_code = CL_SUCCESS;
  long long value = 0;
  error_code = clGetDeviceInfo(device_id, CL_DEVICE_LOCAL_MEM_TYPE, sizeof(value), &value, nullptr);
  std::ostringstream os{};

  if( error_code == CL_SUCCESS )
  {
    static const char *lmemTypes[] = { "???", "Local", "Global" };
    static int numTypes = sizeof lmemTypes / sizeof lmemTypes[0];
    os << (value < numTypes ? lmemTypes[value] : "???");

  } else {
     return "Unable to get CL_DEVICE_LOCAL_MEM_TYPE";
  }

  return os.str();
}



std::vector<size_t> OpenCL_Get_Max_Work_Group_Sizes(const cl_device_id& device_id) {

  cl_int error_code = CL_SUCCESS;
  std::vector<size_t> max_sizes;
  size_t size = 0;
  
  error_code = clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_ITEM_SIZES, 0, 0, &size);
  if( error_code != CL_SUCCESS ) {
    return std::move(max_sizes);
  }

  size_t res_vec[size/sizeof(size_t)]; 

  error_code = clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_ITEM_SIZES, size, res_vec, &size);
  if( error_code != CL_SUCCESS ) {
    return std::move(max_sizes);
  }

  for (size_t i = 0; i < size/sizeof(size_t); ++i) {
    max_sizes.push_back(res_vec[i]);
  }

  return std::move(max_sizes);
}



void OpenCL_Print_Device_Info(const cl_device_id& device_id, std::ostream& os) {

    os << "DEVICE_TYPE: " << OpenCL_Get_Device_Type(device_id) << std::endl;
    os << "EXECUTION_CAPABILITIES: " << OpenCL_Get_Device_Execution_Capabilities(device_id) << std::endl;
    os << "GLOBAL_MEM_CACHE_TYPE: " << OpenCL_Get_Device_Global_Mem_Cache_Type(device_id) << std::endl;
    os << "LOCAL_MEM_TYPE: " << OpenCL_Get_Device_Local_Mem_Type(device_id) << std::endl;

    std::vector<size_t> max_sizes{OpenCL_Get_Max_Work_Group_Sizes(device_id)};
    os << "MAX_WORK_GROUP_SIZES: ";
    for(const auto& size : max_sizes) {
      os << size << " ";
    }
    os << std::endl;

    OpenCL_Print_String_Properties(OpenCL_Get_String_Properties(device_id), os);

    OpenCL_Print_Integer_Properties(OpenCL_Get_Integer_Properties(device_id), os);

    OpenCL_Print_Hex_Properties(OpenCL_Get_Hex_Properties(device_id), os);

}



cl_platform_id OpenCL_Choose_Platform(std::istream& is, std::ostream& os) {

  os << "--- Choose Platform ---" << std::endl;

  cl_uint number_of_platforms = 0;

  {
    cl_int error_code = CL_SUCCESS;
    error_code = clGetPlatformIDs(0, nullptr, &number_of_platforms);
    if( error_code != CL_SUCCESS ) {
       throw std::domain_error{ report_error( "OpenCL; Failed to receive platform ids, '" 
                                        << OpenCL_Error_Message(error_code) << "'." ) };
    }
  }

  if( number_of_platforms == 0 ) {
    throw std::domain_error{ report_error( "OpenCL; No platforms available on this machine." ) };
  }

  cl_platform_id platform_ids[number_of_platforms];

  {
    cl_int error_code = CL_SUCCESS;
    error_code = clGetPlatformIDs(number_of_platforms, platform_ids, nullptr);
    if( error_code != CL_SUCCESS ) {
       throw std::domain_error{ report_error( "OpenCL; Failed to receive platform ids, '" 
                                        << OpenCL_Error_Message(error_code) << "'." ) };
    }
  }

  os << "Platforms available: " << std::endl;

  for(cl_uint i=0; i<number_of_platforms; ++i) {
    char vendor_name_buffer[OPENCL_MAX_VENDOR_NAME_LENGTH];
    clGetPlatformInfo(platform_ids[i], CL_PLATFORM_VENDOR, sizeof(vendor_name_buffer), vendor_name_buffer, nullptr);
    os << "\t[" << i << "] : " << vendor_name_buffer << std::endl;
  }

  os << std::endl;

  unsigned int platform = 0;
  char confirm = 'e';
  bool has_chosen_platform = false;
  
  while( !has_chosen_platform ) {

    {
      std::string line{"e"};
      bool has_read_vaild_value = false;
      while( !has_read_vaild_value ) {
        os << "Choose platform: ";
        try {
          std::getline(is, line);
          platform = read_unsigned_int(line);
          if( platform >= 0 && platform < static_cast<unsigned int>(number_of_platforms) ) {
            has_read_vaild_value = true;
          } else {
            throw std::invalid_argument{"Choice is not within range."};
          }
        } catch(const std::invalid_argument& e) {
          os << "Invalid choice: " << e.what() << std::endl;
        } catch(const std::invalid_argument& e) {
          os << "Invalid choice: Choice is not an integer." << std::endl;
        }
      }
    }
   
    {
      std::string line{"e"};
      bool has_read_vaild_value = false;
      while( !has_read_vaild_value ) {
        os << "Choose platform " << platform << "? (y/n): ";
        try {
          std::getline(is, line);
          confirm = line[0];
          if( line.size() == 1 && (confirm == 'y' || confirm == 'Y' || confirm == 'n' || confirm == 'N') ) {
            has_read_vaild_value = true;
          } else {
            throw std::invalid_argument{"Choice is not within range."};
          }
        } catch(const std::invalid_argument& e) {
           os << "Invalid choice: " << e.what() << std::endl;
        } 
      }
    }

    if( confirm == 'y' || confirm == 'Y' ) {
      has_chosen_platform = true;
    }

  }

  cl_platform_id platform_choice = platform_ids[platform];

  os << std::endl;

  return platform_choice;
}


cl_device_id OpenCL_Choose_Device(const cl_platform_id& platform_id, std::istream& is, std::ostream& os) {

  os << "--- Choose Device ---" << std::endl;

  cl_uint number_of_devices = 0;

  {
    cl_int error_code = CL_SUCCESS;
    error_code = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 0, nullptr, &number_of_devices);
    if( error_code != CL_SUCCESS ) {
       throw std::domain_error{ report_error( "OpenCL; Failed to receive device ids, '" 
                                        << OpenCL_Error_Message(error_code) << "'." ) };
    }
  }

  cl_device_id* device_ids;

  try {
    device_ids = new cl_device_id[number_of_devices];
  } catch(const std::bad_alloc& e) {
    print_error(e);
    std::exit(EXIT_FAILURE);
  }

  {
    cl_int error_code = CL_SUCCESS;
    error_code = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, number_of_devices, device_ids, nullptr);
    if( error_code != CL_SUCCESS ) {
       throw std::domain_error{ report_error( "OpenCL; Failed to receive device ids, '" 
                                        << OpenCL_Error_Message(error_code) << "'." ) };
    }
  }

  os << "Devices available: " << std::endl;

  for(cl_uint i=0; i<number_of_devices; ++i) {

    char vendor_name_buffer[OPENCL_MAX_VENDOR_NAME_LENGTH];

    {
      cl_int error_code = CL_SUCCESS;
      error_code = clGetDeviceInfo(device_ids[i], CL_DEVICE_NAME, sizeof(vendor_name_buffer), vendor_name_buffer, nullptr);
      if( error_code != CL_SUCCESS ) {
         throw std::domain_error{ report_error( "OpenCL; Failed to receive device info, '" 
                                          << OpenCL_Error_Message(error_code) << "'." ) };
      }
    }

    os << "\t[" << i << "] : " << vendor_name_buffer << std::endl;
    
    os << "\t\tDEVICE_TYPE: " << OpenCL_Get_Device_Type(device_ids[i]) << std::endl;

    std::map<cl_device_info, String_Property> string_properties{OpenCL_Get_String_Properties(device_ids[i])};
    
    os << "\t\t";
    OpenCL_Print_String_Property(string_properties[CL_DRIVER_VERSION], os);
    os << "\t\t";
    OpenCL_Print_String_Property(string_properties[CL_DEVICE_VERSION], os);

    os << std::endl;
  }

  unsigned int device = 0;
  char confirm = 'e';
  bool has_chosen_device = false;

  while( !has_chosen_device ) {

    {
      std::string line{"e"};
      bool has_read_vaild_value = false;
      while( !has_read_vaild_value ) {
        os << "Choose device: ";
        try {
          std::getline(is, line);
          device = read_unsigned_int(line);
          if( device >= 0 && device < static_cast<unsigned int>(number_of_devices) ) {
            has_read_vaild_value = true;
          } else {
            throw std::invalid_argument{"Choice is not within range."};
          }
        } catch(const std::invalid_argument& e) {
          os << "Invalid choice: " << e.what() << std::endl;
        } catch(const std::invalid_argument& e) {
          os << "Invalid choice: Choice is not an integer." << std::endl;
        }
      }
    }

    os << std::endl;
    os << "--- Device Info ---" << std::endl;
    OpenCL_Print_Device_Info(device_ids[device], os);
    os << std::endl;

    {
      std::string line{"e"};
      bool has_read_vaild_value = false;
      while( !has_read_vaild_value ) {
        os << "Choose device " << device << "? (y/n): ";
        try {
          std::getline(is, line);
          confirm = line[0];
          if( line.size() == 1 && (confirm == 'y' || confirm == 'Y' || confirm == 'n' || confirm == 'N') ) {
            has_read_vaild_value = true;
          } else {
            throw std::invalid_argument{"Choice is not within range."};
          }
        } catch(const std::invalid_argument& e) {
           os << "Invalid choice: " << e.what() << std::endl;
        } 
      }
    }

    if( confirm == 'y' || confirm == 'Y' ) {
      has_chosen_device = true;
    }

  }

  cl_device_id device_choice = device_ids[device];

  delete [] device_ids;

  os << std::endl;

  return device_choice;
}



cl_device_id OpenCL_Choose_Platform_And_Device(std::istream& is, std::ostream& os) {

  return OpenCL_Choose_Device(OpenCL_Choose_Platform(is, os), is, os);

}
