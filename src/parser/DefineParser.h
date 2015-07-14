#ifndef DEFINEPARSER_H
#define DEFINEPARSER_H

#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_map>

#include "regex/Regular_Expression.h"
#include "utility/String_Utilities.h"

#include "parser/RecursiveParser.h"

template<typename Parser>
class DefineParser : public RecursiveParser<Parser> {

public:
  DefineParser() : regex_{"A-Za-z0-9_ \n\0"} {}

  void addDefine(const std::string& name, const std::string& impl) {
    defines_[name] = impl;
  }
  
  void eraseDefine(const std::string& name) {
    defines_.erase(name);
  }
  
protected:

 
  void setup() override {
    // std::cout << "Setup: DefineParser" << std::endl;
  } 

  void cleanup() override {
    // std::cout << "Cleanup: DefineParser" << std::endl;
    defines_.clear();
  }

  virtual std::string actualLineParsing(const std::string& onePreParsedLine) override {
    // std::cout << "ActualLineParsing: DefineParser: " << onePreParsedLine << std::endl;
    
    std::string finalLine;
    std::string token;

    std::string preParsedLine = onePreParsedLine;

    for(const auto& define : defines_) {
      preParsedLine = parseString(preParsedLine, define.first, define.second);
    }

    std::istringstream is{preParsedLine};

    while( is >> token ) {

      if(token == "#define") {
        is >> token >> std::ws;
        std::string rest;
        std::getline(is, rest);
        defines_[token] = rest;
        return finalLine;
      }

      if(token == "#undef") {
        is >> token;
        defines_.erase(token);
        return finalLine;
      }

      finalLine += token;  

      if( !is.eof() ) {
        finalLine += " ";
      }
    }

    return finalLine;
  }


private:
  std::string parseString(const std::string& s11, const std::string& s22, const std::string& s33) {
    std::string s1{s11};
    std::string s2{s22};
    std::string s3{s33};
    std::vector<unsigned int> pos = Equal_middle(s1, s2);
    int j;
    unsigned int offset = 0;

    for(const auto& i : pos) {    
      if( (j = i - offset - 1) >= 0 ) {
        if( regex_.is_match(s1[j]) ) continue;
      }

      if( (j = i - offset + s2.size()) < s1.size() ) {
        if( regex_.is_match(s1[j]) ) continue;
      }

      s1.replace(i - offset, s2.size(), s3);
      offset += s2.size() - s3.size();
    }

    return s1;
  }

  const Regular_Expression regex_;
  std::unordered_map<std::string, std::string> defines_;


};

#endif