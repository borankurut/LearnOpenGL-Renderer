#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "particle_system.h"
#include "shader.h"
#include "camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "gl_debug.h"
#include "texture.h"

#define SCR_WIDTH 1920
#define SCR_HEIGHT 1080

Camera camera = camera_create(glm::vec3(0.0f, 0.0f, 0.0f));
float delta_time = 0.0f;
float last_time = 0.0f;

bool first_mouse = true;
bool pause = false;
float last_x = SCR_WIDTH / 2.0f;
float last_y = SCR_HEIGHT / 2.0f;

ParticleSystem particle_system_fog;
ParticleRenderer particle_renderer_fog;
ParticleEmitterVolume particle_emitter_fog;
Texture fog_noise_texture;

ParticleSystem particle_system_rain;
ParticleRenderer particle_renderer_rain;
ParticleEmitterVolume particle_emitter_rain;
Texture particle_texture_rain;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos_in, double ypos_in)
{
    float xpos = static_cast<float>(xpos_in);
    float ypos = static_cast<float>(ypos_in);

    if (first_mouse)
    {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos;
    last_x = xpos;
    last_y = ypos;

    camera_process_mouse_movement(camera, xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera_process_mouse_scroll(camera, static_cast<float>(yoffset));
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
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera.speed = 40;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        camera.speed = 20;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        emit_particles_volume(particle_system_fog, particle_emitter_fog, 20);
        emit_particles_volume(particle_system_rain, particle_emitter_rain, 30);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        particle_system_fog.particles_t_data.resize(0);
        particle_system_fog.particles_p_data.resize(0);
    }

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        pause = true;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
    {
        pause = false;
    }
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);

    GLFWwindow* window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "particle scene", nullptr, nullptr);
    if (!window)
    {
        std::cout << "failed to create window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "failed to init glad\n";
        return -1;
    }

    setup_gl_debug();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // fog particle
    Shader particle_shader_fog =
        shader_create("shaders/particle.vert", "shaders/particle_fog.frag");

    fog_noise_texture = texture_create("textures/perlin_noise.png");

    particle_system_fog.particle_gravity_effect = -0.00f;
    particle_system_fog.particle_life_length = 100.0f;
    particle_system_fog.max_particles = 5;

    particle_emitter_fog.position = glm::vec3(0.0f, 0.0f, -20.0f);
    particle_emitter_fog.volume_size = glm::vec3(100.0f, 20.0f, 100.0f);
    particle_emitter_fog.base_speed = 0.2f;
    particle_emitter_fog.spread = 0.1f;
    particle_emitter_fog.scale = 500.0f;

    set_renderer(particle_renderer_fog, particle_shader_fog);
    shader_use(particle_shader_fog);
    shader_set_int(particle_shader_fog, "particle_tex", 0);
    shader_set_vec3(particle_shader_fog, "fog_color", glm::vec3(0.55f, 0.55f, 0.60f));
    shader_set_float(particle_shader_fog, "fog_density", 0.02f);
    shader_set_float(particle_shader_fog, "fog_global_opacity", 0.3f);

    // rain particle
    Shader particle_shader_rain =
        shader_create("shaders/particle_rain.vert", "shaders/particle_rain.frag");

    particle_texture_rain = texture_create("textures/droplet_cut.png");

    particle_system_rain.particle_gravity_effect = -0.00f;
    particle_system_rain.particle_life_length = 2.0f;
    particle_system_rain.max_particles = 30000;

    particle_emitter_rain.position = glm::vec3(0.0f, 0.0f, -20.0f);
    particle_emitter_rain.volume_size = glm::vec3(20.0f, 1.0f, 150.0f);
    // add emitter direction, dot with volume size.
    particle_emitter_rain.spread = 0.0f;
    particle_emitter_rain.scale = 0.5f;
    particle_emitter_rain.rotation = 38.0f;
    particle_emitter_rain.randomize_rotation = 0.0f;
    particle_emitter_rain.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    particle_emitter_rain.base_speed = 50;

    set_renderer(particle_renderer_rain, particle_shader_rain);
    shader_use(particle_shader_rain);
    shader_set_int(particle_shader_rain, "particle_tex", 0);

    while (!glfwWindowShouldClose(window))
    {
        float current_time = static_cast<float>(glfwGetTime());
        delta_time = current_time - last_time;
        last_time = current_time;

        std::cout << "fps: " << 1.0f / delta_time << std::endl;
        std::cout << "particles: " << particle_system_rain.particles_p_data.size() << std::endl;

        process_input(window);

        particle_emitter_rain.position = camera.position + glm::vec3(0, 30, 0);
        update_particles(particle_system_fog, delta_time);
        update_particles(particle_system_rain, delta_time);

        glClearColor(0x1B / 256.0f, 0x39 / 256.0f, 0x5B / 256.0f, 1.0f);
        /* glClearColor(0, 0, 0, 1.0f); */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);

        texture_bind(fog_noise_texture);
        particles_render(particle_system_fog, particle_renderer_fog, camera);

        texture_bind(particle_texture_rain);
        particles_render(particle_system_rain, particle_renderer_rain, camera);

        if (!pause)
        {
            glfwSwapBuffers(window);
        }
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
