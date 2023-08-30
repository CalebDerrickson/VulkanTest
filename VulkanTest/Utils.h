#pragma once

#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#define VK_USE_PLATFORM_WIN32_KHR

#include "common/QueueFamilyIndices.h"
#include "common/Vertex.h"
#include "common/SwapChainSupportDetails.h"
#include "constants.h"


#include <string>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>
#include <tiny_obj_loader.h>


namespace CommonUtils
{

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

	std::vector<char> readFile(const char* filename);

}

namespace DebugUtils
{
	VkResult CreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger);

	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);


	void DestroyDebugUtilsMessengerEXT(
		VkInstance instance,
		VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator);
}

namespace DeviceUtils
{

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	uint32_t findMemoryType(uint32_t& typeFilter, const VkMemoryPropertyFlags& properties, VkPhysicalDevice physicalDevice);

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice physicalDevice);

	VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice physicalDevice);
	bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);

}


namespace InstanceUtils
{
	bool checkValidationLayerSupport();

	std::vector<const char*> getRequiredExtensions();
}

namespace MainUtils
{
	VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool, VkDevice device);

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool commandPool, VkQueue graphicsQueue, VkDevice device);

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, VkCommandPool commandPool,
		VkQueue graphicsQueue, VkDevice device);

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory,
		VkPhysicalDevice physicalDevice, VkDevice device);

	void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format,
		VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory,
		VkPhysicalDevice physicalDevice, VkDevice device);
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels, VkDevice device);


	void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkCommandPool commandPool, VkQueue graphicsQueue, VkDevice device);

	VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);

	void generateMipMaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels,
		VkCommandPool commandPool, VkQueue graphicsQueue, VkDevice device, VkPhysicalDevice physicalDevice);

	bool hasStencilComponent(VkFormat format);

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout,
		uint32_t mipLevels, VkCommandPool commandPool, VkQueue graphicsQueue, VkDevice device);

	void loadModel(std::vector<uint32_t>& indicesObject, std::vector<Vertex>& verticesObject);

}

namespace ShaderUtils
{

	VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);
}

namespace SwapChainUtils
{

	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

}

