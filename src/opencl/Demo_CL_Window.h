#ifndef DEMO_CL_WINDOW_H
#define DEMO_CL_WINDOW_H

#include <iostream>

#include <CL/cl.h>
#include <CL/cl_gl.h>
#ifdef __linux__
  #include <GL/glx.h>
#endif

#include "opencl/OpenCL_Window.h"

#include "exception/Error.h"


class Demo_CL_Window :  public OpenCL_Window {

public:

  static Demo_CL_Window& getInstance() {
    static Demo_CL_Window window{};
    return window;
  };

  Demo_CL_Window() = default;
  ~Demo_CL_Window() = default;

  void initialize(std::istream& is = std::cin, std::ostream& os = std::cout) override;
  void cleanup() override;

  cl_context_properties* get_context_properties() override;

  cl_context get_context() const;
  cl_command_queue get_command_queue() const;
  cl_device_id get_device_id() const;

protected:

private:
  bool has_gl_sharing_;

  cl_context_properties context_properties_[6];

  bool initialize_again(std::istream& is, std::ostream& os);

};


#endif // DEMO_CL_WINDOW_H