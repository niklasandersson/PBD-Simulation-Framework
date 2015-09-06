#include <iostream>
#include <memory>

#include "parser/Config.h"

#include "opengl/Demo_GL_Window.h"

#include "console/Console.h"


void printApplicationInfo() {
  Config& config = Config::getInstance();

  std::string name     = config.getValue<std::string>("Application.name");
  float version        = config.getValue<float>("Application.version");
  std::string* authors = config.getArray<3, std::string>("Application.authors");
  std::string license  = config.getValue<std::string>("Application.license");

  std::cout << "------------------------------------------------------------------------" << std::endl;
  std::cout << "Name: " <<  name << std::endl;
  std::cout << "Version: " << version << std::endl;
  std::cout << "Authors: " << authors[0] << ", " << authors[1] << " & " << authors[2] << std::endl;
  std::cout << "License: " << license << std::endl;
  std::cout << "------------------------------------------------------------------------" << std::endl;

  delete [] authors;
}
    

int main(int argc, const char* argv[]) {

  printApplicationInfo();

  Config& config = Config::getInstance();

  Demo_GL_Window glWindow(config.getValue<unsigned int>("Application.OpenGL.width"),
                          config.getValue<unsigned int>("Application.OpenGL.height"),
                          config.getValue<std::string>("Application.name"));

  glWindow.initialize();

  std::shared_ptr<Console> console = Console::getInstance();

  console->add("quit", [&](const char* args) {
    console->close();
    glWindow.set_should_close();
  });

  while( !glWindow.should_close() ) {
    glWindow.render();
  }

  glWindow.cleanup();

  std::cout << "DONE" << std::endl;

  std::exit(EXIT_SUCCESS);

}
