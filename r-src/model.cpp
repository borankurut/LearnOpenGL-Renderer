#include "model.h"
#include <cstddef>
#include <string>
#include "assimp/material.h"
#include "assimp/mesh.h"
#include "shader.h"
#include "texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

static void setup_mesh(Mesh& mesh);
static void load_model(Model& model, const std::string& path);
static void process_node(Model& model, aiNode* node, const aiScene* scene);
static Mesh process_mesh(Model& model, aiMesh* mesh, const aiScene* scene);
static std::vector<Texture> load_material_textures(
    Model& model, aiMaterial* mat, aiTextureType type, std::string type_name);

Mesh mesh_create(
    const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
    const std::vector<Texture>& textures)
{
    Mesh mesh;
    mesh.vertices = vertices;
    mesh.indices = indices;
    mesh.textures = textures;

    setup_mesh(mesh);
    return mesh;
}

void mesh_draw(const Mesh& mesh, const Shader& shader)
{
    unsigned int diffuse_counter = 1;
    unsigned int specular_counter = 1;

    for (unsigned int i = 0; i < mesh.textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string number;
        std::string name = mesh.textures[i].type;

        if (name == "texture_diffuse")
        {
            number = std::to_string(diffuse_counter++);
        }
        else if (name == "texture_specular")
        {
            number = std::to_string(specular_counter++);
        }

        shader_set_int(shader, "material." + name + number, i);
        texture_bind(mesh.textures[i]);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
}

static void setup_mesh(Mesh& mesh)
{
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);

    glBindVertexArray(mesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(
        GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0],
        GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Model model_create(const std::string& path)
{
    Model model;
    load_model(model, path);
    return model;
}

void model_draw(const Model& model, const Shader& shader)
{
    for (int i = 0; i < model.meshes.size(); ++i)
    {
        mesh_draw(model.meshes[i], shader);
    }
}

static void load_model(Model& model, const std::string& path)
{
    Assimp::Importer import;

    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    model.directory = path.substr(0, path.find_last_of('/'));
    process_node(model, scene->mRootNode, scene);
}

static void process_node(Model& model, aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        model.meshes.push_back(process_mesh(model, mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        process_node(model, node->mChildren[i], scene);
    }
}

static Mesh process_mesh(Model& model, aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // proccess vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        if (mesh->mTextureCoords[0])  // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.tex_coords = vec;
        }
        else
        {
            vertex.tex_coords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps =
            load_material_textures(model, material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps =
            load_material_textures(model, material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    return mesh_create(vertices, indices, textures);
}

static std::vector<Texture> load_material_textures(
    Model& model, aiMaterial* mat, aiTextureType type, std::string type_name)
{
    std::vector<Texture> textures;
    int texCount = mat->GetTextureCount(type);
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (int j = 0; j < model.textures_loaded.size(); j++)
        {
            if (std::strcmp(
                    model.textures_loaded[j].path.data(),
                    (std::string(model.directory + "/" + str.C_Str())).c_str()) == 0)
            {
                textures.push_back(model.textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            // if texture hasn’t been loaded already, load it
            Texture texture = texture_create(model.directory + "/" + str.C_Str());
            texture.type = type_name;
            if (texture.id != 0)
            {
                textures.push_back(texture);
                model.textures_loaded.push_back(texture);
            }
            // add to loaded textures
        }
    }
    return textures;
}
