#pragma once

#include "MainUtils.h"
#include "../common/QueueFamilyIndices.h"
namespace CommonUtils {
	// The buffer copy command requires a queue family that supports transfer operations, which is 
	// indicated using VK_QUEUE_TRANSFER_BIT. The good news is that any queue family with VK_QUEUE_GRAPHICS_BIT 
	// or VK_QUEUE_COMPUTE_BIT capabilities already implicitly support VK_QUEUE_TRANSFER_BIT operations. The 
	// implementation is not required to explicitly list it in queueFlags in those cases.
	//
	// If you like a challenge, then you can still try to use a different queue family specifically for transfer 
	// operations.It will require you to make the following modifications to your program :
	//
	//	Modify QueueFamilyIndices and findQueueFamilies to explicitly look for a queue family with the 
	//		VK_QUEUE_TRANSFER_BIT bit, but not the VK_QUEUE_GRAPHICS_BIT.
	//	Modify createLogicalDevice to request a handle to the transfer queue
	//	Create a second command pool for command buffers that are submitted on the transfer queue family
	//	Change the sharingMode of resources to be VK_SHARING_MODE_CONCURRENT and specify both the graphics and 
	//		transfer queue families
	//	Submit any transfer commands like vkCmdCopyBuffer(which we'll be using in this chapter) to the transfer 
	//		queue instead of the graphics queue

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
	{

		QueueFamilyIndices indices;
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const VkQueueFamilyProperties& queueFamily : queueFamilies) {

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			if (presentSupport) {
				indices.presentFamily = i;
			}

			if (indices.isComplete()) break;
			i++;
		}

		return indices;
	}

	static std::vector<char> readFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file " + filename + "!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}







}
