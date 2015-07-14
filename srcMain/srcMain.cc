#include <iostream>

#include "parser/Config.h"

int main(int argc, const char* argv[]) {

  std::cout << "Title: " << Config::getInstance().getValue<std::string>("Application", "title") << std::endl;;
  std::cout << "Version: " << Config::getInstance().getValue<std::string>("Application", "version") << std::endl;

  std::string* authors = Config::getInstance().getArray<3, std::string>("Application", "authors");
  std::cout << "Authors: " << authors[0] << ", " << authors[1] << " & " << authors[2] << std::endl;
  delete [] authors;

  std::cout << "License: " << Config::getInstance().getValue<std::string>("Application", "license") << std::endl;
  
  std::exit(EXIT_SUCCESS);

}
