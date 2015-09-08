#version 330

layout(location = 0) in vec3 vertex_position;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main() {
  
  gl_Position = projection_matrix * view_matrix * vec4(vertex_position, 1.0);
  
  // gl_Position.xyz = vertex_position;
  // gl_Position.w = 1.0f;

}
