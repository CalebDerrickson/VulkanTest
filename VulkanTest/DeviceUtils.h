#ifndef __DEVICE_UTILS__
#define __DEVICE_UTILS__

#include "MainUtils.h"
#include "SwapChainUtils.h"
#include "CommonUtils.h"

namespace DeviceUtils {

	bool checkDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionCount{};
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const VkExtensionProperties& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		bool suitability = true;

		// Family Indices check
		CommonUtils::QueueFamilyIndices indices = CommonUtils::findQueueFamilies(device, surface);
		suitability = suitability && indices.isComplete();

		// Device Extensions Check
		suitability = suitability && checkDeviceExtensionSupport(device);

		// SwapChain check
		if (suitability) {
			SwapChainUtils::SwapChainSupportDetails swapchainSupport = SwapChainUtils::querySwapChainSupport(device, surface);
			suitability = suitability && !(swapchainSupport.formats.empty() || swapchainSupport.presentModes.empty());
		}


		return suitability;
	}

	
}


#endif // !__DEVICE_UTILS__
