#include "game.h"
#include "../engine/vulkan_context.h"
#include "../engine/vulkan_swapchain.h"
#include "../engine/vulkan_pipeline.h"
#include "../engine/vulkan_renderer.h"
#include <iostream>

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

    vulkanRenderer = new VulkanRenderer(vulkanContext, vulkanSwapchain, vulkanPipeline);
    vulkanRenderer->create();
}

void Game::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        vulkanRenderer->drawFrame();
    }

    vkDeviceWaitIdle(vulkanContext->getDevice());
}

void Game::cleanup() {
    delete vulkanRenderer;
    delete vulkanPipeline;
    delete vulkanSwapchain;
    delete vulkanContext;
    glfwDestroyWindow(window);
    glfwTerminate();
}
