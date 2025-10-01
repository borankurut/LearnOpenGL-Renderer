#version 330 core

out vec4 frag_color;

uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_world_position;
uniform vec3 viewer_world_position;
uniform float specular_power;

in vec3 fragment_world_position;
in vec3 normal_v;

void main() {
    vec3 normal = normalize(normal_v);

    // ambient
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

    vec3 result_color = (ambient_color + diffuse_color + specular_color) * object_color;

    frag_color = vec4(result_color, 1.0f);
}
