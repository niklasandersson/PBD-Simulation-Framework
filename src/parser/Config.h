#ifndef CONFIG_H
#define CONFIG_H

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

protected:

private:
  Config() {
    std::string pathAndName = "tests/parser/conf1.txt";
    parser.parseFile(pathAndName);
  }

  ConfigParser<IncludeParser<DefineParser<CommentParser<Parser> > > > parser;

};

#endif