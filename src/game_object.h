#pragma once
#define GLFW_INCLUDE_VULKAN
#include "../engine/vulkan_buffer.h"
#include "../engine/vulkan_texture.h"
#include "glm/glm.hpp"
#include "transform.h"

struct GameObject {
    Transform transform;
    VulkanBuffer* mesh;
    VulkanBuffer* indexBuffer;
    VulkanTexture* texture;
};