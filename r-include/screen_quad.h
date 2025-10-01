#pragma once

struct ScreenQuad
{
    unsigned int vao;
    unsigned int vbo;
};

ScreenQuad screen_quad_create();
void screen_quad_destroy();
