#ifndef GLFW_WINDOW_H
#define GLFW_WINDOW_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <atomic>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "opengl/OpenGL_Window.h"
#include "exception/Error.h"

class GLFW_Window : public OpenGL_Window {

public:

  virtual ~GLFW_Window() = default;

  virtual void initialize() override;
  virtual void cleanup() override;
  virtual void render() override;

  void initialize_time(const double start_time = 0.0);
  double get_current_time() const;
  double get_delta_time() const;
  double get_fps() const;
  bool should_close() const;
  void set_should_close();

protected:
  GLFW_Window(const unsigned int window_width, const unsigned int window_height, const std::string& window_title);

  virtual void set_glfw_window_hints() = 0;

  GLFWwindow* glfw_window_;

  double current_time_;
  double delta_time_;
  double fps_;

private:
  double last_time_;
  std::atomic<bool> shouldClose_;


};


#endif // GLFW_WINDOW_H