#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <sstream>
#include <algorithm>
#include <typeinfo>
#include <stdexcept>

#include <exception/Error.h>

#include "parser/Parser.h"
#include "parser/CommentParser.h"
#include "parser/IncludeParser.h"
#include "parser/DefineParser.h"
#include "parser/ConfigParser.h"

class Config {

public:
  static Config& getInstance() {
    static Config instance;
    return instance;
  }

  ~Config() = default;

  template<typename T, typename S> 
  T getValue(S arg) {
    std::vector<std::string> path = split(arg);
    return parser.getValueImpl<T>(path);
  }

  template<typename T, typename S1, typename... S2>
  T getValue(S1 arg, S2... args) {
    return parser.getValue<T>(arg, args...);
  }

  template<unsigned int nArgs, typename T, typename S>
  T* getArray(S arg) {
    std::vector<std::string> path = split(arg);
    return parser.getArrayImpl<nArgs, T>(path);
  }

  template<unsigned int nArgs, typename T, typename S1, typename... S2>
  T* getArray(S1 arg, S2... args) {
    return parser.getArray<nArgs, T>(arg, args...);
  }

protected:

private:
  Config() {
    std::string pathAndName = "config.txt";
    parser.addDefine("true", "1");
    parser.addDefine("false", "0");
    parser.parseFile(pathAndName);
  }

  std::vector<std::string> split(const std::string& arg) {
    std::istringstream is{arg};
    std::vector<std::string> path;
    std::string temp;
    while( std::getline(is, temp, '.') ) {
      path.push_back(temp);
    }
    return path;
  }

  ConfigParser<IncludeParser<DefineParser<CommentParser<Parser> > > > parser;

};

#endif