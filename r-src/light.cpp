#include "light.h"

void light_set_uniform_directional(
    const Shader& shader, const DirectionalLight& light, const std::string& light_name)
{
    shader_set_vec3(shader, light_name + ".direction", light.direction);

    shader_set_vec3(shader, light_name + ".ambient", light.ambient);
    shader_set_vec3(shader, light_name + ".diffuse", light.diffuse);
    shader_set_vec3(shader, light_name + ".specular", light.specular);
}

void light_set_uniform_point(
    const Shader& shader, const PointLight& light, const std::string& light_name)
{
    shader_set_vec3(shader, light_name + ".position", light.position);

    shader_set_vec3(shader, light_name + ".ambient", light.ambient);
    shader_set_vec3(shader, light_name + ".diffuse", light.diffuse);
    shader_set_vec3(shader, light_name + ".specular", light.specular);

    shader_set_float(shader, light_name + ".constant_attenuation", light.constant_attenuation);
    shader_set_float(shader, light_name + ".linear_attenuation", light.linear_attenuation);
    shader_set_float(shader, light_name + ".quadratic_attenuation", light.quadratic_attenuation);
}

void light_set_uniform_spot(
    const Shader& shader, const SpotLight& light, const std::string& light_name)
{
    shader_set_vec3(shader, light_name + ".position", light.position);
    shader_set_vec3(shader, light_name + ".direction", light.direction);

    shader_set_float(shader, light_name + ".inner_cutoff_cos", light.inner_cutoff_cos);
    shader_set_float(shader, light_name + ".outer_cutoff_cos", light.outer_cutoff_cos);

    shader_set_vec3(shader, light_name + ".ambient", light.ambient);
    shader_set_vec3(shader, light_name + ".diffuse", light.diffuse);
    shader_set_vec3(shader, light_name + ".specular", light.specular);

    shader_set_float(shader, light_name + ".constant_attenuation", light.constant_attenuation);
    shader_set_float(shader, light_name + ".linear_attenuation", light.linear_attenuation);
    shader_set_float(shader, light_name + ".quadratic_attenuation", light.quadratic_attenuation);
}
