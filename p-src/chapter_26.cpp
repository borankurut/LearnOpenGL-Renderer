#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/geometric.hpp"
#include "model.h"
#include "shader.h"
#include "camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"
#include "light.h"
#include "models/cube/cube.h"
#include "models/plane/plane.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <string>

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double x_pos_in, double y_pos_in);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
void draw_scene(const Camera& camera);

Camera camera = camera_create(glm::vec3(0.0f, 0.0f, 3.0f));
float delta_time = 0.0f;
float last_time = 0.0f;

bool first_mouse = true;
float last_x = SCR_WIDTH / 2.0f;
float last_y = SCR_HEIGHT / 2.0f;

Shader shader_program;
Shader shader_program_post_process;
unsigned int VAO_cube;
unsigned int VBO_cube;
unsigned int VAO_plane;
unsigned int VBO_plane;

glm::vec3 mirror_view_pos_debug;

int main()
{
    // init glfw window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "chapter_21", nullptr, nullptr);
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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSwapInterval(0);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    shader_program = shader_create("shaders/chapter_22.vert", "shaders/chapter_26.frag");
    shader_program_post_process =
        shader_create("shaders/chapter_22.vert", "shaders/chapter_26_texture.frag");

    glGenVertexArrays(1, &VAO_cube);

    glBindVertexArray(VAO_cube);

    glGenBuffers(1, &VBO_cube);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_cube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &VAO_plane);

    glBindVertexArray(VAO_plane);

    glGenBuffers(1, &VBO_plane);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_plane);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // generate framebuffer
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    unsigned int texture_mirror;
    glGenTextures(1, &texture_mirror);
    glBindTexture(GL_TEXTURE_2D, texture_mirror);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_mirror, 0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        float current_time = static_cast<float>(glfwGetTime());
        delta_time = current_time - last_time;
        last_time = current_time;

        glm::vec3 plane_pos = glm::vec3(15.0f, 0.0f, -5.0f);
        float plane_rotation_by_y = -180.0f;

        glm::vec3 mirror_to_cam = camera.position - plane_pos;
        glm::vec3 mirror_normal = glm::vec3(-1.0f, 0.0f, 0.0f);

        glm::vec3 mirror_viewer_pos =
            camera.position - (2 * glm::dot(mirror_normal, mirror_to_cam)) * mirror_normal;

        mirror_view_pos_debug = mirror_viewer_pos;

        glm::vec3 mirror_viewer_dir = glm::normalize(plane_pos - mirror_viewer_pos);

        Camera mirror_viewer =
            camera_create(mirror_viewer_pos, glm::vec3(0.0f, 1.0f, 0.0f), mirror_viewer_dir);

        // input
        process_input(window);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        // rendering..
        glClearColor(0.1f, 0.01f, 0.01f, 1.0f);
        draw_scene(mirror_viewer);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        draw_scene(camera);

        glm::mat4 model_plane = glm::mat4(1.0f);

        model_plane = glm::translate(model_plane, plane_pos);
        model_plane =
            glm::rotate(model_plane, plane_rotation_by_y + 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        model_plane = glm::scale(model_plane, glm::vec3(5.0f, 5.0f, 5.0f));

        shader_use(shader_program_post_process);

        glm::mat4 view = camera_get_view_matrix(camera);
        glm::mat4 projection = glm::perspective(
            glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        shader_set_mat4(shader_program, "view", view);
        shader_set_mat4(shader_program, "projection", projection);
        shader_set_mat4(shader_program, "model", model_plane);
        glBindTexture(GL_TEXTURE_2D, texture_mirror);

        glBindVertexArray(VAO_plane);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // check events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    shader_destroy(shader_program);
    glDeleteFramebuffers(1, &fbo);

    glfwTerminate();
    return 0;
}

void draw_scene(const Camera& camera)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = camera_get_view_matrix(camera);
    glm::mat4 projection = glm::perspective(
        glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    shader_use(shader_program);

    shader_set_mat4(shader_program, "view", view);
    shader_set_mat4(shader_program, "projection", projection);

    /* cube_positions[0] = mirror_view_pos_debug; */

    for (int i = 0; i < 10; ++i)
    {
        glm::mat4 model_cube = glm::mat4(1.0f);
        model_cube = glm::translate(model_cube, cube_positions[i]);
        shader_set_mat4(shader_program, "model", model_cube);

        glBindVertexArray(VAO_cube);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
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

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera_process_keyboard(camera, CAMERA_FORWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera_process_keyboard(camera, CAMERA_BACKWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera_process_keyboard(camera, CAMERA_LEFT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera_process_keyboard(camera, CAMERA_RIGHT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera_process_keyboard(camera, CAMERA_UP, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera_process_keyboard(camera, CAMERA_DOWN, delta_time);
    }
}

void mouse_callback(GLFWwindow* window, double x_pos_in, double y_pos_in)
{
    float x_pos = static_cast<float>(x_pos_in);
    float y_pos = static_cast<float>(y_pos_in);

    if (first_mouse)
    {
        last_x = x_pos;
        last_y = y_pos;
        first_mouse = false;
    }

    float x_offset = x_pos - last_x;
    float y_offset = last_y - y_pos;  // reversed since y-coordinates go from bottom to top

    last_x = x_pos;
    last_y = y_pos;

    camera_process_mouse_movement(camera, x_offset, y_offset);
}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
    camera_process_mouse_scroll(camera, static_cast<float>(y_offset));
}
