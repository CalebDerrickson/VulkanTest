#pragma once

#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#define VK_USE_PLATFORM_WIN32_KHR

#include <vulkan/vulkan.h>
#include <vector>

extern const int MAX_FRAMES_IN_FLIGHT;
extern const char* MODEL_PATH;
extern const char* TEXTURE_PATH;

#ifdef NDEBUG 
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	//Enable Best Practices vaildation in the future
};

#define renderPass() _renderPassManager.getRenderPass()
#define swapChain() _swapChainManager.getSwapChain()
#define surface() _surfaceManager.getSurface()
#define window() _windowManager.getWindow()
#define instance() _instanceManager.getInstance()
#define physicalDevice() _physicalDeviceManager.getPhysicalDevice()
#define device() _deviceManager.getDevice()
#define descriptorSetLayout() _graphicsPipelineManager.getDescriptorSetLayout()
#define graphicsPipelineLayout() _graphicsPipelineManager.getGraphicsPipelineLayout()
#define graphicsPipeline() _graphicsPipelineManager.getGraphicsPipeline()
#define commandPool() _commandManager.getCommandPool()
#define commandBuffers() _commandManager.getCommandBuffers()