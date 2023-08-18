#ifndef __MAIN_UTILS__
#define __MAIN_UTILS__


#include <iostream>
#include <fstream>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <optional>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <set>
#include <string>
#include <vulkan/vk_enum_string_helper.h>


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
};

#include "CommonUtils.h"
#include "DebugUtils.h"
#include "DeviceUtils.h"
#include "InstanceUtils.h"
#include "SwapChainUtils.h"
#include "ShaderUtils.h"

namespace MainUtils {

	// The right way to allocate memory for a large number of objects at the same time 
	// is to create a custom allocator that splits up a single allocation among many 
	// different objects by using the offset parameters that we've seen in many functions.
	// 
	// 	You can either implement such an allocator yourself, or use the VulkanMemoryAllocator 
	// library provided by the GPUOpen initiative.However, for this tutorial it's okay to use a 
	// separate allocation for every resource, because we won't come close to hitting any of 
	// these limits for now.

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory,
		VkPhysicalDevice physicalDevice, VkDevice device)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = DeviceUtils::findMemoryType(memRequirements.memoryTypeBits, properties, physicalDevice);

		if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(device, buffer, bufferMemory, 0);
	}

	// this one either
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool commandPool, VkQueue graphicsQueue, VkDevice device) 
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferCopy copyRegion{};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);

		// This is for the time being, We could use a fence and wait to 
		// schedule multiple transfers simultaneously, thus better optimization
		vkQueueWaitIdle(graphicsQueue);

		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
	}

	template<typename T>
	void createVkBuffer(T& vkData, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkBufferUsageFlags usage, VkPhysicalDevice physicalDevice, VkDevice device,
		VkQueue graphicsQueue, VkCommandPool commandPool)
	{

		VkDeviceSize bufferSize = sizeof(vkData[0]) * vkData.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		MainUtils::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory, physicalDevice, device);

		void* data;
		vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vkData.data(), (size_t)bufferSize);
		vkUnmapMemory(device, stagingBufferMemory);

		MainUtils::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage , VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			buffer, bufferMemory, physicalDevice, device);

		MainUtils::copyBuffer(stagingBuffer, buffer, bufferSize, commandPool, graphicsQueue, device);

		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, stagingBufferMemory, nullptr);
	}



}

#endif // !__MAIN_UTILS__
