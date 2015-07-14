#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <sstream>
#include <algorithm>
#include <typeinfo>
#include <stdexcept>

#include <exception/Error.h>

#include "parser/Parser.h"
#include "parser/RecursiveParser.h"
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

  template<typename T, typename... S>
  T getValue(S... args) {
    return parser.getValue<T>(args...);
  }

  template<unsigned int nArgs, typename T, typename... S>
  T* getArray(S... args) {
    return parser.getArray<nArgs, T>(args...);
  }

protected:

private:
  Config() {
    std::string pathAndName = "config.txt";
    parser.addDefine("true", "1");
    parser.addDefine("false", "0");
    parser.parseFile(pathAndName);
  }

  ConfigParser<IncludeParser<DefineParser<CommentParser<Parser> > > > parser;

};

#endif