#include "test/TestClass.h"

#include "parser/Parser.h"

BATCH_CLASS(ParserBatch) {

  BATCH(ParseTextBatch) {

    TEST(ParseTextTest1) {
      std::string textToParse = "Hej this is jocke!\nHej this is rad2.\n";
      Parser parser;
      return EQUAL(parser.parseText(textToParse), textToParse);
    };

    TEST(ParseTextTest2) {
      std::string textToParse = "Hej this is jocke!\nHej this is rad2.";
      Parser parser;
      return EQUAL(parser.parseText(textToParse), textToParse);
    };

    TEST(ParseTextTest3) {
      std::string textToParse = "Hej this is jocke!\nHej this is rad2.\n\n\n";
      Parser parser;
      return EQUAL(parser.parseText(textToParse), textToParse);
    };

    TEST(ParseTextTest4) {
      std::string textToParse = "\n\n\nHej this is jocke!\n\n\nHej this is rad2.\n\n\n";
      Parser parser;
      return EQUAL(parser.parseText(textToParse), textToParse);
    };

  };

  BATCH(ParseFileBatch) {

    TEST(ParseFileTest1) {
      std::string pathAndName = "tests/parser/testFile1.txt";
      std::ifstream ifs(pathAndName);
      std::string textInFile{std::istreambuf_iterator<char>(ifs), (std::istreambuf_iterator<char>()) };
      // log_ << "textInFile:" << std::endl;
      // log_ << textInFile;
      Parser parser;
      return EQUAL(parser.parseFile(pathAndName), textInFile);
    };


    TEST(ParseFileTest2) {
      std::string pathAndName = "tests/parser/testFile2.txt";
      std::ifstream ifs(pathAndName);
      std::string textInFile{std::istreambuf_iterator<char>(ifs), (std::istreambuf_iterator<char>()) };
      // log_ << "textInFile:" << std::endl;
      // log_ << textInFile;
      Parser parser;
      return EQUAL(parser.parseFile(pathAndName), textInFile);
    };

    TEST(ParseFileTest3) {
      std::string pathAndName = "tests/parser/testFile3.txt";
      std::ifstream ifs(pathAndName);
      std::string textInFile{std::istreambuf_iterator<char>(ifs), (std::istreambuf_iterator<char>()) };
      // log_ << "textInFile:" << std::endl;
      // log_ << textInFile;
      Parser parser;
      return EQUAL(parser.parseFile(pathAndName), textInFile);
    };


  };


};