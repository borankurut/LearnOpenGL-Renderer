#pragma once

#include <glad/glad.h>
#include <iostream>
#include <vector>

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
    int width;
    int height;
    int nr_channels;
    GLint internal_format;
    GLenum format;
    char* data;
};

struct ImageData
{
    const unsigned char* data;
    int width;
    int height;
    int nr_channels;
};

struct Cubemap
{
    unsigned int id;
};

ImageData image_create(const std::string& path);
void image_destroy(ImageData& image_data);

Texture texture_create(const std::string& path);
void texture_bind(const Texture& texture);
void texture_destroy(Texture& texture);

Cubemap cubemap_create(std::vector<std::string> faces);
void cubemap_bind(const Cubemap& cubemap);
void cubemap_destroy(Cubemap& cubemap);
