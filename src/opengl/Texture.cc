#include "Texture.h"

Texture::Texture(int size_x, int size_y, TextureType type, GLenum internal_format) {
  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  if (type == COLOR_TEXTURE) {
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, size_x, size_y, 0,GL_RGBA, GL_FLOAT, 0);

  }else if (type == DEPTH_TEXTURE) {
    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, size_x, size_y, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
  }
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
  glDeleteTextures(1, &texture_id_);
}

void Texture::activateOn(int texture_unit) const {
  glActiveTexture(texture_unit);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
}
