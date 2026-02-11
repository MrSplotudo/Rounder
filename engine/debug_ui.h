#pragma once
#define GLFW_INCLUDE_VULKAN
#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "imgui_impl_glfw.h"


class DebugUI {
public:
    DebugUI(GLFWwindow* windowIn, VkInstance instanceIn, VkPhysicalDevice physicalDeviceIn, VkDevice deviceIn, VkQueue queueIn, VkRenderPass renderPassIn, uint32_t imageCountIn, uint32_t queueFamilyIndexIn);
    ~DebugUI();

    void create();
    void beginFrame();
    void endFrame();

private:
    GLFWwindow* window;
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkQueue graphicsQueue;
    VkRenderPass renderPass;
    uint32_t imageCount;
    uint32_t queueFamilyIndex;
    VkDescriptorPool descriptorPool = nullptr;
};