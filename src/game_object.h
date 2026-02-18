#pragma once
#define GLFW_INCLUDE_VULKAN
#include "../engine/vulkan_buffer.h"
#include "../engine/vulkan_texture.h"
#include "Jolt/Jolt.h"
#include "Jolt/Physics/Body/BodyID.h"
#include "transform.h"

struct GameObject {
    Transform transform;
    VulkanBuffer* mesh;
    VulkanBuffer* indexBuffer;
    VulkanTexture* texture;
    JPH::BodyID bodyID;
};
