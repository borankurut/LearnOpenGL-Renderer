#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "texture.h"
#include <glm/glm.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

// cube
float g_vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
    1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  -0.5f,
    -0.5f, -0.5f, 0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  1.0f,
    0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  -0.5f, 0.5f,
    0.5f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  1.0f,  0.0f,
    -0.5f, 0.5f,  -0.5f, 1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  -0.5f, -0.5f, -0.5f,
    0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  1.0f,  0.0f,  0.5f,
    0.5f,  0.5f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 0.0f,
    1.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  0.5f,  0.5f,
    0.5f,  1.0f,  0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  1.0f,
    0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
    0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.5f,
    0.5f,  -0.5f, 1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,
    0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f};

glm::vec3 cube_positions[] = {glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
                              glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                              glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
                              glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                              glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

float camera_pos_x = 0.0f;
float camera_pos_y = 0.0f;
float camera_pos_z = -3.0f;

float blend_value = 0.5f;

int main()
{
    // init glfw window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "chapter_09_1", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader shader_program = shader_create("shaders/chapter_09_1.vert", "shaders/chapter_09_1.frag");

    Texture texture_container = texture_create("textures/container.jpg");

    Texture texture_smiley = texture_create("textures/smiley.png");

    shader_use(shader_program);
    shader_set_int(shader_program, "texture0", 0);
    shader_set_int(shader_program, "texture1", 1);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertices), g_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        process_input(window);

        // rendering..
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(-camera_pos_x, -camera_pos_y, camera_pos_z));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        /* glm::mat4 projection = glm::mat4(1.0f); */

        shader_use(shader_program);

        shader_set_mat4(shader_program, "view", view);
        shader_set_mat4(shader_program, "projection", projection);
        shader_set_float(shader_program, "blend_value", blend_value);

        glActiveTexture(GL_TEXTURE0);
        texture_bind(texture_container);

        glActiveTexture(GL_TEXTURE1);
        texture_bind(texture_smiley);

        for (int i = 0; i < 10; ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);

            model = glm::translate(model, cube_positions[i]);

            model = glm::rotate(
                model, glm::radians(80.0f * (float)glfwGetTime()),
                glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));

            shader_set_mat4(shader_program, "model", model);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // check events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    shader_destroy(shader_program);

    texture_destroy(texture_container);
    texture_destroy(texture_smiley);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && blend_value < 1.0f)
    {
        blend_value += 0.001f;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && blend_value > 0.0f)
    {
        blend_value -= 0.001f;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera_pos_y += 0.005f;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera_pos_y -= 0.005f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera_pos_x -= 0.005f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera_pos_x += 0.005f;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera_pos_z += 0.005f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera_pos_z -= 0.005f;
    }
}
