#include "debug_ui.h"
#include <stdexcept>

DebugUI::DebugUI(GLFWwindow* windowIn, VkInstance instanceIn, VkPhysicalDevice physicalDeviceIn, VkDevice deviceIn, VkQueue queueIn, VkRenderPass renderPassIn, uint32_t imageCountIn, uint32_t queueFamilyIndexIn) : window(windowIn), instance(instanceIn), physicalDevice(physicalDeviceIn), device(deviceIn), graphicsQueue(queueIn), renderPass(renderPassIn), imageCount(imageCountIn), queueFamilyIndex(queueFamilyIndexIn) {

}

DebugUI::~DebugUI() {
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
}

void DebugUI::create() {
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

void DebugUI::beginFrame() {
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void DebugUI::endFrame() {
    ImGui::Render();
}
