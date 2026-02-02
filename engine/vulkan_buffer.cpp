#include "vulkan_buffer.h"
#include "vulkan_vertex.h"
#include <stdexcept>



VulkanBuffer::VulkanBuffer(VkDevice deviceIn, VkPhysicalDevice physicalDeviceIn) : device(deviceIn), physicalDevice(physicalDeviceIn) {
}

VulkanBuffer::~VulkanBuffer() {
    vkFreeMemory(device, bufferMemory, nullptr);
    vkDestroyBuffer(device, buffer, nullptr);
}

void VulkanBuffer::create(VkDeviceSize sizeIn, void* dataIn) {
    bufferSize = sizeIn;
    vertexCount = bufferSize / sizeof(Vertex);
    createBuffer(sizeIn);
    allocateMemory();
    copyData(dataIn, sizeIn);
}

void VulkanBuffer::createBuffer(VkDeviceSize size) {
    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size = size;
    bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    vkCreateBuffer(device, &bufferCreateInfo, nullptr, &buffer);
}

void VulkanBuffer::allocateMemory() {
    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);

    VkMemoryAllocateInfo memoryAllocateInfo = {};
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &bufferMemory);
}

void VulkanBuffer::copyData(const void* data, VkDeviceSize size) const {
    void* mappedMemory;

    vkBindBufferMemory(device, buffer, bufferMemory, 0);
    vkMapMemory(device, bufferMemory, 0, size, 0, &mappedMemory);
    memcpy(mappedMemory, data, size);
    vkUnmapMemory(device, bufferMemory);
}

uint32_t VulkanBuffer::findMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags properties) const {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
            }
    }

    throw std::runtime_error("Failed to find suitable memory type!");
}
