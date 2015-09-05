#include <iostream>

#include "parser/Config.h"


void printInfo() {
  Config config = Config::getInstance();

  std::string title    = config.getValue<std::string>("Application.title");
  float version        = config.getValue<float>("Application.version");
  std::string* authors = config.getArray<3, std::string>("Application.authors");
  std::string license  = config.getValue<std::string>("Application.license");

  std::cout << "Title: " <<  title << std::endl;
  std::cout << "Version: " << version << std::endl;
  std::cout << "Authors: " << authors[0] << ", " << authors[1] << " & " << authors[2] << std::endl;
  std::cout << "License: " << license << std::endl;

  delete [] authors;
}
    

int main(int argc, const char* argv[]) {

  printInfo();

  std::exit(EXIT_SUCCESS);

}
