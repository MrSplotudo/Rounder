#pragma once
#define GLFW_INCLUDE_VULKAN
#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "imgui_impl_glfw.h"
#include "debug_ui.h"
#include <stdexcept>

class DebugUI {
public:
    DebugUI(GLFWwindow* windowIn, VkInstance instanceIn, VkPhysicalDevice physicalDeviceIn, VkDevice deviceIn, VkQueue queueIn, VkRenderPass renderPassIn, uint32_t imageCountIn, uint32_t queueFamilyIndexIn) : window(windowIn), instance(instanceIn), physicalDevice(physicalDeviceIn), device(deviceIn), graphicsQueue(queueIn), renderPass(renderPassIn), imageCount(imageCountIn), queueFamilyIndex(queueFamilyIndexIn) {

    }
    ~DebugUI() {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        vkDestroyDescriptorPool(device, descriptorPool, nullptr);
    }

    void create() {
        ImGui_ImplVulkan_InitInfo imGuiInitInfo = {};
        imGuiInitInfo.Instance = instance;
        imGuiInitInfo.PhysicalDevice = physicalDevice;
        imGuiInitInfo.Device = device;
        imGuiInitInfo.QueueFamily = queueFamilyIndex;
        imGuiInitInfo.Queue = graphicsQueue;
        imGuiInitInfo.PipelineInfoMain.RenderPass = renderPass;
        imGuiInitInfo.ImageCount = imageCount;
        imGuiInitInfo.MinImageCount = imageCount;

        VkDescriptorPoolSize poolSize = {};
        poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSize.descriptorCount = 100;
        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = 100;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

        if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create descriptor pool!");
        }
        imGuiInitInfo.DescriptorPool = descriptorPool;

        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplVulkan_Init(&imGuiInitInfo);
    }

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