#version 330 core
layout(location = 0) in vec3 a_pos;

out vec4 color_from_vertex_shader;

void main()
{
    color_from_vertex_shader = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0f);
}
