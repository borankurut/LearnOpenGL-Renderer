#version 330 core

in vec2 tex_coords_from_vertex_shader;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float blend_value;

out vec4 frag_color;

void main() {
    frag_color = mix(texture(texture0, tex_coords_from_vertex_shader), texture(texture1, tex_coords_from_vertex_shader), clamp(blend_value, 0.0f, 1.0f));
}
