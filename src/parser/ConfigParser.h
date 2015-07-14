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
  T getValue(S... args) {

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
      std::ostringstream os{};
      bool first = true;
      for(auto& p : path) {
        if( first ) {
          first = false;
        } else {
          os << " ";
        }
        os << p;
      }
      throw std::runtime_error{ report_error( "Failed to retrive variable '" << os.str() << "' in config '" << RecursiveParser<Parser>::getFileName() << "' " << e.what() ) };
    }

    std::istringstream is{temp};
    T rtr = readValue<T>(is);

    return std::move(rtr);
  }

  template<unsigned int nArgs, typename T, typename... S>
  T* getArray(S... args) {
    std::string str = getValue<std::string>(args...);
    std::istringstream is{str};
    T* array = new T[nArgs];
    unsigned int numberOfParsedArguments = 0;
    for(unsigned int i=0; (i<nArgs) && (!is.eof()); i++) {
      std::string arg;
      std::getline(is, arg, ',');
      std::istringstream is2{arg};
      array[i] = readValue<T>(is2);
      numberOfParsedArguments++;
    }
    is >> std::ws;
    if( (numberOfParsedArguments != nArgs) || !is.eof() ) {
      std::vector<std::string> path;
      getPath(path, args...);
      std::ostringstream os{};
      bool first = true;
      for(auto& p : path) {
        if( first ) {
          first = false;
        } else {
          os << " ";
        }
        os << p;
      }
      throw std::runtime_error{ report_error( "Failed to retrive array '" << os.str() << "' in config '" << RecursiveParser<Parser>::getFileName() << "';"
            << " the array contains the wrong number of elements " ) };
    }
    return array;
  }

protected:

  void setup() override {
    // std::cout << "Setup: ConfigParser" << std::endl;
    inString_ = false;
    inArray_ = false;
    line_ = "";
    activeScope_ = globalScope_;
    scopes_.push_back(activeScope_);
  } 

  void cleanup() override {
    // std::cout << "Cleanup: ConfigParser" << std::endl;
    scopes_.clear();
  }

  virtual std::string actualLineParsing(const std::string& onePreParsedLine) {
    // std::cout << "ActualLineParsing: ConfigParser: " << onePreParsedLine << std::endl;

    std::istringstream is{onePreParsedLine};

    char c;
    // TODO: FIX INSTRING AND INARRAY

    while( is >> std::noskipws >> c ) {
      if( c == '"' ) {
        if( inString_ ) {
          inString_ = false;
        } else {
          inString_ = true;
        }
      } else if( inString_ ) {
        line_ += c;
      } else if( c == ':' ) {
        // std::cout << "Scope: " << line_ << std::endl;
        std::shared_ptr<ConfigObject> newScope = std::shared_ptr<ConfigObject>(new ConfigObject(line_));
        activeScope_->addChild(newScope);
        scopes_.push_back(newScope);
        line_ = "";
      } else if( c == '[' ) {
        inArray_ = true;
        // line_ = "";
      } else if( c == ']' ) {
        inArray_ = false;
        
        // line_ = "";
      } else if( c == '{' ) {
        activeScope_ = scopes_.back();
        line_ = "";
      } else if( c == '}' ) {
        scopes_.pop_back();
        activeScope_ = scopes_.back();
        line_ = "";
      } else if( c == ';' ) {
        std::string variable = "";
        std::string value = "";
        char equal;
        std::istringstream is2{line_};
        std::getline(is2, variable, '=');
        std::getline(is2, value, ';');

        if( variable != "" && value != "" ) {
          activeScope_->addDefine(variable, value);
          // std::cout << "Scope: " << activeScope_->getName() << " : '" << variable << "' = '" << value  << "'"<< std::endl;
        } else if( variable != "" && value == "" ) {
          throw std::runtime_error{ report_error( "Uninitialized value in config file @ " << RecursiveParser<Parser>::getFileName() << " " << RecursiveParser<Parser>::getLineNumber() ) };
        } else if( variable == "" && value != "" ) {
          throw std::runtime_error{ report_error( "Invalid config file @ " << RecursiveParser<Parser>::getFileName() << " " << RecursiveParser<Parser>::getLineNumber() ) };
        }

        line_ = "";
      } else if( c == ' ' ) {
      } else {
        line_ += c;
      }

    }

    if( inString_ ) {
      line_ += '\n';
    }

    return onePreParsedLine;
  }


private:
  template<typename T>
  typename std::enable_if<!std::is_same<T, std::string>::value, T>::type
  readValue(std::istream& is) {
    T value;
    is >> value;
    return value;
  }

  template<typename T>
  typename std::enable_if<std::is_same<T, std::string>::value, T>::type
  readValue(std::istream& is) {
    std::string value{std::istreambuf_iterator<char>(is), (std::istreambuf_iterator<char>()) };
    return value;
  }

  void getPath(std::vector<std::string>& path, const std::string& str) {
    path.push_back(str); 
  }

  template<typename... S>
  void getPath(std::vector<std::string>& path, const std::string& str, S... args) {
    path.push_back(str); 
    getPath(path, args...);
  }

  bool inString_;
  bool inArray_;
  std::string line_;
  std::shared_ptr<ConfigObject> globalScope_;
  std::shared_ptr<ConfigObject> activeScope_;
  std::vector<std::shared_ptr<ConfigObject> > scopes_;

};

#endif