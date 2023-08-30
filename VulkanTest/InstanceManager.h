#pragma once
#include <vulkan/vulkan.h>
#include "constants.h"
#include "Utils.h"

class InstanceManager
{
public:
	InstanceManager();
	~InstanceManager();

	InstanceManager(const InstanceManager&) = delete;
	InstanceManager& operator=(const InstanceManager&) = delete;

	//getters
	inline VkInstance getInstance() const noexcept { return _instance; }

	void createInstance(const char* applicationName = "Default Instance Name");
	void destroyInstance();

private: // Private Values

	const char* _applicationName;
	VkInstance _instance;
};

