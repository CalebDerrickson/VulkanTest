#include "PhysicalDeviceManager.h"

PhysicalDeviceManager::PhysicalDeviceManager(VkInstance instance, VkSurfaceKHR surface)
	: _physicalDevice(nullptr), _msaaSamples(VK_SAMPLE_COUNT_1_BIT),
	_deviceCount(0), _devices({})
{
	pickPhysicalDevice(instance, surface);
}

PhysicalDeviceManager::~PhysicalDeviceManager()
{

	// Empty?
}

void PhysicalDeviceManager::pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
{
	
	vkEnumeratePhysicalDevices(instance, &_deviceCount, nullptr);

	if (_deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	_devices.resize(_deviceCount);
	vkEnumeratePhysicalDevices(instance, &_deviceCount, _devices.data());

	for (const VkPhysicalDevice device : _devices) {
		if (DeviceUtils::isDeviceSuitable(device, surface)) {
			_physicalDevice = device;
			_msaaSamples = DeviceUtils::getMaxUsableSampleCount(_physicalDevice);
			break;
		}
	}

	if (_physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}

}


