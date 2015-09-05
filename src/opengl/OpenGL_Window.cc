#include "opengl/OpenGL_Window.h"


OpenGL_Window::OpenGL_Window(const unsigned int window_width, const unsigned int window_height, const std::string& window_title) 
: window_width_{window_width}, window_height_{window_height}, window_title_{window_title} {
  glViewport(0, 0, window_width_, window_height_);
}


void OpenGL_Window::set_window_width(const unsigned int window_width) {
  window_width_ = window_width;
}


unsigned int OpenGL_Window::get_window_width() const {
  return window_width_;
}


void OpenGL_Window::set_window_height(const unsigned int window_height) {
  window_height_ = window_height;
}


unsigned int OpenGL_Window::get_window_height() const {
  return window_height_;
}


void OpenGL_Window::set_window_title(const std::string& window_title) {
  window_title_ = window_title;
}


std::string OpenGL_Window::get_window_title() const {
  return window_title_;
}