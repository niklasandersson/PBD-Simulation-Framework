#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <memory>
#include <cstdarg>
#include <sstream>
#include <typeinfo>

#include "parser/RecursiveParser.h"
#include "parser/ConfigObject.h"

template<typename Parser>
class ConfigParser : public RecursiveParser<Parser> {

public:
  ConfigParser() : globalScope_(new ConfigObject("global")) {}  
  
  template<typename T, typename... S>
  typename std::enable_if<std::is_same<T, std::string>::value, T>::type
  getValue(S... args) {

    std::vector<std::string> path;
    getPath(path, args...);

    T rtr;
    try {
      std::shared_ptr<ConfigObject> scope = globalScope_;
      for(unsigned int i=0; i<path.size()-1; i++) {
        scope = scope->getChild(path[i]);
      }
      rtr = scope->getDefine(path[path.size()-1]);
    } catch(const std::exception& e) {
      throw std::runtime_error{ report_error( "Failed to parse config '" << RecursiveParser<Parser>::getFileName() << "' " << e.what() ) };
    }
    return rtr;
  }

  template<typename T, typename... S>
  typename std::enable_if<!std::is_same<T, std::string>::value, T>::type
  getValue(S... args) {

    std::vector<std::string> path;
    getPath(path, args...);

    std::string temp;
    try {
      std::shared_ptr<ConfigObject> scope = globalScope_;
      for(unsigned int i=0; i<path.size()-1; i++) {
        scope = scope->getChild(path[i]);
      }
      temp = scope->getDefine(path[path.size()-1]);
    } catch(const std::exception& e) {
      throw std::runtime_error{ report_error( "Failed to parse config '" << RecursiveParser<Parser>::getFileName() << "' " << e.what() ) };
    }

    std::istringstream is{temp};
    T rtr;
    is >> rtr;

    return std::move(rtr);
  }

protected:

  void setup() override {
    std::cout << "Setup: ConfigParser" << std::endl;
    inString_ = false;
    activeScope_ = globalScope_;
    scopes_.push_back(activeScope_);
  } 

  void cleanup() override {
    std::cout << "Cleanup: ConfigParser" << std::endl;
    scopes_.clear();
  }

  virtual std::string actualLineParsing(const std::string& onePreParsedLine) {
    std::cout << "ActualLineParsing: ConfigParser: " << onePreParsedLine << std::endl;

    std::string finalLine;
    std::istringstream is{onePreParsedLine};

    std::string line = "";
    char c;

    while( is >> std::noskipws >> c ) {
      if( c == '"' ) {
        if( inString_ ) {
          inString_ = false;
        } else {
          inString_ = true;
        }
      } else if( inString_ ) {
        line += c;
      } else if( c == ':' ) {
        std::cout << "Scope: " << line << std::endl;
        std::shared_ptr<ConfigObject> newScope = std::shared_ptr<ConfigObject>(new ConfigObject(line));
        activeScope_->addChild(newScope);
        scopes_.push_back(newScope);
        line = "";
      } else if( c == '{' ) {
        activeScope_ = scopes_.back();
      } else if( c == '}' ) {
        scopes_.pop_back();
        activeScope_ = scopes_.back();
      } else if( c == ';' ) {
        std::string variable = "";
        std::string value = "";
        char equal;
        std::istringstream is2{line};
        std::getline(is2, variable, '=');
        std::getline(is2, value, ';');

        if( variable != "" && value != "" ) {
          activeScope_->addDefine(variable, value);
          std::cout << "Scope: " << activeScope_->getName() << " : '" << variable << "' = '" << value  << "'"<< std::endl;
        } else if( variable != "" && value == "" ) {
          throw std::runtime_error{ report_error( "Uninitialized value in config file @ " << RecursiveParser<Parser>::getFileName() << " " << RecursiveParser<Parser>::getLineNumber() ) };
        } else if( variable == "" && value != "" ) {
          throw std::runtime_error{ report_error( "Invalid config file @ " << RecursiveParser<Parser>::getFileName() << " " << RecursiveParser<Parser>::getLineNumber() ) };
        }

        line = "";
      } else if( c == ' ' ) {
      } else {
        line += c;
      }

    }

    return finalLine;
  }


private:
  void getPath(std::vector<std::string>& path, const std::string& str) {
    path.push_back(str); 
  }

  template<typename... S>
  void getPath(std::vector<std::string>& path, const std::string& str, S... args) {
    path.push_back(str); 
    getPath(path, args...);
  }

  bool inString_;
  std::shared_ptr<ConfigObject> globalScope_;
  std::shared_ptr<ConfigObject> activeScope_;
  std::vector<std::shared_ptr<ConfigObject> > scopes_;

};

#endif