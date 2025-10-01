#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal_matrix;

uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_world_position;
uniform vec3 viewer_world_position;
uniform float specular_power;

out vec3 frag_color_from_vertex_shader;

void main()
{
    vec4 v_pos = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0f);
    gl_Position = projection * view * model * v_pos;
    vec3 fragment_world_position = vec3(model * v_pos);
    vec3 normal = normalize(normal_matrix * a_normal);

    float ambient_component = 0.1;
    vec3 ambient_color = ambient_component * light_color;

    // diffuse
    vec3 to_light_dir = normalize(light_world_position - fragment_world_position);

    float diffuse_component = max(dot(to_light_dir, normal), 0.0f);
    vec3 diffuse_color = diffuse_component * light_color;

    // specular
    vec3 view_dir = normalize(fragment_world_position - viewer_world_position);
    vec3 reflect_dir = reflect(view_dir, normal);
    float specular_component = max(dot(to_light_dir, reflect_dir), 0.0f);
    specular_component = pow(specular_component, specular_power);
    vec3 specular_color = specular_component * light_color;

    frag_color_from_vertex_shader = (ambient_color + diffuse_color + specular_color) * object_color;
}
