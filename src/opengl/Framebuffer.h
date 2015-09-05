#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include<vector>

#include <GL/glew.h>

#include "Texture.h"

class Framebuffer {
public:
  Framebuffer(int size_x, int size_y);
  ~Framebuffer();

  void activate();
  void deactivate();

  void attachDepthTarget();
  void attachRenderTarget(GLenum target, GLenum internal_format);

  const Texture* getColorTexture(int i) {return color_textures_[i];}
  const Texture* getDepthTexture() {return depth_texture_;}

private:
  GLuint framebuffer_id_;
  int size_x_;
  int size_y_;

  std::vector<Texture*> color_textures_;
  Texture* depth_texture_;

  std::vector<GLenum> draw_buffers_;
};
#endif
