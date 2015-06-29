#ifndef ANSI_H
#define ANSI_H

#include <iostream>
#include <string>
#include <sstream>


class Ansi {

public:
  enum Type { NORMAL, BOLD };
  enum Color { BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN , WHITE };

  static std::string color(const std::string& str, Color color, Type type = NORMAL);

};


#endif