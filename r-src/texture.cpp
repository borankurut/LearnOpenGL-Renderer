#include "texture.h"
#include <glad/glad.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

ImageData image_create(const std::string& path)
{
    ImageData image_data;

    image_data.data =
        stbi_load(path.c_str(), &image_data.width, &image_data.height, &image_data.nr_channels, 0);

    if (!image_data.data)
    {
        std::cout << "CAN'T LOAD IMAGE: " << path << std::endl;
    }

    return image_data;
}

void image_destroy(ImageData& image_data)
{
    stbi_image_free((void*)image_data.data);
}

Texture texture_create(const std::string& path)
{
    stbi_set_flip_vertically_on_load(true);

    Texture texture;
    texture.id = 0;
    texture.path = path;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    ImageData image_data = image_create(path);
    texture.width = image_data.width;
    texture.height = image_data.height;
    texture.nr_channels = image_data.nr_channels;

    if (image_data.data)
    {
        if (texture.nr_channels == 1)
        {
            texture.internal_format = GL_RED;
            texture.format = GL_RED;
        }
        else if (texture.nr_channels == 2)
        {
            texture.internal_format = GL_RG;
            texture.format = GL_RG;
        }
        else if (texture.nr_channels == 3)
        {
            texture.internal_format = GL_SRGB;
            texture.format = GL_RGB;
        }
        else if (texture.nr_channels == 4)
        {
            texture.internal_format = GL_SRGB_ALPHA;
            texture.format = GL_RGBA;
        }

        glTexImage2D(
            GL_TEXTURE_2D, 0, texture.internal_format, texture.width, texture.height, 0,
            texture.format, GL_UNSIGNED_BYTE, image_data.data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "unable to load texture: " << path << "\n";

        glDeleteTextures(1, &texture.id);
        texture.id = 0;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    image_destroy(image_data);

    return texture;
}

void texture_bind(const Texture& texture)
{
    glBindTexture(GL_TEXTURE_2D, texture.id);
}

void texture_destroy(Texture& texture)
{
    glDeleteTextures(1, &texture.id);
    texture.id = 0;
}

Cubemap cubemap_create(std::vector<std::string> faces)
{
    assert(faces.size() == 6);

    stbi_set_flip_vertically_on_load(false);

    Cubemap cubemap;
    glGenTextures(1, &cubemap.id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.id);

    for (int i = 0; i < 6; ++i)
    {
        ImageData image = image_create(faces[i]);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, image.width, image.height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, image.data);

        if (!image.data)
        {
            std::cout << "CUBEMAP FAILED TO LOAD: " << faces[i] << std::endl;
            cubemap_destroy(cubemap);
            break;
        }

        image_destroy(image);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return cubemap;
}

void cubemap_bind(const Cubemap& cubemap)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.id);
}

void cubemap_destroy(Cubemap& cubemap)
{
    glDeleteTextures(1, &cubemap.id);
    cubemap.id = 0;
}
