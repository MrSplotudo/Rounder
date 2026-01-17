#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
// Std
#include <array>

struct Vertex {
    std::array<float, 2> position;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription =  {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 1> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions = {};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);

        return attributeDescriptions;
    }
};


class VulkanBuffer {
public:
    VulkanBuffer(VkDevice deviceIn, VkPhysicalDevice physicalDeviceIn);
    ~VulkanBuffer();

    void create(VkDeviceSize sizeIn, void* dataIn);
    VkBuffer getBuffer() { return buffer; }

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



};