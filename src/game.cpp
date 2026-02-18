#include "game.h"
#include "mesh_data.h"
#include "camera.h"
#include "processInput.h"
#include "obj_loader.h"
#include "../engine/vulkan_context.h"
#include "../engine/vulkan_swapchain.h"
#include "../engine/vulkan_pipeline.h"
#include "../engine/vulkan_renderer.h"
#include "../engine/vulkan_texture.h"
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
    dirtTexture = new VulkanTexture(vulkanContext->getDevice(), vulkanContext->getPhysicalDevice(), vulkanContext->getGraphicsQueue(), vulkanContext->findQueueFamilies(vulkanContext->getPhysicalDevice()).graphicsFamily);
    dirtTexture->load("../assets/textures/dirt.png", vulkanPipeline->getDescriptorSetLayout());

    // Deer
    std::vector<Vertex> deerVertices;
    std::vector <uint32_t> deerIndices;
    loadOBJ("../assets/models/deer_stag.obj", deerVertices, deerIndices);
    deerMesh = new VulkanBuffer(vulkanContext->getDevice(), vulkanContext->getPhysicalDevice());
    deerMesh->create(sizeof(Vertex) * deerVertices.size(), deerVertices.data(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    deerMeshIndices = new VulkanBuffer(vulkanContext->getDevice(), vulkanContext->getPhysicalDevice());
    deerMeshIndices->create(sizeof(uint32_t) * deerIndices.size(), deerIndices.data(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

    // Cube
    std::vector<Vertex> cubeVertices;
    std::vector<uint32_t> cubeIndices;
    loadOBJ("../assets/models/cube.obj", cubeVertices, cubeIndices);
    cubeMesh = new VulkanBuffer(vulkanContext->getDevice(), vulkanContext->getPhysicalDevice());
    cubeMesh->create(sizeof(Vertex) * cubeVertices.size(), cubeVertices.data(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    cubeMeshIndices = new VulkanBuffer(vulkanContext->getDevice(), vulkanContext->getPhysicalDevice());
    cubeMeshIndices->create(sizeof(uint32_t) * cubeIndices.size(), cubeIndices.data(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

    JPH::BodyID groundID = physicsWorld->createBody(
        JPH::RVec3(0.0f, 0.0f, 0.0f),
        JPH::Vec3(5.0f, 0.5f, 5.0f),
        JPH::EMotionType::Static,
        Layers::STATIC);

    gameObjects.push_back({
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
        cubeMesh, cubeMeshIndices, dirtTexture, groundID});

    JPH::BodyID fallingCubeID = physicsWorld->createBody(
        JPH::RVec3(0.0f, 10.0f, 0.0f),
        JPH::Vec3(0.5f, 0.5f, 0.5f),
        JPH::EMotionType::Dynamic,
        Layers::DYNAMIC);

    gameObjects.push_back({
        glm::vec3(0.0f, 10.0f, 0.0f),
        glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        cubeMesh, cubeMeshIndices, dirtTexture, fallingCubeID});

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
            JPH::RVec3 physPos = physicsWorld->getPosition(object.bodyID);
            JPH::Quat physRot = physicsWorld->getRotation(object.bodyID);

            object.transform.position = glm::vec3(physPos.GetX(), physPos.GetY(), physPos.GetZ());
            object.transform.rotation = glm::quat(physRot.GetW(), physRot.GetX(), physRot.GetY(), physRot.GetZ());
        }


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
    delete deerMesh;
    delete deerMeshIndices;
    delete cubeMesh;
    delete cubeMeshIndices;
    delete vulkanContext;
    glfwDestroyWindow(window);
    glfwTerminate();

    delete camera;
}
