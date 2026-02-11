#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "game_object.h"


class VulkanContext;
class VulkanSwapchain;
class VulkanPipeline;
class VulkanRenderer;
class VulkanTexture;
class VulkanBuffer;
class Camera;
class ProcessInput;
class DebugUI;



class Game {
public:
    void run();

private:
    void initEngine();
    void initGame();
    void mainLoop();
    void cleanup();

    GLFWwindow* window = nullptr;
    VulkanContext* vulkanContext = nullptr;
    VulkanSwapchain* vulkanSwapchain = nullptr;
    VulkanPipeline* vulkanPipeline = nullptr;
    VulkanRenderer* vulkanRenderer = nullptr;
    DebugUI* debugUI = nullptr;

    VulkanTexture* dirtTexture = nullptr;
    VulkanTexture* grassTexture = nullptr;
    VulkanBuffer* cubeMesh = nullptr;

    std::vector<GameObject> gameObjects;

    VkDescriptorPool imGuiDescriptorPool = nullptr;

    Camera* camera = nullptr;
    ProcessInput* processInput = nullptr;

    float lastFrame = 0.0f;
    float deltaTime = 0.0f;
    double lastMouseX = 400.0;
    double lastMouseY = 400.0;
    bool firstMouse = true;

    const uint32_t WIDTH = 1920;
    const uint32_t HEIGHT = 1080;
};