#version 430 core
out vec4 frag_color;
in vec2 tex_coords;

uniform sampler2D particle_tex;

void main()
{
    vec4 tex_color = texture(particle_tex, tex_coords);
    frag_color = mix(vec4(tex_color), vec4(1, 1, 1, tex_color.a), 0.2);
}
