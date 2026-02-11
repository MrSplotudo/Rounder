#include "game.h"
#include "mesh_data.h"
#include "camera.h"
#include "processInput.h"
#include "../engine/vulkan_context.h"
#include "../engine/vulkan_swapchain.h"
#include "../engine/vulkan_pipeline.h"
#include "../engine/vulkan_renderer.h"
#include "../engine/vulkan_texture.h"
#include "../engine/vulkan_buffer.h"
#include "../engine/vulkan_vertex.h"
#include "../engine/debug_ui.h"
#include <iostream>

void Game::run() {
    initEngine();
    initGame();
    mainLoop();
    cleanup();
}

void Game::initEngine() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan App", nullptr, nullptr);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    vulkanContext = new VulkanContext(window);

    vulkanSwapchain = new VulkanSwapchain(vulkanContext->getPhysicalDevice(), vulkanContext->getDevice(), vulkanContext->getSurface());
    vulkanSwapchain->create(WIDTH, HEIGHT);

    vulkanPipeline = new VulkanPipeline(vulkanContext->getDevice(),vulkanSwapchain->getImageFormat(),vulkanSwapchain->getExtent());
    vulkanPipeline->createPipeline();

    vulkanRenderer = new VulkanRenderer(vulkanContext, vulkanSwapchain, vulkanPipeline, WIDTH, HEIGHT);
    vulkanRenderer->create();

    debugUI = new DebugUI(window, vulkanContext->getInstance(), vulkanContext->getPhysicalDevice(), vulkanContext->getDevice(), vulkanContext->getGraphicsQueue(), vulkanPipeline->getRenderPass(), vulkanSwapchain->getImages().size(), vulkanContext->findQueueFamilies(vulkanContext->getPhysicalDevice()).graphicsFamily);
    debugUI->create();
}

void Game::initGame() {
    dirtTexture = new VulkanTexture(vulkanContext->getDevice(), vulkanContext->getPhysicalDevice(), vulkanContext->getGraphicsQueue(), vulkanContext->findQueueFamilies(vulkanContext->getPhysicalDevice()).graphicsFamily);
    dirtTexture->load("../textures/dirt.png", vulkanPipeline->getDescriptorSetLayout());

    grassTexture = new VulkanTexture(vulkanContext->getDevice(), vulkanContext->getPhysicalDevice(), vulkanContext->getGraphicsQueue(), vulkanContext->findQueueFamilies(vulkanContext->getPhysicalDevice()).graphicsFamily);
    grassTexture->load("../textures/grass.png", vulkanPipeline->getDescriptorSetLayout());

    std::vector<Vertex> cubeVertices = getCubeVertices();

    cubeMesh = new VulkanBuffer(vulkanContext->getDevice(), vulkanContext->getPhysicalDevice());
    cubeMesh->create(sizeof(Vertex) * cubeVertices.size(), cubeVertices.data());

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            gameObjects.push_back({glm::vec3(i, 2.0f, j),glm::vec3(0, 0, 0),glm::vec3(1, 1, 1), cubeMesh, grassTexture});
            gameObjects.push_back({glm::vec3(i, 1.0f, j),glm::vec3(0, 0, 0),glm::vec3(1, 1, 1), cubeMesh, dirtTexture});
        }
    }


    processInput = new ProcessInput();
    camera = new Camera(glm::vec3(0.0f, 5.0f, 3.0f));
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
        processInput->processMouse(mouseX - lastMouseX, mouseY - lastMouseY, camera->transform);
        lastMouseX = mouseX;
        lastMouseY = mouseY;

        // Keyboard input
        processInput->processKeyboard(window, deltaTime, camera->transform, 5.0f);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
        ImGui::Text("Objects: %d", (int)gameObjects.size());
        ImGui::Render();

        vulkanRenderer->drawObjects(gameObjects, camera->getViewMatrix());
    }

    vkDeviceWaitIdle(vulkanContext->getDevice());
}

void Game::cleanup() {
    delete debugUI;
    delete vulkanRenderer;
    delete vulkanPipeline;
    delete vulkanSwapchain;
    delete dirtTexture;
    delete grassTexture;
    delete cubeMesh;
    delete vulkanContext;
    glfwDestroyWindow(window);
    glfwTerminate();

    delete camera;
}
