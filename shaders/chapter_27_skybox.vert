#version 330 core
layout(location = 0) in vec3 a_pos;

uniform mat4 projection;
uniform mat4 view;

out vec3 tex_coords;

void main()
{
    tex_coords = a_pos;
    vec4 pos = projection * mat4(mat3(view)) * vec4(a_pos, 1.0f);
    gl_Position = pos.xyww; // sets z to 1.0f.
}
