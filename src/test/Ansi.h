#ifndef ANSI_H
#define ANSI_H

#include <iostream>
#include <string>
#include <sstream>


// http://softkube.com/blog/ansi-command-line-colors-under-windows

class Ansi {

public:
  enum Type { NORMAL, BOLD };
  enum Color { BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN , WHITE };

  static std::string color(const std::string& str, Color color, Type type = NORMAL) {
    std::ostringstream os;

    if( type == NORMAL ) {
      os << "\033[3" << color << "m" << str << "\033[0m";
    } else if( type == BOLD ) {
      os << "\033[1;3" << color << "m" << str << "\033[0m";
    }
    
    return os.str();
  }

  static std::string black(const std::string& str, Type type = NORMAL) {
    return color(str, BLACK, type);
  }

  static std::string red(const std::string& str, Type type = NORMAL) {
    return color(str, RED, type);
  }

  static std::string green(const std::string& str, Type type = NORMAL) {
    return color(str, GREEN, type);
  }

  static std::string yellow(const std::string& str, Type type = NORMAL) {
    return color(str, YELLOW, type);
  }

  static std::string blue(const std::string& str, Type type = NORMAL) {
    return color(str, BLUE, type);
  }

  static std::string magenta(const std::string& str, Type type = NORMAL) {
    return color(str, MAGENTA, type);
  }

  static std::string cyan(const std::string& str, Type type = NORMAL) {
    return color(str, CYAN, type);
  }

  static std::string white(const std::string& str, Type type = NORMAL) {
    return color(str, WHITE, type);
  }

};


#endif