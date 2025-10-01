#pragma once
#include <glm/glm.hpp>
#include <string.h>
#include "shader.h"

struct DirectionalLight
{
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct PointLight
{
    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant_attenuation;
    float linear_attenuation;
    float quadratic_attenuation;
};

struct SpotLight
{
    glm::vec3 position;
    glm::vec3 direction;

    float inner_cutoff_cos;
    float outer_cutoff_cos;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant_attenuation;
    float linear_attenuation;
    float quadratic_attenuation;
};

void light_set_uniform_directional(
    const Shader& shader, const DirectionalLight& light, const std::string& light_name);

void light_set_uniform_point(
    const Shader& shader, const PointLight& light, const std::string& light_name);

void light_set_uniform_spot(
    const Shader& shader, const SpotLight& light, const std::string& light_name);
