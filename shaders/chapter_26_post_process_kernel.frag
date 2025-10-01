#version 330 core

in vec2 tex_coords;

uniform sampler2D scene_tex;
out vec4 frag_color;

const float offset = 1.0f / 300.0f;

vec2 offsets[9] = vec2[](
        vec2(-offset, offset), // top left
        vec2(0, offset), // top center
        vec2(offset, offset), // top right
        vec2(-offset, 0), // mid left
        vec2(0, 0), // mid center
        vec2(offset, 0), // mid right
        vec2(-offset, -offset), // bottom left
        vec2(0, -offset), // bottom center
        vec2(offset, -offset) // bottom right
    );

float kernel_edge[9] = float[](
        -1.0f, -1.0f, -1.0f, //
        -1.0f, 9.0f, -1.0f, //
        -1.0f, -1.0f, -1.0f //
    );
float kernel_blur[9] = float[](1.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 4.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 1.0 / 16);

void main() {
    vec3 sample_color;
    vec3 result_color = vec3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 9; ++i) {
        sample_color = vec3(texture(scene_tex, tex_coords.st + offsets[i]));
        result_color += sample_color * kernel_edge[i];
    }

    frag_color = vec4(result_color, 1.0f);
}
