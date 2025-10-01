#version 330 core

in vec4 color_from_vertex_shader;
out vec4 frag_color;

void main() {
    frag_color = color_from_vertex_shader;
}
