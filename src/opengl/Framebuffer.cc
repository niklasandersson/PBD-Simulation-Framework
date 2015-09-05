#include "Framebuffer.h"

Framebuffer::Framebuffer(int size_x, int size_y) :
size_x_(size_x), size_y_(size_y){
  glGenFramebuffers(1, &framebuffer_id_);
}

Framebuffer::~Framebuffer() {
  glDeleteFramebuffers(1, &framebuffer_id_);
}

void Framebuffer::attachDepthTarget() {
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id_);
  depth_texture_ = new Texture(size_x_, size_y_, Texture::DEPTH_TEXTURE, GL_DEPTH_COMPONENT24);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture_->getId(), 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::attachRenderTarget(GLenum target, GLenum internal_format) {
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id_);
  color_textures_.push_back(new Texture(size_x_, size_y_, Texture::COLOR_TEXTURE, internal_format));
  glFramebufferTexture(GL_FRAMEBUFFER, target, color_textures_.back()->getId(), 0);
  draw_buffers_.push_back(target);
  glDrawBuffers(draw_buffers_.size(), &draw_buffers_[0]);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::activate() {
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id_);
}
void Framebuffer::deactivate() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
