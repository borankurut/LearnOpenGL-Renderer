#pragma once
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include "glm/fwd.hpp"

#define DEFAULT_CAMERA_YAW -90.0f
#define DEFAULT_CAMERA_PITCH 0.0f
#define DEFAULT_CAMERA_SPEED 2.5f
#define DEFAULT_CAMERA_SENSITIVITY 0.1f
#define DEFAULT_CAMERA_ZOOM 45.0f

typedef struct
{
    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;

    float yaw;
    float pitch;
    float speed;
    float sensitivity;
    float zoom;
} Camera;

typedef enum
{
    CAMERA_FORWARD,
    CAMERA_BACKWARD,
    CAMERA_LEFT,
    CAMERA_RIGHT,
    CAMERA_UP,
    CAMERA_DOWN
} CameraMovement;

Camera camera_create(
    glm::vec3 position, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = DEFAULT_CAMERA_YAW,
    float pitch = DEFAULT_CAMERA_PITCH);

Camera camera_create(glm::vec3 position, glm::vec3 up, glm::vec3 forward);

void camera_update_vectors(Camera& camera);
void camera_process_keyboard(Camera& camera, CameraMovement dir, float delta_time);
void camera_process_mouse_movement(
    Camera& camera, float x_offset, float y_offset, bool constrain_pitch = true);
void camera_process_mouse_scroll(Camera& camera, float y_offset);
glm::mat4 camera_get_view_matrix(const Camera& camera);
glm::mat4 update_billboard_matrix(
    glm::vec3 pos, float rotation, float scale, const glm::mat4& view);
