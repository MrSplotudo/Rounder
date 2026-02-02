#pragma once
#define GLFW_INCLUDE_VULKAN
#include "../engine/vulkan_vertex.h"
#include "../engine/vulkan_buffer.h"
#include "../engine/vulkan_texture.h"
#include "glm/glm.hpp"

struct GameObject {
    glm::vec3 position;
    VulkanBuffer* mesh;
    VulkanTexture* texture;
};