#ifndef OPENCL_UTILITIES_H
#define OPENCL_UTILITIES_H

#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include <sstream>
#include <string>
#include <limits>
#include <stdexcept>
#include <type_traits>

#include <CL/cl.h>
#include <CL/cl_gl.h>
#ifdef __linux__
  #include <GL/glx.h>
#endif

#include "exception/Error.h"

#include "utility/String_Utilities.h"

#define OPENCL_MAX_VENDOR_NAME_LENGTH 1024


const char* OpenCL_Error_Message(const cl_int& error_code);

namespace CL_CALL_UTILITY {

  template<class... Args, class... Bargs>
  inline cl_int CL_Call_Function(const char* file, const unsigned int line, 
                                 cl_int(&function)(Args...), Bargs&&... bargs) {
    static_assert(sizeof...(Args) == sizeof...(Bargs), "Wrong number of arguments given.");
    cl_int error_code{function(std::forward<Bargs>(bargs)...)};
    if( error_code != CL_SUCCESS ) {
      std::cerr << "Assert: CL_CALL failed in file " << file 
                << " at line " << line 
                << " : " << OpenCL_Error_Message(error_code)
                << std::endl;
      std::exit(EXIT_FAILURE);
    }
    return std::move(error_code);
  }

  template<class Ret, class... Args, class... Bargs>
  inline Ret CL_Call_Function(const char* file, const unsigned int line, 
                              Ret(&function)(Args...), Bargs&&... bargs) {
    static_assert(sizeof...(Args) == sizeof...(Bargs) + 1, "Wrong number of arguments given.");
    cl_int error_code;
    Ret ret{function(std::forward<Bargs>(bargs)..., &error_code)};
    if( error_code != CL_SUCCESS ) {
      std::cerr << "Assert: CL_CALL failed in file " << file 
                << " at line " << line 
                << " : " << OpenCL_Error_Message(error_code)
                << std::endl;
      std::exit(EXIT_FAILURE);
    }
    return std::move(ret);
  }

}

#define CL_CALL(function, ...) CL_CALL_UTILITY::CL_Call_Function(__FILE__, __LINE__, function, __VA_ARGS__)


typedef struct {
    std::string name_;
    std::string value_;
} String_Property;

typedef struct {
    std::string name_;
    long long value_;
} Integer_Property;

typedef struct {
    std::string name_;
    long long value_;
} Hex_Property;


cl_platform_id OpenCL_Choose_Platform(std::istream& is = std::cin, std::ostream& = std::cout);
cl_device_id OpenCL_Choose_Device(const cl_platform_id& platform_id, std::istream& is = std::cin, std::ostream& = std::cout);
cl_device_id OpenCL_Choose_Platform_And_Device(std::istream& is = std::cin, std::ostream& = std::cout);

void OpenCL_Print_Device_Info(const cl_device_id& device, std::ostream& os = std::cout);

std::map<cl_device_info, String_Property> OpenCL_Get_String_Properties(const cl_device_id& device_id);
std::map<cl_device_info, Integer_Property> OpenCL_Get_Integer_Properties(const cl_device_id& device_id);
std::map<cl_device_info, Hex_Property> OpenCL_Get_Hex_Properties(const cl_device_id& device_id);

void OpenCL_Print_String_Properties(const std::map<cl_device_info, String_Property>& string_properties, std::ostream& os = std::cout);
void OpenCL_Print_String_Property(const String_Property& string_property, std::ostream& os = std::cout);

void OpenCL_Print_Integer_Properties(const std::map<cl_device_info, Integer_Property>& integer_properties, std::ostream& os = std::cout);
void OpenCL_Print_Integer_Property(const Integer_Property& integer_property, std::ostream& os = std::cout);

void OpenCL_Print_Hex_Properties(const std::map<cl_device_info, Hex_Property>& hex_properties, std::ostream& os = std::cout);
void OpenCL_Print_Hex_Property(const Hex_Property& hex_property, std::ostream& os = std::cout);

std::string OpenCL_Get_Device_Type(const cl_device_id& device_id);
std::string OpenCL_Get_Device_Execution_Capabilities(const cl_device_id& device_id);
std::string OpenCL_Get_Device_Global_Mem_Cache_Type(const cl_device_id& device_id);
std::string OpenCL_Get_Device_Local_Mem_Type(const cl_device_id& device_id);
std::vector<size_t> OpenCL_Get_Max_Work_Group_Sizes(const cl_device_id& device_id);


#endif // OPENCL_UTILITIES_H