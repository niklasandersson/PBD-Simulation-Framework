#include "glfw/GLFW_Window.h"


GLFW_Window::GLFW_Window(const unsigned int window_width, const unsigned int window_height, const std::string& window_title)
: OpenGL_Window{window_width, window_height, window_title} 
, current_time_{0}
, delta_time_{0}
, fps_{0}
, shouldClose_{false}
{
  glfwSetWindowTitle(glfw_window_, window_title_.c_str());

}


void GLFW_Window::initialize() {

  // Initialize GLFW
  if( !glfwInit() ) { 
    throw std::domain_error{ report_error("Failed to initialize GLFW for window '" << window_title_ << "'.") };
  }

  set_glfw_window_hints(); // <-- Implemented in subclasses

  // Create actual window
  glfw_window_ = glfwCreateWindow(window_width_, window_height_, window_title_.c_str(), nullptr, nullptr);
  if( glfw_window_ == nullptr ) {
    glfwTerminate();
    throw std::domain_error{ report_error("Failed to open GLFW window '" << window_title_ << "'.") };
  }
  glfwMakeContextCurrent(glfw_window_);

  // Initialize GLEW
  glewExperimental = true; // <-- Needed for core profile
  if( glewInit() != GLEW_OK ) {
    throw std::domain_error{ report_error("Failed to initialize GLEW for GLFW window '" << window_title_ << "'.") };
  }

}


void GLFW_Window::cleanup() {

  if( glfw_window_ ) {
    glfwDestroyWindow(glfw_window_);
  }

  glfwTerminate();

}


void GLFW_Window::render() {
  current_time_ = glfwGetTime();
  delta_time_ = current_time_ - last_time_;
  last_time_ = current_time_;
  fps_ = 1 / delta_time_;
}


void GLFW_Window::initialize_time(const double start_time) {
  glfwSetTime(start_time);
  last_time_ = glfwGetTime();
}


double GLFW_Window::get_current_time() const {
  return current_time_;
}


double GLFW_Window::get_delta_time() const {
  return delta_time_;
}


double GLFW_Window::get_fps() const {
  return fps_;
}


bool GLFW_Window::should_close() const {
  return glfwGetKey(glfw_window_, GLFW_KEY_ESCAPE ) == GLFW_PRESS ||
         glfwWindowShouldClose(glfw_window_) == 1 || shouldClose_;
}


void GLFW_Window::set_should_close() {
  shouldClose_ = true;
}
