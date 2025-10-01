#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal_matrix;

out VS_OUT {
    vec3 normal_v;
    vec3 fragment_world_position;
    vec2 tex_coords;
} vs_out;

void main()
{
    vec4 v_pos = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0f);
    gl_Position = projection * view * model * v_pos;
    vs_out.normal_v = normalize(normal_matrix * a_normal);
    vs_out.fragment_world_position = vec3(model * v_pos);
    vs_out.tex_coords = a_tex_coords;
}
