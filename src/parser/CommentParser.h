#ifndef COMMENTPARSER_H
#define COMMENTPARSER_H

#include "parser/RecursiveParser.h"

template<typename Parser>
class CommentParser : public RecursiveParser<Parser> {

public:
  CommentParser() : inComment_{false} {}
  
protected:

  void setup() override {
    // std::cout << "Setup: CommentParser" << std::endl;
  } 

  void cleanup() override {
    // std::cout << "Cleanup: CommentParser" << std::endl;
    inComment_ = false;
  }

  virtual std::string actualLineParsing(const std::string& onePreParsedLine) {
    // std::cout << "ActualLineParsing: CommentParser: " << onePreParsedLine << std::endl;

    std::string finalLine;
    char ab[2];
    std::istringstream is{onePreParsedLine};

    while( is >> std::noskipws >> ab[0] ) {

      ab[1] = is.peek();

      if( ab[0] == '*' && ab[1] == '/' ) {
        is.ignore(1);
        inComment_ = false;
        continue;
      } else if( inComment_ ) {
        continue;
      }

      if( ab[0] == '/' && ab[1] == '*' ) {
        is.ignore(1);
        inComment_ = true;
        continue;
      }

      if( ab[0] == '/' && ab[1] == '/' ) {
        break;
      }

      finalLine += ab[0];
    }

    return finalLine;
  }


private:
  bool inComment_;

};

#endif