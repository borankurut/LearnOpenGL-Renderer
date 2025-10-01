#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"
#include <glm/glm.hpp>

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float shininess;
};

struct Light
{
    glm::vec3 world_position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

void set_directional_light_uniform(const Shader& shader, const Light& light);
void set_material_uniform(const Shader& shader, const Material& material);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double x_pos_in, double y_pos_in);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

Camera camera = camera_create(glm::vec3(0.0f, 0.0f, 3.0f));
float delta_time = 0.0f;
float last_time = 0.0f;

bool first_mouse = true;
float last_x = SCR_WIDTH / 2.0f;
float last_y = SCR_HEIGHT / 2.0f;

// cube
float g_vertices[] = {
    // positions		 // normals		      // texture coords
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,  // 1
    0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f,  // 2
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,  // 3
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,  // 4
    -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f,  // 5
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,  // 6
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,  // 7
    0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,  // 8
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,  // 9
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,  // 10
    -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,  // 11
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,  // 12
    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,  // 13
    -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 1.0f,  // 14
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,  // 15
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,  // 16
    -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 0.0f,  // 17
    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,  // 18
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  // 19
    0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f,  // 20
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,  // 21
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,  // 22
    0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,  // 23
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  // 24
    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f,  // 25
    0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f, 1.0f,  // 26
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f,  // 27
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f,  // 28
    -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f, 0.0f,  // 29
    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f,  // 30
    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f,  // 31
    0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f,  // 32
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,  // 33
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,  // 34
    -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,  // 35
    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f   // 36
};

int main()
{
    // init glfw window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "chapter_14", nullptr, nullptr);
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

    Shader light_cube_shader = shader_create(
        "shaders/chapter_12_light_cube_shader.vert", "shaders/chapter_14_light_cube_shader.frag");

    Shader lighting_shader = shader_create(
        "shaders/chapter_14_lighting_shader.vert", "shaders/chapter_14_lighting_shader.frag");

    unsigned int VAO_container;
    glGenVertexArrays(1, &VAO_container);

    unsigned int VAO_light_cube;
    glGenVertexArrays(1, &VAO_light_cube);

    glBindVertexArray(VAO_container);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertices), g_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(VAO_light_cube);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Light light{
        glm::vec3(1.2f, 1.0f, 2.0f),  // pos

        glm::vec3(0.2f, 0.2f, 0.2f),  // amb
        glm::vec3(0.5f, 0.5f, 0.5f),  // diff
        glm::vec3(1.0f, 1.0f, 1.0f),  // spec
    };
    Material material{
        glm::vec3(1.0f, 0.5f, 0.31f),  // amb
        glm::vec3(1.0f, 0.5f, 0.31f),  // diff
        glm::vec3(0.5f, 0.5f, 0.5f),   // spec

        32.0f  // shininess
    };

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        float current_time = static_cast<float>(glfwGetTime());
        delta_time = current_time - last_time;
        last_time = current_time;

        std::cout << "FPS: " << 1.0f / delta_time << std::endl;

        // input
        process_input(window);

        // rendering..

        // draw container
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model_container = glm::mat4(1.0f);
        glm::mat4 view = camera_get_view_matrix(camera);
        glm::mat4 projection = glm::perspective(
            glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        glm::vec3 light_color;
        light_color.x = sin(glfwGetTime() * 2.0f);
        light_color.y = sin(glfwGetTime() * 0.7f);
        light_color.z = sin(glfwGetTime() * 1.3f);

        light.diffuse = light_color * glm::vec3(0.5f);
        light.ambient = light.diffuse * glm::vec3(0.2f);

        shader_use(lighting_shader);

        shader_set_mat4(lighting_shader, "model", model_container);
        shader_set_mat4(lighting_shader, "view", view);
        shader_set_mat4(lighting_shader, "projection", projection);

        glm::mat3 normal_matrix = glm::mat3(glm::transpose(glm::inverse(model_container)));
        shader_set_mat3(lighting_shader, "normal_matrix", normal_matrix);

        shader_set_vec3(lighting_shader, "viewer_world_position", camera.position);

        set_directional_light_uniform(lighting_shader, light);
        set_material_uniform(lighting_shader, material);

        glBindVertexArray(VAO_container);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // draw lightcube

        glm::mat4 model_light_cube = glm::mat4(1.0f);

        model_light_cube = glm::translate(model_light_cube, light.world_position);
        model_light_cube = glm::scale(model_light_cube, glm::vec3(0.2f, 0.2f, 0.2f));

        shader_use(light_cube_shader);

        shader_set_mat4(light_cube_shader, "model", model_light_cube);
        shader_set_mat4(light_cube_shader, "view", view);
        shader_set_mat4(light_cube_shader, "projection", projection);

        shader_set_vec3(light_cube_shader, "light_color", light.diffuse);

        glBindVertexArray(VAO_light_cube);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // check events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    shader_destroy(lighting_shader);
    shader_destroy(light_cube_shader);

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

void set_directional_light_uniform(const Shader& shader, const Light& light)
{
    shader_set_vec3(shader, "light.world_position", light.world_position);

    shader_set_vec3(shader, "light.ambient", light.ambient);
    shader_set_vec3(shader, "light.diffuse", light.diffuse);
    shader_set_vec3(shader, "light.specular", light.specular);
}
void set_material_uniform(const Shader& shader, const Material& material)
{
    shader_set_vec3(shader, "material.ambient", material.ambient);
    shader_set_vec3(shader, "material.diffuse", material.diffuse);
    shader_set_vec3(shader, "material.specular", material.specular);

    shader_set_float(shader, "material.shininess", material.shininess);
}
