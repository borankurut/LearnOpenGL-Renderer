#include "particle_system.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/constants.hpp>
#include "shader.h"
#include "utils.h"

void update_particles(ParticleSystem& particle_system, float dt)
{
    size_t write_index = 0;
    for (size_t i = 0; i < particle_system.particles_p_data.size(); ++i)
    {
        ParticlePhysics& pp = particle_system.particles_p_data[i];
        ParticleTransform& pt = particle_system.particles_t_data[i];
        pp.velocity.y += particle_system.particle_gravity_effect * dt;
        pt.pos += pp.velocity * dt;
        pp.elapsed_time += dt;

        if (pp.elapsed_time <= particle_system.particle_life_length)
        {
            particle_system.particles_p_data[write_index] = pp;
            particle_system.particles_t_data[write_index] = pt;
            ++write_index;
        }
    }
    particle_system.particles_p_data.resize(
        std::min((int)write_index, particle_system.max_particles));
    particle_system.particles_t_data.resize(
        std::min((int)write_index, particle_system.max_particles));
}

void emit_particles(ParticleSystem& particle_system, const ParticleEmitter& emitter, int count)
{
    particle_system.particles_p_data.reserve(particle_system.particles_p_data.size() + count);
    particle_system.particles_t_data.reserve(particle_system.particles_t_data.size() + count);
    for (int i = 0; i < count; ++i)
    {
        ParticlePhysics pp;
        ParticleTransform pt;
        pt.pos = emitter.position;

        glm::vec3 dir(random_m11() * emitter.spread, 1.0f, random_m11() * emitter.spread);
        dir = glm::normalize(dir);
        pp.velocity = dir * (emitter.base_speed + random_01() * 2.0f);

        pt.rotation = random_m11() * 3.14f;
        pt.scale = emitter.scale;
        pp.elapsed_time = 0.0f;

        particle_system.particles_p_data.push_back(pp);
        particle_system.particles_t_data.push_back(pt);
        std::cout << particle_system.particles_p_data.size() << std::endl;
    }
}

void emit_particles_volume(ParticleSystem& ps, const ParticleEmitterVolume& emitter, int count)
{
    ps.particles_p_data.reserve(ps.particles_p_data.size() + count);
    ps.particles_t_data.reserve(ps.particles_t_data.size() + count);

    for (int i = 0; i < count; ++i)
    {
        ParticlePhysics pp;
        ParticleTransform pt;

        float rx = (random_01() - 0.5f) * emitter.volume_size.x;
        float ry = (random_01() - 0.5f) * emitter.volume_size.y;
        float rz = (random_01() - 0.5f) * emitter.volume_size.z;
        pt.pos = emitter.position + glm::vec3(rx, ry, rz);

        glm::vec3 dir_relative_emitter(
            random_m11() * emitter.spread, 1.0f, random_m11() * emitter.spread);
        dir_relative_emitter = glm::normalize(dir_relative_emitter);

        glm::quat rotation_to_emitter =
            glm::rotation(glm::vec3(0.0f, 1.0f, 0.0f), glm::normalize(emitter.direction));
        glm::vec3 dir = glm::normalize(rotation_to_emitter * dir_relative_emitter);

        pp.velocity = dir * (emitter.base_speed + random_01() * emitter.randomize_speed);

        pt.rotation = random_m11() * emitter.randomize_rotation + emitter.rotation;
        pt.scale = emitter.scale;
        pp.elapsed_time = 0.0f;

        ps.particles_p_data.push_back(pp);
        ps.particles_t_data.push_back(pt);
    }
}

void set_renderer(ParticleRenderer& renderer, Shader shader)
{
    renderer.shader = shader;

    GLuint vao, vbo, ebo;
    float vertices[] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, 1.0f, 0.0f,
                        0.5f,  0.5f,  1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 1.0f};
    unsigned int indices[] = {0, 1, 2, 0, 2, 3};

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    GLuint instance_vbo;
    glGenBuffers(1, &instance_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, instance_vbo);
    glBufferData(
        GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(ParticleTransform), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(2);  // instance position
    glVertexAttribPointer(
        2, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleTransform),
        (void*)offsetof(ParticleTransform, pos));
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(
        3, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleTransform),
        (void*)offsetof(ParticleTransform, rotation));
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(
        4, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleTransform),
        (void*)offsetof(ParticleTransform, scale));
    glVertexAttribDivisor(4, 1);

    glBindVertexArray(0);

    renderer.quad_vao = vao;
    renderer.instance_vbo = instance_vbo;
}

void particles_render(
    const ParticleSystem& ps, const ParticleRenderer& renderer, const Camera& camera)
{
    shader_use(renderer.shader);
    glBindVertexArray(renderer.quad_vao);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    /* glBlendFunc(GL_SRC_ALPHA, GL_ONE); */
    glDepthMask(false);

    constexpr float near_plane = 6.0f;
    constexpr float far_plane = 1000.0f;
    glm::mat4 view = camera_get_view_matrix(camera);
    glm::mat4 projection =
        glm::perspective(glm::radians(camera.zoom), 800.0f / 600.0f, near_plane, far_plane);

    shader_set_mat4(renderer.shader, "proj", projection);
    shader_set_mat4(renderer.shader, "view", view);

    glBindBuffer(GL_ARRAY_BUFFER, renderer.instance_vbo);
    glBufferSubData(
        GL_ARRAY_BUFFER, 0,
        std::min((int)ps.particles_t_data.size(), MAX_PARTICLES) * sizeof(ParticleTransform),
        ps.particles_t_data.data());

    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, ps.particles_t_data.size());

    glDepthMask(true);
    glDisable(GL_BLEND);
    glBindVertexArray(0);
}
