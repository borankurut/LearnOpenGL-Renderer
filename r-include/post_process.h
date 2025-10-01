#pragma once
#include <string>
#include "shader.h"
#include <vector>

struct PostProcessPass
{
    unsigned int framebuffer;
    unsigned int color_texture;
    unsigned int depth_renderbuffer;
    unsigned int quad_vao;
    int width;
    int height;
    Shader shader;
};

struct PostProcessPipeline
{
    std::vector<PostProcessPass> passes;
};

PostProcessPass post_process_create(int width, int height, const std::string& fragment_shader_path);
void bind(PostProcessPass& pp_data);
void unbind(PostProcessPass& pp_data);
