#include "opencl/Kernel.h"


Kernel::Kernel(const cl_kernel kernel, const cl_program program)
: kernel_{kernel}, program_{program} {}


Kernel::Kernel(const Kernel& other)
: kernel_{other.get_kernel()}, program_{other.get_program()} {}


Kernel::Kernel(Kernel&& other) 
: kernel_{other.get_kernel()}, program_{other.get_program()} {}


Kernel& Kernel::operator=(const Kernel& other) {
  kernel_ = other.get_kernel();
  program_ = other.get_program();
  return *this;
}


Kernel& Kernel::operator=(Kernel&& other) {
  kernel_ = other.get_kernel();
  program_ = other.get_program();
  return *this;
}


Kernel::~Kernel() {

  if( kernel_ ) {
    CL_CALL(clReleaseKernel, kernel_);
  }

  if( program_ ) {
    CL_CALL(clReleaseProgram, program_);
  }

}


cl_kernel Kernel::get_kernel() const {
  return kernel_;
}


cl_program Kernel::get_program() const {
  return program_;
}