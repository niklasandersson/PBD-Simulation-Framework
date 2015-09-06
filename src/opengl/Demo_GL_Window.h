#ifndef DEMO_GL_WINDOW_H
#define DEMO_GL_WINDOW_H

#include "glfw/GLFW_Window.h"

#include "opengl/OpenGL_Loader.h"

#include "app/Scene.h"

class Demo_GL_Window : public GLFW_Window {

public:
  Demo_GL_Window(const unsigned int window_width = 800, 
                 const unsigned int window_height = 600, 
                 const std::string& window_title = "Demo_GL_Window",
                 const unsigned int multisampling_and_antialiasing = 4,
                 const unsigned int opengl_major_version = 3,
                 const unsigned int opengl_minor_version = 3);

  ~Demo_GL_Window();

  virtual void initialize() override;
  virtual void render() override;
  virtual void cleanup() override;

protected:
  virtual void set_glfw_window_hints() override;

private:
  void loadPrograms();

  unsigned int multisampling_and_antialiasing_;
  unsigned int opengl_major_version_;
  unsigned int opengl_minor_version_;

  Scene* scene_;

};

#endif