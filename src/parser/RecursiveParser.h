#ifndef RECURSIVEPARSER_H
#define RECURSIVEPARSER_H

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "parser/Parser.h"


template<typename Parser>
class RecursiveParser : public Parser {

public:
  RecursiveParser() = default;
  ~RecursiveParser() = default;

  // virtual std::string parseFile(const std::string& pathAndName) override {
  //   return Parser::parseFile(pathAndName);
  // }

  // virtual std::string parseText(const std::string& text) override {
  //   // setup();
  //   // std::string finalText = parseTextImp(text); 
  //   std::string finalText = Parser::parseText(text); 
  //   // cleanup();
  //   return finalText;
  // }

  virtual std::string parseStream(std::istream& input) override {
    setuper();
    std::string finalStream = parseStreamImp(input);
    cleanuper();
    return finalStream;
  }

  virtual std::string parseLine(const std::string& line) override {
    setuper();
    std::string finalLines = parseLineImp2(line);
    cleanuper();
    return finalLines;
  }


protected:

  virtual void setup() {
    std::cout << "Setup: RecursiveParser" << std::endl;
  }

  virtual void cleanup() {
    std::cout << "Cleanup: RecursiveParser" << std::endl;
  }

  virtual void setuper() {
    setuperImp();
    setup();
  }

  virtual void setuperImp() {
    Parser::setuperImp();
    Parser::setup();
  }

  virtual void cleanuper() {
    cleanup();
    cleanuperImp();
  }

  virtual void cleanuperImp() {
    Parser::cleanup();
    Parser::cleanuperImp();
  }

  virtual std::string parseAboveFile(const std::string& pathAndName) {
    std::string filePath;
    std::string fileName;
    try {
      size_t end_notation = pathAndName.rfind('/');
      filePath = pathAndName.substr(0, end_notation+1);   
      fileName = pathAndName.substr(end_notation+1, pathAndName.size());
    } catch(...) {
      filePath = "";
      fileName = pathAndName;
    }

    std::ifstream input{(filePath + fileName).c_str()};

    if( !input ) {
      std::ostringstream os;
      os << "Could not open file '" << filePath + fileName << "'";
      throw std::invalid_argument{os.str()};
    }

    return parseAboveStream(input);
  }


  virtual std::string parseAboveStream(std::istream& input) {
    std::ostringstream output;
    std::string line = "";
    char c;
    unsigned int lineNumber = 1;

    while( input >> std::noskipws >> c ) {
      
      if( c == '\n' ) {
        output << parseLineImp(line) << std::endl;
        lineNumber++;
        line = "";
      } else {
        line += c;
      }

    }

    output << parseLineImp(std::move(line));

    return output.str();
  }

  virtual std::string parseAboveText(const std::string& text) {
    std::istringstream input{text};
    return parseAboveStream(input);
  }

  virtual std::string parseAboveLine(const std::string& line) {
    return RecursiveParser<Parser>::parseLineImp(line);
  }


  virtual std::string parseFileImp(const std::string& pathAndName) override {
    std::string parsedFile = Parser::parseFileImp(pathAndName);
    return parsedFile;
  }

  virtual std::string parseStreamImp(std::istream& input) override {
    std::string parsedStream = Parser::parseStreamImp(input);
    return parsedStream;
  }


  virtual std::string parseLineImp(const std::string& inputStr) override {



    std::istringstream input{Parser::parseLineImp(inputStr)};

    std::ostringstream output;
    std::string line = "";
    char c;

     while( input >> std::noskipws >> c ) {
      
      if( c == '\n' ) {
        output << Parser::actualLineParsing(line) << std::endl;
        line = "";
      } else {
        line += c;
      }

    }

    output << Parser::actualLineParsing(std::move(line));

    return output.str();

  }

  virtual std::string parseLineImp2(const std::string& inputStr) override {

    std::istringstream input{parseLineImp(inputStr)};

    std::ostringstream output;
    std::string line = "";
    char c;

     while( input >> std::noskipws >> c ) {
      
      if( c == '\n' ) {
        output << actualLineParsing(line) << std::endl;
        line = "";
      } else {
        line += c;
      }

    }

    output << actualLineParsing(std::move(line));

    return output.str();

  }


  virtual std::string actualLineParsing(const std::string& onePreParsedLine) {
    std::cout << "ActualLineParsing: RecursiveParser: " << onePreParsedLine << std::endl;
    return onePreParsedLine;
  }

private:

};


#endif 