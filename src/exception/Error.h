#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <exception>
#include <sstream>
#include <typeinfo>


#define report_error(e) [&](const std::string& file, const unsigned int line) \
                              {  \
                                std::ostringstream macro_error_os{}; \
                                macro_error_os << "@ " << file << " " << line << "; " << e; \
                                return macro_error_os.str(); \
                              }(__FILE__, __LINE__)

void print_error(const std::exception& e, std::ostream& os = std::cerr);

#endif // ERROR_H