#pragma once

struct RenderTarget
{
    unsigned int framebuffer;
    unsigned int color_texture;
    unsigned int depth_rbo;
    int width;
    int height;
};
