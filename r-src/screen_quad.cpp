#include "screen_quad.h"
#include <glad/glad.h>

ScreenQuad screen_quad_create()
{
    ScreenQuad quad;

    float quad_vertices[]{
        // pos  // tex
        -1, 1,  0, 1,  //
        -1, -1, 0, 0,  //
        1,  -1, 1, 1,  //

        1,  -1, 1, 0,  //
        1,  1,  1, 1,  //
        -1, 1,  0, 1   //
    };

    glGenVertexArrays(1, &quad.vao);
    glBindVertexArray(quad.vao);

    glGenBuffers(1, &quad.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, quad.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return quad;
}

void screen_quad_destroy(ScreenQuad& quad)
{
    glDeleteVertexArrays(1, &quad.vao);
    glDeleteBuffers(1, &quad.vbo);
    quad.vao = 0;
    quad.vbo = 0;
}
