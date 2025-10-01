#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal_matrix;
uniform vec3 light_world_position;

out vec3 normal_v;
out vec3 fragment_view_position;
out vec3 light_view_position; // doing this maybe sucks but maybe better than cpu?

void main()
{
    vec4 v_pos = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0f);
    gl_Position = projection * view * model * v_pos;
    fragment_view_position = vec3(view * model * v_pos);
    normal_v = normalize(normal_matrix * a_normal);
    light_view_position = vec3(view * vec4(light_world_position, 1.0f));
}
