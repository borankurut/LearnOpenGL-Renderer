#pragma once
#include <glm/glm.hpp>

float plane_vertices[] = {
    // positions		 // normals		      // texture coords
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // 1
    0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // 2
    0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // 3
    0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // 4
    -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // 5
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // 6
};
