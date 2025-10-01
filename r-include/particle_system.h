#pragma once
#include <vector>
#include "camera.h"
#include "glm/glm.hpp"
#include "shader.h"

#define MAX_PARTICLES 30000

struct ParticleTransform
{
    glm::vec3 pos;
    float rotation;
    float scale;
};

struct ParticlePhysics
{
    glm::vec3 velocity;
    float elapsed_time = 0;
};

struct ParticleSystem
{
    float particle_gravity_effect;
    float particle_life_length;
    std::vector<ParticlePhysics> particles_p_data;
    std::vector<ParticleTransform> particles_t_data;
    int max_particles = 10000;
};

struct ParticleRenderer
{
    unsigned int quad_vao;
    unsigned int instance_vbo;
    Shader shader;
};

struct ParticleEmitter
{
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    float spread = 1.0f;
    float base_speed = 3.0f;
    float scale = 1.0f;
    glm::vec3 direction_range_0 = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 direction_range_1 = glm::vec3(1.0f, 1.0f, 1.0f);
};

struct ParticleEmitterVolume
{
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    glm::vec3 volume_size = {1.0f, 1.0f, 1.0f};
    glm::vec3 direction = {0.0f, 1.0f, 0.0f};
    float spread = 1.0f;
    float base_speed = 3.0f;
    float scale = 1.0f;
    float rotation = 0.0f;
    float randomize_rotation = 0.0f;
    float randomize_speed = 0.0f;
};

void update_particles(ParticleSystem& particle_system, float dt);
void set_renderer(ParticleRenderer& renderer, Shader shader);
void particles_render(
    const ParticleSystem& particle_system, const ParticleRenderer& renderer, const Camera& camera);
void emit_particles(ParticleSystem& ps, const ParticleEmitter& emitter, int count);
void emit_particles_volume(ParticleSystem& ps, const ParticleEmitterVolume& emitter, int count);
