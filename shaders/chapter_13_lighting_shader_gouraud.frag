#version 330 core

out vec4 frag_color;

in vec3 frag_color_from_vertex_shader;

void main() {
    frag_color = vec4(frag_color_from_vertex_shader, 1.0f);
}
