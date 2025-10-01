#version 330 core

out vec4 frag_color;
uniform samplerCube sampler_skybox;
in vec3 tex_coords;

void main() {
    frag_color = texture(sampler_skybox, tex_coords);
}
