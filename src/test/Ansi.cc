#include "test/Ansi.h"


std::string Ansi::color(const std::string& str, Color color, Type type) {
  std::ostringstream os;

  if( type == NORMAL ) {
    os << "\033[3" << color << "m" << str << "\033[0m";
  } else if( type == BOLD ) {
    os << "\033[1;3" << color << "m" << str << "\033[0m";
  }
  
  return os.str();
}
