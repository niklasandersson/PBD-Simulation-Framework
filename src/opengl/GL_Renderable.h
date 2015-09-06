#ifndef GL_RENDERABLE_H
#define GL_RENDERABLE_H

#include <map>
#include <vector>
#include <algorithm>
#include <string>

#include <GL/glew.h>

#include "exception/Error.h"

#include "opengl/OpenGL_Loader.h"

class GL_Renderable {

public:
  GL_Renderable(const std::string& program_name) {
    program_ = OpenGL_Loader::getInstance().accessProgram(program_name);
  }

  virtual ~GL_Renderable() = default;

  virtual void render() = 0;

protected:
  GLuint program_;

  std::vector<GLuint> buffers_vec_;
  std::vector<GLuint> vaos_vec_;
  std::vector<GLuint> gluints_vec_;

  void generate_vaos_and_buffers() {
    if( vaos_vec_.size() > 0 ) {
      glGenVertexArrays(vaos_vec_.size(), &vaos_vec_[0]);
    }

    if( buffers_vec_.size() > 0 ) {
      glGenBuffers(buffers_vec_.size(), &buffers_vec_[0]);
    }
  }

  void add_buffer(const std::string& buffer_name) {
    add_impl(buffers_map_, buffers_vec_, buffer_name);
  }

  void add_vao(const std::string& vao_name) {
    add_impl(vaos_map_, vaos_vec_, vao_name);
  }

  void add_uniform(const std::string& uniform_name) {
    if( uniforms_map_.find(uniform_name) != uniforms_map_.end() ) {
      throw std::invalid_argument{ report_error("The map does already contain the key '" << uniform_name << "'") };
    }
    uniforms_map_[uniform_name] = glGetUniformLocation(program_, uniform_name.c_str());
  }

  void add_gluint(const std::string& gluint_name) {
    add_impl(gluints_map_, gluints_vec_, gluint_name);
  }

  GLuint get_buffer(const std::string& buffer_name) {
    return buffers_vec_[get_impl(buffers_map_, buffer_name)];
  }

  GLuint get_vao(const std::string& vao_name) {
    return vaos_vec_[get_impl(vaos_map_, vao_name)];
  }

  GLuint get_uniform(const std::string& uniform_name) {
    return get_impl(uniforms_map_, uniform_name);
  }

  GLuint get_gluint(const std::string& gluint_name) {
    return gluints_vec_[get_impl(gluints_map_, gluint_name)];
  }

  void bindVertexArray(const std::string& vao_name) {
    glBindVertexArray(get_vao(vao_name));
  }

  void unBindVertexArray() {
    glBindVertexArray(0);
  }

  void bindBuffer(const std::string& buffer_name, GLuint type = GL_ARRAY_BUFFER) {
    glBindBuffer(type, get_buffer(buffer_name));
  }


private:

  void add_impl(std::map<std::string, unsigned int>& map, std::vector<GLuint>& vec, const std::string& key) {
    if( map.find(key) != map.end() ) {
      throw std::invalid_argument{ report_error("The map does already contain the key '" << key << "'") };
    }

    map[key] = vec.size(); 
    vec.resize(vec.size() + 1);
  }

  GLuint get_impl(std::map<std::string, unsigned int>& map, const std::string& key) {
    auto ptr = map.find(key);

    if( ptr == map.end() ) {
      throw std::invalid_argument{ report_error("The map does not contain the key '" << key << "'") };
    } 

    return ptr->second;
  }

  std::map<std::string, unsigned int> buffers_map_;
  std::map<std::string, unsigned int> vaos_map_;
  std::map<std::string, unsigned int> gluints_map_;

  std::map<std::string, GLuint> uniforms_map_;

};


#endif // GL_RENDERABLE_H