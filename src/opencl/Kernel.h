#ifndef KERNEL_H
#define KERNEL_H

#include <CL/cl.h>

#include "opencl/OpenCL_Utilities.h"


class Kernel {

public:
  Kernel(const cl_kernel kernel, const cl_program program);
  Kernel(const Kernel& other);
  Kernel(Kernel&& other);

  Kernel& operator=(const Kernel& other);
  Kernel& operator=(Kernel&& other);

  ~Kernel();

  cl_kernel get_kernel() const;
  cl_program get_program() const;

protected:

private:
  mutable cl_kernel kernel_;
  mutable cl_program program_;

};


#endif