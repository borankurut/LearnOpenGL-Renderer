#include "shader.h"
#include <string>
#include <glad/glad.h>
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "utils.h"
#include <cassert>

static GLuint compile_shader(const std::string& source, GLenum type)
{
    GLuint shader_id = glCreateShader(type);

    const char* src = source.c_str();
    glShaderSource(shader_id, 1, &src, NULL);
    glCompileShader(shader_id);

    int success;
    char info_log[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader_id, 512, nullptr, info_log);

        if (type == GL_VERTEX_SHADER)
        {
            std::cout << "ERROR: \nVERTEX SHADER COMPILATION FAILED: \n" << info_log;
        }
        else if (type == GL_FRAGMENT_SHADER)
        {
            std::cout << "ERROR: \nFRAGMENT SHADER COMPILATION FAILED: \n" << info_log;
        }
        else
        {
            std::cout << "ERROR: \nSHADER COMPILATION FAILED: \n" << info_log;
        }
    }

    return shader_id;
}

Shader shader_create(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
    Shader shader;

    // vertex shader
    const std::string vertex_shader_source = file_to_string(vertex_shader_path);
    unsigned int vertex_shader_id = compile_shader(vertex_shader_source.c_str(), GL_VERTEX_SHADER);

    // fragment shader
    const std::string fragment_shader_source = file_to_string(fragment_shader_path);
    unsigned int fragment_shader_id =
        compile_shader(fragment_shader_source.c_str(), GL_FRAGMENT_SHADER);

    shader.id = glCreateProgram();
    glAttachShader(shader.id, vertex_shader_id);
    glAttachShader(shader.id, fragment_shader_id);
    glLinkProgram(shader.id);

    int success;
    glGetProgramiv(shader.id, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shader.id, 512, nullptr, infoLog);
        std::cout << "ERROR: \nSHADER LINKING FAILED: \n" << infoLog;
    }
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return shader;
}

Shader shader_create(
    const std::string& vertex_shader_path, const std::string& geometry_shader_path,
    const std::string& fragment_shader_path)
{
    Shader shader;

    // vertex shader
    const std::string vertex_shader_source = file_to_string(vertex_shader_path);
    unsigned int vertex_shader_id = compile_shader(vertex_shader_source.c_str(), GL_VERTEX_SHADER);

    // geometry shader
    const std::string geometry_shader_source = file_to_string(geometry_shader_path);
    unsigned int geometry_shader_id =
        compile_shader(geometry_shader_source.c_str(), GL_GEOMETRY_SHADER);

    // fragment shader
    const std::string fragment_shader_source = file_to_string(fragment_shader_path);
    unsigned int fragment_shader_id =
        compile_shader(fragment_shader_source.c_str(), GL_FRAGMENT_SHADER);

    shader.id = glCreateProgram();
    glAttachShader(shader.id, vertex_shader_id);
    glAttachShader(shader.id, geometry_shader_id);
    glAttachShader(shader.id, fragment_shader_id);
    glLinkProgram(shader.id);

    int success;
    glGetProgramiv(shader.id, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shader.id, 512, nullptr, infoLog);
        std::cout << "ERROR: \nSHADER LINKING FAILED: \n" << infoLog;
    }
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return shader;
}

void shader_destroy(Shader& shader)
{
    glDeleteProgram(shader.id);
}

void shader_use(const Shader& shader)
{
    glUseProgram(shader.id);
}

void shader_set_bool(const Shader& shader, const std::string& name, bool value)
{
#ifdef DEBUG
    std::cout << "shader << set: " << name << std::endl;
#endif

    int location = glGetUniformLocation(shader.id, name.c_str());
    assert((location != -1) && "gl uniform location is not found");
    glUniform1i(location, (int)value);
}

void shader_set_int(const Shader& shader, const std::string& name, int value)
{
#ifdef DEBUG
    std::cout << "shader << set: " << name << std::endl;
#endif

    int location = glGetUniformLocation(shader.id, name.c_str());
    assert((location != -1) && "gl uniform location is not found");
    glUniform1i(location, value);
}

void shader_set_float(const Shader& shader, const std::string& name, float value)
{
#ifdef DEBUG
    std::cout << "shader << set: " << name << std::endl;
#endif

    int location = glGetUniformLocation(shader.id, name.c_str());
    assert((location != -1) && "gl uniform location is not found");
    glUniform1f(location, value);
}

void shader_set_mat3(const Shader& shader, const std::string& name, const glm::mat3& matrix)
{
#ifdef DEBUG
    std::cout << "shader << set: " << name << std::endl;
#endif

    int location = glGetUniformLocation(shader.id, name.c_str());
    assert((location != -1) && "gl uniform location is not found");
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void shader_set_mat4(const Shader& shader, const std::string& name, const glm::mat4& matrix)
{
#ifdef DEBUG
    std::cout << "shader << set: " << name << std::endl;
#endif

    int location = glGetUniformLocation(shader.id, name.c_str());
    assert((location != -1) && "gl uniform location is not found");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void shader_set_vec3(const Shader& shader, const std::string& name, const glm::vec3 vec3)
{
#ifdef DEBUG
    std::cout << "shader << set: " << name << std::endl;
#endif

    int location = glGetUniformLocation(shader.id, name.c_str());
    assert((location != -1) && "gl uniform location is not found");
    glUniform3fv(location, 1, glm::value_ptr(vec3));
}

void shader_set_vec4(const Shader& shader, const std::string& name, const glm::vec4 vec4)
{
#ifdef DEBUG
    std::cout << "shader << set: " << name << std::endl;
#endif
    int location = glGetUniformLocation(shader.id, name.c_str());
    assert((location != -1) && "gl uniform location is not found");
    glUniform4fv(location, 1, glm::value_ptr(vec4));
}
