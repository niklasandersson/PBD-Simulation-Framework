#ifndef OPENCL_WINDOW_H
#define OPENCL_WINDOW_H

#include <map>
#include <iostream>

#include <CL/cl.h>

#include "exception/Error.h"

#include "opencl/OpenCL_Utilities.h"


using callback_function_cl_context_errors_type = void (*)(const char*, const void*, size_t, void*);

class OpenCL_Window {

public:
  OpenCL_Window() = default;
  virtual ~OpenCL_Window() = default;

  virtual void initialize(std::istream& is = std::cin, std::ostream& os = std::cout);
  virtual void cleanup();

  virtual cl_context_properties* get_context_properties();
  virtual callback_function_cl_context_errors_type get_callback_function_for_context_errors();
  virtual void* get_user_data_for_callback_function();
  virtual cl_command_queue_properties get_command_queue_properties();

protected:
  cl_platform_id platform_id_;
  cl_device_id device_id_;
  cl_context context_;
  cl_command_queue command_queue_;

  std::map<cl_device_info, String_Property> string_properties_;
  std::map<cl_device_info, Integer_Property> integer_properties_;
  std::map<cl_device_info, Hex_Property> hex_properties_;

};


#endif // OPENCL_WINDOW_H