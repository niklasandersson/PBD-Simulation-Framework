#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

#include <string>

#include <GL/glew.h>


class OpenGL_Window {

public:
  virtual ~OpenGL_Window() = default;

  virtual void initialize() = 0;
  virtual void cleanup() = 0;
  
  virtual void render() = 0;

  void set_window_width(const unsigned int window_width);
  unsigned int get_window_width() const;

  void set_window_height(const unsigned int window_height);
  unsigned int get_window_height() const;

  void set_window_title(const std::string& window_title);
  std::string get_window_title() const;


protected:
  OpenGL_Window(const unsigned int window_width, const unsigned int window_height, const std::string& window_title);

  mutable unsigned int window_width_;
  mutable unsigned int window_height_;
  mutable std::string window_title_;

private:


};

#endif // OPENGL_WINDOW_H