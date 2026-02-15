#pragma once

#include "../engine/vulkan_vertex.h"
#include "vector"


std::vector<Vertex> getCubeVertices() {
    std::vector<Vertex> cubeVertices = {
        // Front face
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}},   // front, top left
        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},    // front, top right
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},     // front, bot right
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},    // front, bot left
        // Back face
        {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        // Left face
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        // Right face
        {{0.5f, 0.5f, 0.5f,}, {0.0f, 1.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
        {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
        // Top face
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}},
        // Bot face
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}},
        {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}}
    };

    return cubeVertices;
}

std::vector<uint16_t> getCubeIndices() {
    std::vector<uint16_t> cubeIndices = {
        0,1,2, 0,2,3,
        4,5,6, 4,6,7,
        8,9,10, 8,10,11,
        12,13,14, 12,14,15,
        16,17,18, 16,18,19,
        20,21,22, 20,22,23,
    };

    return cubeIndices;
}
