#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class VulkanContext;
class VulkanSwapchain;
class VulkanPipeline;
class VulkanRenderer;

class Game {
public:
    void run();

private:
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

    GLFWwindow* window = nullptr;
    VulkanContext* vulkanContext = nullptr;
    VulkanSwapchain* vulkanSwapchain = nullptr;
    VulkanPipeline* vulkanPipeline = nullptr;
    VulkanRenderer* vulkanRenderer = nullptr;

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 800;
};