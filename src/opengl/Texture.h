#ifndef TEXTURE_HPP
#define WINDOW_HPP

#include <GL/glew.h>

class Texture {
public:
  enum TextureType {
    COLOR_TEXTURE,
    DEPTH_TEXTURE
  };

  Texture(int size_x, int size_y, TextureType type, GLenum internal_format);
  ~Texture();

  GLuint getId() {return texture_id_;}

  void activateOn(int texture_unit) const;

private:
  GLuint texture_id_;
};

#endif
