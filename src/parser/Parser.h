#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <sstream>


class Parser {

public:
  Parser() = default;
  virtual ~Parser() = default;

  std::string getFileName() const;
  std::string getFilePath() const;
  unsigned int getLineNumber() const;

  virtual std::string parseFile(const std::string& pathAndName);
  virtual std::string parseText(const std::string& text);
  virtual std::string parseStream(std::istream& input);
  virtual std::string parseLine(const std::string& line);


protected:

  template<typename P>
  std::string extractLines(std::istream& input, P* parser, std::string(P::*func)(const std::string&)) const {
    std::ostringstream output;
    std::string line = "";
    char c;

     while( input >> std::noskipws >> c ) {
      
      if( c == '\n' ) {
        output << ((parser)->*(func))(line) << std::endl;
        line = "";
      } else {
        line += c;
      }

    }

    output << ((parser)->*(func))(std::move(line));

    return output.str();
  }


  virtual void setup() {
    // std::cout << "Setup: Parser" << std::endl;
  }

  virtual void cleanup() {
    // std::cout << "Cleanup: Parser" << std::endl;
  }

  virtual void setuper() {}
  virtual void setuperImp() {}

  virtual void cleanuper() {}
  virtual void cleanuperImp() {}


  virtual std::string actualLineParsing(const std::string& onePreParsedLine) {
    // std::cout << "ActualLineParsing: Parser: " << onePreParsedLine << std::endl;
    return onePreParsedLine;
  }

  virtual std::string parseFileImp(const std::string& pathAndName) {
    return Parser::parseFile(pathAndName);
  }

  virtual std::string parseLineImp(const std::string& line) {
    return line;
  }

  virtual std::string parseLineImp2(const std::string& line) {
    return line;
  }

  virtual std::string parseStreamImp(std::istream& input) {
    return Parser::parseStream(input);
  }


private:
  bool fileNameSet_{false};
  std::string fileName_;
  bool filePathSet_{false};
  std::string filePath_;
  unsigned int lineNumber_{1};

};


#endif // PARSER_H