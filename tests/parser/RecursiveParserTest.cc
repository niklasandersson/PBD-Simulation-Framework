#include "test/TestClass.h"

#include "parser/Parser.h"
#include "parser/RecursiveParser.h"
#include "parser/CommentParser.h"
#include "parser/IncludeParser.h"
#include "parser/DefineParser.h"
#include "parser/ConfigParser.h"
#include "parser/Config.h"


BATCH_CLASS(RecursiveParserBatch) {

  // TEST(RecursiveParserTest1) {
  //   std::string pathAndName = "tests/parser/testFile1.txt";
  //   std::ifstream ifs(pathAndName);
  //   std::string textInFile{std::istreambuf_iterator<char>(ifs), (std::istreambuf_iterator<char>()) };
  //   RecursiveParser<Parser> parser;
  //   return EQUAL(parser.parseFile(pathAndName), textInFile);
  // }; 

  // TEST(RecursiveParserTest2) {
  //   std::string pathAndName = "tests/parser/testFile1.txt";
  //   std::ifstream ifs(pathAndName);
  //   std::string textInFile{std::istreambuf_iterator<char>(ifs), (std::istreambuf_iterator<char>()) };

  //   CommentParser<Parser> parser;
  //   return EQUAL(parser.parseFile(pathAndName), textInFile);
  // };

  // TEST(RecursiveParserTest3) {
  //   std::string pathAndName = "tests/parser/testFile1.txt";
  //   std::ifstream ifs(pathAndName);
  //   std::string textInFile{std::istreambuf_iterator<char>(ifs), (std::istreambuf_iterator<char>()) };

  //   IncludeParser<CommentParser<Parser> > parser;
  //   return EQUAL(parser.parseFile(pathAndName), textInFile);
  // };

  // // TEST(RecursiveParserTest4) {
  // //   std::string pathAndName = "tests/parser/testFile1.txt";
  // //   std::ifstream ifs(pathAndName);
  // //   std::string textInFile{std::istreambuf_iterator<char>(ifs), (std::istreambuf_iterator<char>()) };

  // //   CommentParser<IncludeParser<CommentParser<RecursiveParser<Parser> > > > parser;

  // //   std::string parsedFile = parser.parseFile(pathAndName);
  // //   log_ << "Parsed file: " << std::endl;
  // //   log_ << parsedFile << std::endl;
  // //   return EQUAL(parsedFile, textInFile);
  // // };



  // TEST(RecursiveParserTest5) {
  //   std::string line = "hej detta e jocke";
  //   IncludeParser<CommentParser<Parser> > parser;
  //   std::string parsedLine = parser.parseLine(line);
  //   log_ << "Parsed line: " << parsedLine << std::endl;
  //   return EQUAL(parsedLine, line);
  // };


  // TEST(RecursiveParserTest6) {
  //   std::string pathAndName = "tests/parser/testFile1.txt";
  //   std::ifstream ifs(pathAndName);
  //   std::string textInFile{std::istreambuf_iterator<char>(ifs), (std::istreambuf_iterator<char>()) };
  //   std::string copy = textInFile;
  //   IncludeParser<CommentParser<Parser> > parser;
  //   return EQUAL(parser.parseText(copy), textInFile);
  // };

  // TEST(RecursiveParserTest7) {
  //   std::string pathAndName = "tests/parser/testFile1.txt";
  //   std::ifstream ifs(pathAndName);
  //   std::string textInFile{std::istreambuf_iterator<char>(ifs), (std::istreambuf_iterator<char>()) };
  //   std::string copy = textInFile;
  //   IncludeParser<CommentParser<Parser> > parser;
  //   std::istringstream is{copy};
  //   return EQUAL(parser.parseStream(is), textInFile);
  // };


  // TEST(RecursiveParserTest8) {
  //   std::string pathAndName = "tests/parser/testFile1.txt";
  //   std::ifstream ifs(pathAndName);
  //   std::string textInFile{std::istreambuf_iterator<char>(ifs), (std::istreambuf_iterator<char>()) };

  //   IncludeParser<CommentParser<IncludeParser<IncludeParser<Parser> > > > parser;
  //   return EQUAL(parser.parseFile(pathAndName), textInFile);
  // };

  BATCH(IncludeParserBatch) {

    TEST(IncludeParserTest1) {
      std::string pathAndName = "tests/parser/inc1.txt";
      std::string textInFile = "\nDetta är fil 1.\n\nDetta är fil 2.\n\n\n\n\nDetta är fil 3.\nDetta är fil 4.\n\nJOCKE+JOCKE";
      CommentParser<IncludeParser<DefineParser<Parser> > > parser;
      std::string result = parser.parseFile(pathAndName);
      std::cout << result << std::endl;
      return EQUAL(result, textInFile);
    };

  };


  BATCH(ConfigParserBatch) {
    TEST(IncludeParserTest1) {
      std::string pathAndName = "tests/parser/conf1.txt";
      std::string textInFile = "text";
      ConfigParser<IncludeParser<DefineParser<CommentParser<Parser> > > > parser;
      std::string result = parser.parseFile(pathAndName);
      std::cout << result << std::endl;

      int myValue = parser.getValue<int>("application", "b");
      std::cout << "myValue: " << myValue << std::endl;

      unsigned int myValue2 = parser.getValue<unsigned int>("application", "group2", "y");
      std::cout << "myValue2: " << myValue2 << std::endl;

      unsigned int myValue3 = parser.getValue<unsigned int>("application", "group1", "y");
      std::cout << "myValue3: " << myValue3 << std::endl;

      std::string myValue4 = parser.getValue<std::string>("e");
      std::cout << "myValue4: " << myValue4 << std::endl;

      double myValue5 = parser.getValue<double>("application", "group2", "z");
      std::cout << "myValue5: " << myValue5 << std::endl;

      double myValue6 = parser.getValue<double>("application", "d");
      std::cout << "myValue6: " << myValue6 << std::endl;

      std::string myValue7 = parser.getValue<std::string>("application", "c");
      std::cout << "myValue7: " << myValue7 << std::endl;

      return EQUAL(result, textInFile);
    };

    TEST(ConfigTest1) {
      log_ << Config::getInstance().getValue<std::string>("application", "c") << std::endl;
      log_ << Config::getInstance().getValue<std::string>("application", "group2", "z") << std::endl;
      return true;
    };

  }; 
  

};