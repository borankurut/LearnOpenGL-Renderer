#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec2 a_tex_coords;

out vec3 color_from_vertex_shader;
out vec2 tex_coords_from_vertex_shader;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    color_from_vertex_shader = a_color;
    tex_coords_from_vertex_shader = a_tex_coords;

    gl_Position = projection * view * model * vec4(a_pos.x, a_pos.y, a_pos.z, 1.0f);
}
