#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
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
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <texture.h>
#include <models/cube/cube.h>

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

void set_directional_light_uniform(
    const Shader& shader, const DirectionalLight& light, const std::string& light_name);

void set_point_light_uniform(
    const Shader& shader, const PointLight& light, const std::string& light_name);

void set_spot_light_uniform(
    const Shader& shader, const SpotLight& light, const std::string& light_name);

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

glm::vec3 point_light_positions[] = {
    glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f), glm::vec3(-4.0f, 2.0f, -12.0f),
    glm::vec3(0.0f, 0.0f, -3.0f)};

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

    Shader lighting_shader = shader_create("shaders/chapter_21.vert", "shaders/chapter_21.frag");
    Shader skybox_shader =
        shader_create("shaders/chapter_27_skybox.vert", "shaders/chapter_27_skybox.frag");

    unsigned int VAO_cube;
    glGenVertexArrays(1, &VAO_cube);
    glBindVertexArray(VAO_cube);

    unsigned int VBO_cube;
    glGenBuffers(1, &VBO_cube);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_cube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    PointLight default_point_light = {
        glm::vec3(1.2f, 1.0f, 2.0f),  // pos

        glm::vec3(0.2f, 0.2f, 0.2f),  // amb
        glm::vec3(0.5f, 0.5f, 0.5f),  // diff
        glm::vec3(1.0f, 1.0f, 1.0f),  // spec

        1.0f,   // const
        0.09f,  // lin
        0.032f  // quad
    };

    PointLight point_lights[4];
    for (int i = 0; i < 4; ++i)
    {
        point_lights[i] = default_point_light;
        point_lights[i].position = point_light_positions[i];
    }

    DirectionalLight directional_light{
        glm::vec3(-0.2f, -1.0f, -0.3f),  // direction

        glm::vec3(0.2f, 0.2f, 0.2f),  // amb
        glm::vec3(0.5f, 0.5f, 0.5f),  // diff
        glm::vec3(1.0f, 1.0f, 1.0f),  // spec
    };

    Model backpack_object = model_create("models/backpack/backpack.obj");

    // create cubemap

    std::string p = "./textures/cubemaps/skybox/";
    std::vector<std::string> faces = {
        p + "right.jpg",   //
        p + "left.jpg",    //
        p + "top.jpg",     //
        p + "bottom.jpg",  //
        p + "front.jpg",   //
        p + "back.jpg",    //
    };

    Cubemap cubemap = cubemap_create(faces);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        float current_time = static_cast<float>(glfwGetTime());
        delta_time = current_time - last_time;
        last_time = current_time;

        // input
        process_input(window);

        // rendering..
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera_get_view_matrix(camera);
        glm::mat4 projection = glm::perspective(
            glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // render scene
        shader_use(lighting_shader);

        shader_set_mat4(lighting_shader, "view", view);
        shader_set_mat4(lighting_shader, "projection", projection);
        shader_set_vec3(lighting_shader, "viewer_world_position", camera.position);

        set_directional_light_uniform(lighting_shader, directional_light, "directional_light");

        for (int i = 0; i < 4; ++i)
        {
            set_point_light_uniform(
                lighting_shader, point_lights[i], "point_lights[" + std::to_string(i) + "]");
        }

        glm::mat4 model_backpack = glm::mat4(1.0f);
        model_backpack = glm::translate(model_backpack, glm::vec3(0.0f, 0.0f, -3.0f));

        shader_set_mat4(lighting_shader, "model", model_backpack);

        glm::mat3 normal_matrix = glm::mat3(glm::transpose(glm::inverse(model_backpack)));
        shader_set_mat3(lighting_shader, "normal_matrix", normal_matrix);

        shader_set_float(lighting_shader, "material.shininess", 32.0f);
        model_draw(backpack_object, lighting_shader);

        // render cubemap

        glDepthFunc(GL_LEQUAL);
        glBindVertexArray(VAO_cube);
        shader_use(skybox_shader);
        shader_set_int(skybox_shader, "sampler_skybox", 0);
        glActiveTexture(GL_TEXTURE0);
        cubemap_bind(cubemap);
        shader_set_mat4(skybox_shader, "view", view);
        shader_set_mat4(skybox_shader, "projection", projection);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);

        // check events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    shader_destroy(lighting_shader);

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

void set_directional_light_uniform(
    const Shader& shader, const DirectionalLight& light, const std::string& light_name)
{
    shader_set_vec3(shader, light_name + ".direction", light.direction);

    shader_set_vec3(shader, light_name + ".ambient", light.ambient);
    shader_set_vec3(shader, light_name + ".diffuse", light.diffuse);
    shader_set_vec3(shader, light_name + ".specular", light.specular);
}

void set_point_light_uniform(
    const Shader& shader, const PointLight& light, const std::string& light_name)
{
    shader_set_vec3(shader, light_name + ".position", light.position);

    shader_set_vec3(shader, light_name + ".ambient", light.ambient);
    shader_set_vec3(shader, light_name + ".diffuse", light.diffuse);
    shader_set_vec3(shader, light_name + ".specular", light.specular);

    shader_set_float(shader, light_name + ".constant_attenuation", light.constant_attenuation);
    shader_set_float(shader, light_name + ".linear_attenuation", light.linear_attenuation);
    shader_set_float(shader, light_name + ".quadratic_attenuation", light.quadratic_attenuation);
}

void set_spot_light_uniform(
    const Shader& shader, const SpotLight& light, const std::string& light_name)
{
    shader_set_vec3(shader, light_name + ".position", light.position);
    shader_set_vec3(shader, light_name + ".direction", light.direction);

    shader_set_float(shader, light_name + ".inner_cutoff_cos", light.inner_cutoff_cos);
    shader_set_float(shader, light_name + ".outer_cutoff_cos", light.outer_cutoff_cos);

    shader_set_vec3(shader, light_name + ".ambient", light.ambient);
    shader_set_vec3(shader, light_name + ".diffuse", light.diffuse);
    shader_set_vec3(shader, light_name + ".specular", light.specular);

    shader_set_float(shader, light_name + ".constant_attenuation", light.constant_attenuation);
    shader_set_float(shader, light_name + ".linear_attenuation", light.linear_attenuation);
    shader_set_float(shader, light_name + ".quadratic_attenuation", light.quadratic_attenuation);
}
