#version 330

uniform vec4 color_uniform;

out vec4 frag_color;

void main() {

  frag_color = color_uniform;
  
}
