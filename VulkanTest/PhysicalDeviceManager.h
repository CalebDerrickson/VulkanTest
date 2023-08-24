#pragma once

#include <vulkan/vulkan.h>
#include "constants.h"
#include "Utils.h"


class PhysicalDeviceManager
{
public:

	PhysicalDeviceManager(VkInstance instance, VkSurfaceKHR surface);
	~PhysicalDeviceManager();

	PhysicalDeviceManager(const PhysicalDeviceManager&) = delete;
	PhysicalDeviceManager& operator=(const PhysicalDeviceManager&) = delete;

	// Getters
	VkPhysicalDevice getPhysicalDevice() { return _physicalDevice; }

private: // Private Functions

	void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);


private: // Private Values

	VkPhysicalDevice _physicalDevice;
	VkSampleCountFlagBits _msaaSamples;
	uint32_t _deviceCount;
	std::vector<VkPhysicalDevice> _devices;
};

