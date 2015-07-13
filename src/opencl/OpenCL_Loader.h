#ifndef OPENCL_LOADER_H
#define OPENCL_LOADER_H

#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <string.h>

#include <CL/cl.h>

#include "exception/Error.h"
#include "opencl/OpenCL_Utilities.h"

#include "opencl/Kernel.h"
#include "parser/Parser.h"

#include "log/Log.h"
#include "parser/Parser.h"

class OpenCL_Loader {

public:

  static OpenCL_Loader& getInstance() {
    static OpenCL_Loader loader{};
    return loader;
  };

  OpenCL_Loader() = default;
  ~OpenCL_Loader();

  Kernel* access_kernel(const std::string& name);

  void load_kernel(cl_context context, cl_device_id device_id,
                   const std::string& name, const std::string& path, std::ostream& os = std::cout);
  void load_kernels(cl_context context, cl_device_id device_id,
                    const std::string& file, std::ostream& os = std::cout);

  void print_available_kernels(std::ostream& os = std::cout);

protected:

private:
  std::unordered_map<std::string, Kernel*> kernels_;

  Parser code_parser_;
  Parser kernels_parser_;

};


#endif // OPENCL_LOADER_H