#version 330

layout(location = 0) in vec3 vertex_position;

void main() {
  
  gl_Position.xyz = vertex_position;
  gl_Position.w = 1.0f;

}
