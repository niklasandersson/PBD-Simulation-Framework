#ifndef GLFW_WINDOW_H
#define GLFW_WINDOW_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <atomic>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "opengl/OpenGL_Window.h"
#include "exception/Error.h"


#define N_FRAMETIMES 100
#define PRINT_FPS_INTERVAL 1.0

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
  void set_print_fps(const bool printFps);

protected:
  GLFW_Window(const unsigned int window_width, const unsigned int window_height, const std::string& window_title);

  virtual void set_glfw_window_hints() = 0;

  GLFWwindow* glfw_window_;

  double current_time_;
  double delta_time_;
  double fps_;

  std::vector<double> frame_times_;
  unsigned int frame_time_index_;

  std::atomic<bool> printFps_;
  double lastPrintTime_;

private:
  double last_time_;
  std::atomic<bool> shouldClose_;


};


#endif // GLFW_WINDOW_H