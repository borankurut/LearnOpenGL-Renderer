#version 430 core
layout(location = 0) in vec2 a_pos;
layout(location = 1) in vec2 a_tex;

// inst
layout(location = 2) in vec3 instance_pos;
layout(location = 3) in float instance_rot;
layout(location = 4) in float instance_scale;

uniform mat4 proj;
uniform mat4 view;

out vec2 tex_coords;

void main()
{
    tex_coords = a_tex;

    vec3 right = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 up = vec3(0, 1, 0);

    float c = cos(radians(instance_rot));
    float s = sin(radians(instance_rot));
    vec3 rot_right = c * right - s * up;
    vec3 rot_up = s * right + c * up;

    vec3 world_pos = instance_pos +
            (a_pos.x * instance_scale) * rot_right +
            (a_pos.y * instance_scale) * rot_up;

    gl_Position = proj * view * vec4(world_pos, 1.0);
}
