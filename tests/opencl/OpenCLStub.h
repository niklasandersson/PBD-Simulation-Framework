#ifndef OPENCLSTUB_H
#define OPENCLSTUB_H

#include <CL/cl.h>
#include <CL/cl_gl.h>
#ifdef __linux__
  #include <GL/glx.h>
#endif

#include "exception/Error.h"
#include "opencl/Demo_CL_Window.h"
#include "opencl/OpenCL_Loader.h"
#include "parser/Parser.h"


class OpenCLStub {

public:
  static OpenCLStub& getInstance() {
    static OpenCLStub stub{};
    return stub;
  };


  cl_context getContext() {
    return Demo_CL_Window::getInstance().get_context();
  }


  cl_command_queue getCommandQueue() {
    return Demo_CL_Window::getInstance().get_command_queue();
  }


  cl_kernel getKernel(std::string kernel) {
    return OpenCL_Loader::getInstance().access_kernel(kernel)->get_kernel();
  }


  ~OpenCLStub() = default;


protected:

private:
  OpenCLStub() {
    Parser parser;
    std::string openCL_config = parser.parseFile("config.txt");

    std::istringstream is{openCL_config};
    std::ostringstream os;

    try {
      // Demo_CL_Window::getInstance().initialize(is, os);
      Demo_CL_Window::getInstance().initialize();
    } catch(const std::domain_error& e) {
      print_error(e);
      std::exit(EXIT_FAILURE);
    }

    cl_device_id device_id_ = Demo_CL_Window::getInstance().get_device_id();
    cl_context context_ = Demo_CL_Window::getInstance().get_context();

    try {
      OpenCL_Loader::getInstance().load_kernels(context_, device_id_, "src/app/kernel/kernels.prog", os);
    } catch(const std::invalid_argument& e) {
      print_error(e);
      std::exit(EXIT_FAILURE);
    }

    os << "Available kernels: " << std::endl;
    OpenCL_Loader::getInstance().print_available_kernels(os);
  }

};


#endif