#ifndef SCENE_H
#define SCENE_H

#include <array>
#include <mutex>

#include "opengl/GL_Renderable.h"

#include "console/Console.h"


class Scene : public GL_Renderable {

public:
  Scene() : GL_Renderable("program_quad") {

    Console::getInstance()->add("color", this, &Scene::setColor);

    generateQuad();

    add_vao("quad_vao");
    add_buffer("quad_vertices");
    add_buffer("element_buffer");
    generate_vaos_and_buffers();

    add_uniform("color_uniform");

    bindVertexArray("quad_vao");

    bindBuffer("quad_vertices");
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * 3 * sizeof(float), &vertices_[0][0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    bindBuffer("element_buffer", GL_ELEMENT_ARRAY_BUFFER);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned short), &indices_[0], GL_STATIC_DRAW);

    unBindVertexArray();
  }

  ~Scene() {

  }

  void render() override {

    // glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glUseProgram(program_);

    color_mutex_.lock();
    glUniform4f(get_uniform("color_uniform"), color_[0], color_[1], color_[2], color_[3]);
    color_mutex_.unlock();

    bindVertexArray("quad_vao");

    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_SHORT, nullptr);

    unBindVertexArray();

  }

  void setColor(float r, float g, float b, float a) {
    std::lock_guard<std::mutex> lock(color_mutex_);
    color_[0] = r;
    color_[1] = g;
    color_[2] = b;
    color_[3] = a;
  }

protected:

private:
  std::vector<std::array<float, 3> > vertices_;
  std::vector<unsigned short> indices_;

  std::mutex color_mutex_;
  std::array<float, 4> color_ = std::array<float, 4>{1.0f, 0.0f, 0.0f, 1.0f};

  void generateQuad() {
    vertices_.push_back({-1.0f, -1.0f, 0.0f});
    vertices_.push_back({1.0f, 1.0f, 0.0f});
    vertices_.push_back({-1.0f, 1.0f, 0.0f});
    vertices_.push_back({1.0f, -1.0f, 0.0f});

    indices_.push_back((unsigned short)0);
    indices_.push_back((unsigned short)1);
    indices_.push_back((unsigned short)2);
    indices_.push_back((unsigned short)0);
    indices_.push_back((unsigned short)3);
    indices_.push_back((unsigned short)1);
  }


};

#endif // SCENE_H