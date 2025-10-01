#version 330 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;

    sampler2D texture_specular1;
    sampler2D texture_specular2;
    sampler2D texture_specular3;

    float shininess;
};

struct DirectionalLight {
    vec3 position;
    vec3 direction;

    float inner_cutoff_cos;
    float outer_cutoff_cos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant_attenuation;
    float linear_attenuation;
    float quadratic_attenuation;
};

struct PointLight {
    vec3 position;
    vec3 direction;

    float inner_cutoff_cos;
    float outer_cutoff_cos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant_attenuation;
    float linear_attenuation;
    float quadratic_attenuation;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    float inner_cutoff_cos;
    float outer_cutoff_cos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant_attenuation;
    float linear_attenuation;
    float quadratic_attenuation;
};

uniform Material material;

uniform DirectionalLight directional_light;
uniform PointLight point_lights[4];
uniform SpotLight spot_light;

uniform vec3 viewer_world_position;

in vec3 fragment_world_position;
in vec2 tex_coords;
in vec3 normal_v;

out vec4 frag_color;

vec3 calculate_spot_light_color(SpotLight light, vec3 view_reflect_dir, vec3 normal);
vec3 calculate_directional_light_color(DirectionalLight light, vec3 view_reflect_dir, vec3 normal);
vec3 calculate_point_light_color(PointLight light, vec3 view_reflect_dir, vec3 normal);

void main() {
    vec3 view_dir = normalize(fragment_world_position - viewer_world_position);
    vec3 normal = normalize(normal_v);
    vec3 view_reflect_dir = reflect(view_dir, normal);

    vec3 result_color = calculate_directional_light_color(directional_light, view_reflect_dir, normal);

    for (int i = 0; i < 4; ++i) {
        result_color += calculate_point_light_color(point_lights[i], view_reflect_dir, normal);
    }

    result_color += calculate_spot_light_color(spot_light, view_reflect_dir, normal);
    frag_color = vec4(result_color, 1.0f);
}

vec3 calculate_spot_light_color(SpotLight light, vec3 view_reflect_dir, vec3 normal) {
    vec3 ambient_color = light.ambient * vec3(texture(material.texture_diffuse1, tex_coords));

    // compute distance
    float dis_light_frag = length(light.position - fragment_world_position);

    float attenuation_component =
        1.0f / (1 + light.constant_attenuation + dis_light_frag * light.linear_attenuation +
                dis_light_frag * dis_light_frag * light.quadratic_attenuation);

    // diffuse
    vec3 to_light_dir = normalize(light.position - fragment_world_position);

    float diffuse_component = max(dot(to_light_dir, normal), 0.0f);
    vec3 diffuse_color = diffuse_component * light.diffuse *
            vec3(texture(material.texture_diffuse1, tex_coords));

    // specular
    float specular_component = max(dot(to_light_dir, view_reflect_dir), 0.0f);
    specular_component = pow(specular_component, material.shininess);
    vec3 specular_color = specular_component * light.specular *
            vec3(texture(material.texture_specular1, tex_coords));

    // compute spot component
    float spot_light_direction_alignment = dot(-normalize(light.direction), to_light_dir);

    float spot_light_component = clamp((spot_light_direction_alignment - light.outer_cutoff_cos) / (light.inner_cutoff_cos - light.outer_cutoff_cos), 0.0f, 1.0f);

    float spot_comp_atten_comp = spot_light_component * attenuation_component;

    vec3 result_color = spot_comp_atten_comp * (ambient_color + diffuse_color + specular_color);

    return result_color;
}

vec3 calculate_point_light_color(PointLight light, vec3 view_reflect_dir, vec3 normal) {
    vec3 ambient_color = light.ambient * vec3(texture(material.texture_diffuse1, tex_coords));

    // compute distance
    float dis_light_frag = length(light.position - fragment_world_position);

    float attenuation_component =
        1.0f / (1 + light.constant_attenuation + dis_light_frag * light.linear_attenuation +
                dis_light_frag * dis_light_frag * light.quadratic_attenuation);

    // diffuse
    vec3 to_light_dir = normalize(light.position - fragment_world_position);

    float diffuse_component = max(dot(to_light_dir, normal), 0.0f);
    vec3 diffuse_color = diffuse_component * light.diffuse *
            vec3(texture(material.texture_diffuse1, tex_coords));

    // specular
    float specular_component = max(dot(to_light_dir, view_reflect_dir), 0.0f);
    specular_component = pow(specular_component, material.shininess);
    vec3 specular_color = specular_component * light.specular *
            vec3(texture(material.texture_specular1, tex_coords));

    vec3 result_color = attenuation_component * (ambient_color + diffuse_color + specular_color);

    return result_color;
}

vec3 calculate_directional_light_color(DirectionalLight light, vec3 view_reflect_dir, vec3 normal) {
    // ambient
    vec3 ambient_color = light.ambient * vec3(texture(material.texture_diffuse1, tex_coords));

    // diffuse
    vec3 to_light_dir = normalize(-light.direction);

    float diffuse_component = max(dot(to_light_dir, normal), 0.0f);
    vec3 diffuse_color = diffuse_component * light.diffuse * vec3(texture(material.texture_diffuse1, tex_coords));

    // specular
    float specular_component = max(dot(to_light_dir, view_reflect_dir), 0.0f);
    specular_component = pow(specular_component, material.shininess);
    vec3 specular_color = specular_component * light.specular * vec3(texture(material.texture_specular1, tex_coords));

    vec3 result_color = ambient_color + diffuse_color + specular_color;

    return result_color;
}
