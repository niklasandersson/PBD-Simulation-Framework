#include "opencl/OpenCL_Window.h"


void OpenCL_Window::initialize(std::istream& is, std::ostream& os) {

  cl_int error_code = CL_SUCCESS;
  
  platform_id_ = OpenCL_Choose_Platform(is, os);
  device_id_   = OpenCL_Choose_Device(platform_id_, is, os);

  // Create OpenCL context for device
  context_ = clCreateContext(
                             0, // get_context_properties() - but then there needs to be a GL context in use
                             1, 
                             &device_id_, 
                             get_callback_function_for_context_errors(), 
                             get_user_data_for_callback_function(), 
                             &error_code
                            );

  if( error_code != CL_SUCCESS ) {
    throw std::domain_error{ report_error( "Failed to create OpenCL context, '" 
                                      << OpenCL_Error_Message(error_code) << "'." ) };
  }

  // Get device properties
  string_properties_  = OpenCL_Get_String_Properties(device_id_);
  integer_properties_ = OpenCL_Get_Integer_Properties(device_id_);
  hex_properties_     = OpenCL_Get_Hex_Properties(device_id_);

  // Establish command queue
  command_queue_ = clCreateCommandQueue(
                                        context_, 
                                        device_id_, 
                                        get_command_queue_properties(), 
                                        &error_code
                                       );

  if( error_code != CL_SUCCESS ) {
    throw std::domain_error{ report_error( "OpenCL; Failed to establish command queue, '" 
                                      << OpenCL_Error_Message(error_code) << "'." ) };
  }

}


void OpenCL_Window::cleanup() {

  if( command_queue_ ) {
    CL_CALL(clReleaseCommandQueue, command_queue_);
  }

  if( context_ ) {
    CL_CALL(clReleaseContext, context_);
  }

}


cl_context_properties* OpenCL_Window::get_context_properties() {
  return 0;
}


callback_function_cl_context_errors_type OpenCL_Window::get_callback_function_for_context_errors() {
  return nullptr;
}


void* OpenCL_Window::get_user_data_for_callback_function() {
  return nullptr;
}


cl_command_queue_properties OpenCL_Window::get_command_queue_properties() {
  return 0;
}