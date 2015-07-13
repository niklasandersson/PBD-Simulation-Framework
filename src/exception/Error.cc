#include "exception/Error.h"


void print_error(const std::exception& error, std::ostream& os) {
  os << "Error: " << typeid(error).name() << " " << error.what() << std::endl;
}