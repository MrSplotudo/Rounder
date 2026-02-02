#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <array>

class VulkanBuffer {
public:
    VulkanBuffer(VkDevice deviceIn, VkPhysicalDevice physicalDeviceIn);
    ~VulkanBuffer();

    void create(VkDeviceSize sizeIn, void* dataIn);
    VkBuffer getBuffer() { return buffer; }
    uint32_t getVertexCount() { return vertexCount; }

private:
    void createBuffer(VkDeviceSize size);
    void allocateMemory();
    void copyData(const void* data, VkDeviceSize size) const;
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

    VkDevice device;
    VkPhysicalDevice physicalDevice;
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
    VkDeviceSize bufferSize;

    uint32_t vertexCount;
};