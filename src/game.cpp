#include "game.h"
#include "../engine/vulkan_context.h"
#include "../engine/vulkan_swapchain.h"
#include "../engine/vulkan_pipeline.h"
#include "../engine/vulkan_renderer.h"
#include "../engine/vulkan_texture.h"
#include "../engine/vulkan_buffer.h"
#include "../engine/vulkan_vertex.h"
#include <iostream>
#include <string>

void Game::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void Game::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan App", nullptr, nullptr);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Game::initVulkan() {
    vulkanContext = new VulkanContext(window);

    vulkanSwapchain = new VulkanSwapchain(
        vulkanContext->getPhysicalDevice(),
        vulkanContext->getDevice(),
        vulkanContext->getSurface());
    vulkanSwapchain->create(WIDTH, HEIGHT);

    vulkanPipeline = new VulkanPipeline(
        vulkanContext->getDevice(),
        vulkanSwapchain->getImageFormat(),
        vulkanSwapchain->getExtent());
    vulkanPipeline->createPipeline();

    vulkanRenderer = new VulkanRenderer(vulkanContext, vulkanSwapchain, vulkanPipeline, WIDTH, HEIGHT);
    vulkanRenderer->create();

    dirtTexture = new VulkanTexture(
        vulkanContext->getDevice(),
        vulkanContext->getPhysicalDevice(),
        vulkanContext->getGraphicsQueue(),
        vulkanContext->findQueueFamilies(vulkanContext->getPhysicalDevice()).graphicsFamily);
    dirtTexture->load("../textures/dirt.png", vulkanPipeline->getDescriptorSetLayout());

    std::vector<Vertex> cubeVertices = {
        // Front face
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
        // Back face
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f}},
        // Top face
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f}},
        // Bottom face
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
        // Right face
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
        // Left face
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f}},
    };

    cubeMesh = new VulkanBuffer(vulkanContext->getDevice(), vulkanContext->getPhysicalDevice());
    cubeMesh->create(sizeof(Vertex) * cubeVertices.size(), cubeVertices.data());

    gameObjects.push_back({glm::vec3(0.0f, 0.0f, 0.0f), cubeMesh, dirtTexture});
    gameObjects.push_back({glm::vec3(1.5f, 0.0f, 0.0f), cubeMesh, dirtTexture});
    gameObjects.push_back({glm::vec3(-1.5f, 0.0f, 0.0f), cubeMesh, dirtTexture});

    camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
}

void Game::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        // Mouse input
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        if (firstMouse) {
            lastMouseX = mouseX;
            lastMouseY = mouseY;
            firstMouse = false;
        }
        camera->processMouse(mouseX - lastMouseX, mouseY - lastMouseY);
        lastMouseX = mouseX;
        lastMouseY = mouseY;

        // Keyboard input
        camera->processKeyboard(window, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        vulkanRenderer->drawObjects(gameObjects, camera->getViewMatrix());
    }

    vkDeviceWaitIdle(vulkanContext->getDevice());
}

void Game::cleanup() {
    delete vulkanRenderer;
    delete vulkanPipeline;
    delete vulkanSwapchain;
    delete dirtTexture;
    delete cubeMesh;
    delete vulkanContext;
    glfwDestroyWindow(window);
    glfwTerminate();

    delete camera;
}
