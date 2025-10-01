#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"

Camera camera_create(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
{
    Camera camera;
    camera.position = position;
    camera.world_up = up;
    camera.yaw = yaw;
    camera.pitch = pitch;
    camera.speed = 2.5f;
    camera.sensitivity = 0.1f;
    camera.zoom = 45.0f;
    camera_update_vectors(camera);
    return camera;
}

Camera camera_create(glm::vec3 position, glm::vec3 up, glm::vec3 forward)
{
    float pitch = glm::degrees(asin(glm::clamp(forward.y, -1.0f, 1.0f)));
    float yaw = glm::degrees(atan2(forward.z, forward.x));
    return camera_create(position, up, yaw, pitch);
}

void camera_update_vectors(Camera& camera)
{
    glm::vec3 front;
    front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    front.y = sin(glm::radians(camera.pitch));
    front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.forward = glm::normalize(front);
    camera.right = glm::normalize(glm::cross(camera.forward, camera.world_up));
    camera.up = glm::normalize(glm::cross(camera.right, camera.forward));
}

void camera_process_keyboard(Camera& camera, CameraMovement direction, float delta_time)
{
    float velocity = camera.speed * delta_time;
    if (direction == CAMERA_FORWARD)
    {
        camera.position += camera.forward * velocity;
    }
    if (direction == CAMERA_BACKWARD)
    {
        camera.position -= camera.forward * velocity;
    }
    if (direction == CAMERA_LEFT)
    {
        camera.position -= camera.right * velocity;
    }
    if (direction == CAMERA_RIGHT)
    {
        camera.position += camera.right * velocity;
    }
    if (direction == CAMERA_UP)
    {
        camera.position += camera.up * velocity;
    }
    if (direction == CAMERA_DOWN)
    {
        camera.position -= camera.up * velocity;
    }
}

void camera_process_mouse_movement(
    Camera& camera, float x_offset, float y_offset, bool constrain_pitch)
{
    x_offset *= camera.sensitivity;
    y_offset *= camera.sensitivity;

    camera.yaw += x_offset;
    camera.pitch += y_offset;

    if (constrain_pitch)
    {
        if (camera.pitch > 89.0f)
        {
            camera.pitch = 89.0f;
        }
        if (camera.pitch < -89.0f)
        {
            camera.pitch = -89.0f;
        }
    }

    camera_update_vectors(camera);
}

void camera_process_mouse_scroll(Camera& camera, float y_offset)
{
    camera.zoom -= y_offset;
    if (camera.zoom < 1.0f)
    {
        camera.zoom = 1.0f;
    }
    if (camera.zoom > 45.0f)
    {
        camera.zoom = 45.0f;
    }
}

glm::mat4 camera_get_view_matrix(const Camera& camera)
{
    return glm::lookAt(camera.position, camera.position + camera.forward, camera.up);
}

glm::mat4 update_billboard_matrix(glm::vec3 pos, float rotation, float scale, const glm::mat4& view)
{
    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, pos);
    model[0][0] = view[0][0];
    model[0][1] = view[1][0];
    model[0][2] = view[2][0];
    model[1][0] = view[0][1];
    model[1][1] = view[1][1];
    model[1][2] = view[2][1];
    model[2][0] = view[0][2];
    model[2][1] = view[1][2];
    model[2][2] = view[2][2];

    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(scale, scale, scale));

    return view * model;
}
