#version 330 core

out vec4 frag_color;

uniform sampler2D renderTexture;
in vec2 tex_coords;

void main() {
    vec2 tex_coords_reverse = vec2(1.0f - tex_coords.x, 1.0f - tex_coords.y);
    frag_color = texture(renderTexture, tex_coords_reverse);
}
