#pragma once
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Shader
{
    unsigned int id;
};

Shader shader_create(
    const std::string& vertex_shader_path, const std::string& fragment_shader_path);

Shader shader_create(
    const std::string& vertex_shader_path, const std::string& geometry_shader_path,
    const std::string& fragment_shader_path);

void shader_destroy(Shader& shader);

void shader_use(const Shader& shader);

void shader_set_bool(const Shader& shader, const std::string& name, bool value);
void shader_set_int(const Shader& shader, const std::string& name, int value);
void shader_set_float(const Shader& shader, const std::string& name, float value);
void shader_set_mat3(const Shader& shader, const std::string& name, const glm::mat3& mat3);
void shader_set_mat4(const Shader& shader, const std::string& name, const glm::mat4& mat4);
void shader_set_vec3(const Shader& shader, const std::string& name, const glm::vec3 vec3);
void shader_set_vec4(const Shader& shader, const std::string& name, const glm::vec4 vec4);
