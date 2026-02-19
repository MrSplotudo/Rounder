#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <array>
#include <string>

class VulkanBuffer {
public:
    VulkanBuffer(VkDevice deviceIn, VkPhysicalDevice physicalDeviceIn);
    ~VulkanBuffer();

    void create(VkDeviceSize sizeIn, void* dataIn, VkBufferUsageFlags usage);
    VkBuffer getBuffer() { return buffer; }
    uint32_t getElementCount() { return elementCount; }

private:
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage);
    void allocateMemory();
    void copyData(const void* data, VkDeviceSize size) const;
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

    VkDevice device;
    VkPhysicalDevice physicalDevice;
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
    VkDeviceSize bufferSize;

    uint32_t elementCount;
};

struct MeshBuffers {
    VulkanBuffer* vertexBuffer;
    VulkanBuffer* indexBuffer;
};

MeshBuffers loadMesh(const std::string& path, VkDevice device, VkPhysicalDevice physicalDevice);