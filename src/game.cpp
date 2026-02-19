#include "game.h"
#include "camera.h"
#include "processInput.h"
#include "../engine/vulkan_context.h"
#include "../engine/vulkan_swapchain.h"
#include "../engine/vulkan_pipeline.h"
#include "../engine/vulkan_renderer.h"
#include "../engine/vulkan_buffer.h"
#include "../engine/vulkan_vertex.h"
#include "../engine/physics_world.h"
#include "../engine/debug_ui.h"
#include <iostream>

#include "Jolt/Physics/Body/MotionType.h"

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

    physicsWorld = new PhysicsWorld();
    physicsWorld->create();

    debugUI = new DebugUI(window, vulkanContext->getInstance(), vulkanContext->getPhysicalDevice(), vulkanContext->getDevice(), vulkanContext->getGraphicsQueue(), vulkanPipeline->getRenderPass(), vulkanSwapchain->getImages().size(), vulkanContext->findQueueFamilies(vulkanContext->getPhysicalDevice()).graphicsFamily);
    debugUI->create();

    processInput = new ProcessInput();
}

void Game::initGame() {
    auto [gunMesh, gunIndices] = loadMesh("../assets/models/gun3.obj", vulkanContext->getDevice(), vulkanContext->getPhysicalDevice());

    JPH::BodyID gunID = physicsWorld->createBody(
        JPH::Vec3(0.0f, 0.0f, 0.0f),
        JPH::Vec3(0.5f, 0.5f, 0.5f),
        JPH::EMotionType::Static,
        Layers::STATIC);

    VulkanTexture* gunTexture = new VulkanTexture(
        vulkanContext->getDevice(),
        vulkanContext->getPhysicalDevice(),
        vulkanContext->getGraphicsQueue(),
        vulkanContext->findQueueFamilies(vulkanContext->getPhysicalDevice()).graphicsFamily);

    gunTexture->load("../assets/textures/gun_texture3.png", vulkanPipeline->getDescriptorSetLayout());

    gameObjects.push_back({
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    gunID, gunMesh, gunIndices, gunTexture});

    camera = new Camera(glm::vec3(-10.0f, 4.0f, 0.0f));
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
        processInput->processMouse(mouseX - lastMouseX, mouseY - lastMouseY, camera->yaw, camera->pitch);
        lastMouseX = mouseX;
        lastMouseY = mouseY;

        // Keyboard input
        processInput->processKeyboard(window, deltaTime, camera->position, camera->yaw, camera->pitch, 5.0f);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        physicsWorld->update(deltaTime);

        // Sync objects to Jolt physics world
        for (auto& object : gameObjects) {
            if (object.bodyID.IsInvalid()) {
                continue;
            }
            JPH::Vec3 physPos = physicsWorld->getPosition(object.bodyID);
            JPH::Quat physRot = physicsWorld->getRotation(object.bodyID);

            object.transform.position = glm::vec3(physPos.GetX(), physPos.GetY(), physPos.GetZ());
            object.transform.rotation = glm::quat(physRot.GetW(), physRot.GetX(), physRot.GetY(), physRot.GetZ());
        }


        debugUI->beginFrame();
        ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
        ImGui::Text("Objects: %d", (int)gameObjects.size());
        debugUI->endFrame();

        vulkanRenderer->drawObjects(gameObjects, camera->getViewMatrix());
    }

    vkDeviceWaitIdle(vulkanContext->getDevice());
}

void Game::cleanup() {
    delete debugUI;
    delete vulkanRenderer;
    delete vulkanPipeline;
    delete vulkanSwapchain;
    for (auto& object : gameObjects) {
        delete object.mesh;
        delete object.indexBuffer;
        delete object.texture;
    }
    delete vulkanContext;
    glfwDestroyWindow(window);
    glfwTerminate();
    delete camera;
}
