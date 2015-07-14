#ifndef INCLUDEPARSER_H
#define INCLUDEPARSER_H

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "parser/RecursiveParser.h"

template<typename Parser>
class IncludeParser : public RecursiveParser<Parser> {

public:

protected:

 
  void setup() override {
    // std::cout << "Setup: IncludeParser" << std::endl;
    includedFiles_.push_back(RecursiveParser<Parser>::getFileName());
  } 

  void cleanup() override {
    // std::cout << "Cleanup: IncludeParser" << std::endl;
    includedFiles_.clear();
  }

  virtual std::string actualLineParsing(const std::string& onePreParsedLine) override {
    // std::cout << "ActualLineParsing: IncludeParser: " << onePreParsedLine << std::endl;
    std::string finalLine;
    std::string token;
    std::istringstream is{onePreParsedLine};

    while( is >> token ) {

      if(token == "#include") {
        is >> token;
        std::string fileToBeIncluded = token.substr(1, token.size()-2);
        if( !fileHasBeenIncluded(fileToBeIncluded) ) {
          includedFiles_.push_back(fileToBeIncluded);
          return RecursiveParser<Parser>::parseAboveFile(fileToBeIncluded); 
        } else {
          return "";
        }
      }

      finalLine += token;

      if( !is.eof() ) {
        finalLine += " ";
      }
    }

    return finalLine;

  }


private:

  bool fileHasBeenIncluded(const std::string& fileToBeIncluded) const {
    auto it = std::find(begin(includedFiles_), std::end(includedFiles_), fileToBeIncluded);
    return it != std::end(includedFiles_);
  }


  std::vector<std::string> includedFiles_;

};

#endif