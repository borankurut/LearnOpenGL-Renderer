#include "render_target.h"
#include <glad/glad.h>
#include <iostream>

RenderTarget render_target_make(int width, int height)
{
    RenderTarget target;
    target.width = width;
    target.height = height;

    // fbo
    glGenFramebuffers(1, &target.framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, target.framebuffer);

    // texture
    glGenTextures(1, &target.color_texture);
    glBindTexture(GL_TEXTURE_2D, target.color_texture);

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, target.width, target.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, target.color_texture, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    // rbo
    glGenRenderbuffers(1, &target.depth_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, target.depth_rbo);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, target.width, target.height);
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, target.depth_rbo);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::RENDER_TARGET:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return target;
}

void render_target_bind(const RenderTarget& target)
{
    glBindFramebuffer(GL_FRAMEBUFFER, target.framebuffer);
    glViewport(0, 0, target.width, target.height);
}

void render_target_unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
