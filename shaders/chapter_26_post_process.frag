#version 330 core

in vec2 tex_coords;

uniform sampler2D scene_tex;
out vec4 frag_color;

void main() {
    vec4 original_color = texture(scene_tex, tex_coords);

    frag_color = vec4((vec3(1.0f) - original_color.rgb), original_color.a);
}
