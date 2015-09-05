#include "test/TestClass.h"

#include "parser/Config.h"
#include "parser/ConfigParser.h"

BATCH_CLASS(ConfigTest) {

  TEST(ConfigOutputTest1) {

    std::cout << "Title: " << Config::getInstance().getValue<std::string>("Application", "title") << std::endl;
    std::cout << "Version: " << Config::getInstance().getValue<double>("Application", "version") << std::endl;

    std::string* authors = Config::getInstance().getArray<3, std::string>("Application", "authors");
    std::cout << "Authors: " << authors[0] << ", " << authors[1] << " & " << authors[2] << std::endl;
    delete [] authors;

    std::cout << "License: " << Config::getInstance().getValue<std::string>("Application", "license") << std::endl;
    
    return true;
  };

  TEST(ConfigOutputTest2) {

    Config config = Config::getInstance();

    std::cout << "Title: " << config.getValue<std::string>("Application.title") << std::endl;
    std::cout << "Version: " << config.getValue<double>("Application.version") << std::endl;

    std::string* authors = config.getArray<3, std::string>("Application.authors");
    std::cout << "Authors: " << authors[0] << ", " << authors[1] << " & " << authors[2] << std::endl;
    delete [] authors;

    std::cout << "License: " << config.getValue<std::string>("Application.license") << std::endl;

    return true;
  };

};
