#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;

    float shininess;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant_attenuation;
    float linear_attenuation;
    float quadratic_attenuation;
};

uniform Material material;
uniform PointLight light;

uniform vec3 viewer_world_position;

in vec3 fragment_world_position;
in vec2 tex_coords;
in vec3 normal_v;

out vec4 frag_color;

void main() {
    normal = normalize(normal_v);
    // ambient
    vec3 ambient_color = light.ambient * vec3(texture(material.diffuse, tex_coords));

    // compute distance
    float dis_light_frag = length(light.position - fragment_world_position);

    float attenuation_component =
        1.0f / (1 + light.constant_attenuation + dis_light_frag * light.linear_attenuation +
                dis_light_frag * dis_light_frag * light.quadratic_attenuation);

    // diffuse
    vec3 to_light_dir = normalize(light.position - fragment_world_position);

    float diffuse_component = max(dot(to_light_dir, normal), 0.0f);
    vec3 diffuse_color = diffuse_component * light.diffuse *
            vec3(texture(material.diffuse, tex_coords));

    // specular
    vec3 view_dir = normalize(fragment_world_position - viewer_world_position);
    vec3 reflect_dir = reflect(view_dir, normal);
    float specular_component = max(dot(to_light_dir, reflect_dir), 0.0f);
    specular_component = pow(specular_component, material.shininess);
    vec3 specular_color = specular_component * light.specular *
            vec3(texture(material.specular, tex_coords));

    vec3 result_color = ambient_color + attenuation_component * diffuse_color + attenuation_component * specular_color;

    frag_color = vec4(result_color, 1.0f);
}
