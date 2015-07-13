#include "opencl/Demo_CL_Window.h"


void Demo_CL_Window::initialize(std::istream& is, std::ostream& os) {
  OpenCL_Window::initialize(is, os);

  bool has_gl_sharing = Equal_middle(
                                     string_properties_[CL_DEVICE_EXTENSIONS].value_,
                                     "cl_khr_gl_sharing"
                                    ).size() > 0;

  if( !has_gl_sharing ) {
    os << "Error: Device does not support cl_khr_gl_sharing." << std::endl;
    if( initialize_again(is, os) ) {
      OpenCL_Window::cleanup();
      OpenCL_Window::initialize(is, os);
    } else {
      throw std::domain_error{ report_error( "Device does not support cl_khr_gl_sharing." ) };
    }
  }

  // bool has_double_precision = Equal_middle(
  //                                          string_properties_[CL_DEVICE_EXTENSIONS].value_,
  //                                          "cl_khr_fp64"
  //                                         ).size() > 0;

  // if( !has_double_precision ) {
  //   os << "Error: Device does not support cl_khr_fp64." << std::endl;
  //   if( initialize_again(is, os) ) {
  //     OpenCL_Window::cleanup();
  //     OpenCL_Window::initialize(is, os);
  //   } else {
  //     throw std::domain_error{ report_error( "Device does not support cl_khr_fp64." ) };
  //   }
  // }

}


void Demo_CL_Window::cleanup() {


  OpenCL_Window::cleanup();
}


cl_context_properties* Demo_CL_Window::get_context_properties() {

#if _WIN32

    context_properties_[0] = CL_GL_CONTEXT_KHR;
    context_properties_[1] = (cl_context_properties) wglgetCurrentContext();

    context_properties_[2] = CL_WGL_HDC_KHR;
    context_properties_[3] = (cl_context_properties) wglGetCurrentDC();

#elif __linux__

    context_properties_[0] = CL_GL_CONTEXT_KHR;
    context_properties_[1] = (cl_context_properties) glXGetCurrentContext();

    context_properties_[2] = CL_GLX_DISPLAY_KHR;
    context_properties_[3] = (cl_context_properties) glXGetCurrentDisplay();

#endif

  context_properties_[4] = CL_CONTEXT_PLATFORM;
  context_properties_[5] = (cl_context_properties) platform_id_;
  context_properties_[6] = 0; // <-- Null ending is needed

  return context_properties_;

}


bool Demo_CL_Window::initialize_again(std::istream& is, std::ostream& os) {
  char confirm;
  std::string line;
  bool has_read_vaild_value = false;
  while( !has_read_vaild_value ) {
     os << "Do you want to choose another device? (y/n): ";
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
  
  if( confirm == 'y' || confirm == 'Y' ) {
    return true;
  } 

  return false;
}


cl_context Demo_CL_Window::get_context() const {
  return context_;
}

cl_command_queue Demo_CL_Window::get_command_queue() const {
  return command_queue_;
}

cl_device_id Demo_CL_Window::get_device_id() const {
  return device_id_;
} 
