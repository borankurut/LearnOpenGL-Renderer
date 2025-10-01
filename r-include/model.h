#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "texture.h"
#include "shader.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
};

struct Model
{
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;
};

Mesh mesh_create(
    const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
    const std::vector<Texture>& textures);

void mesh_draw(const Mesh& mesh, const Shader& shader);

Model model_create(const std::string& path);

void model_draw(const Model& model, const Shader& shader);
