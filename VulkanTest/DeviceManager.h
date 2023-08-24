#pragma once

#include "constants.h"
#include "Utils.h"
#include "common/QueueFamilyIndices.h"

class DeviceManager
{
public:
	DeviceManager();
	~DeviceManager();

	DeviceManager(const DeviceManager&) = delete;
	DeviceManager& operator=(const DeviceManager&) = delete;

	void createLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	void destroyDevice();

	VkDevice getDevice() { return _device; }
	VkQueue getGraphicsQueue() { return _graphicsQueue; }
	VkQueue getPresentQueue() { return _presentQueue; }

private:

	VkDevice _device;
	VkQueue _graphicsQueue;
	VkQueue _presentQueue;
	QueueFamilyIndices _indices;
};

