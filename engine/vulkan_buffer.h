#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <array>

struct Vertex {
    std::array<float, 3> position;
    std::array<float, 2> UV;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription =  {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, UV);

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