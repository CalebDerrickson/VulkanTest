#pragma once
#include <vulkan/vulkan.h>
#include "constants.h"
#include "MainUtils.h"

class InstanceManager
{
public:
	InstanceManager(const char* applicationName = "Default Instance Name");
	~InstanceManager();

	InstanceManager(const InstanceManager&) = delete;
	InstanceManager& operator=(const InstanceManager&) = delete;

	//getters
	VkInstance getInstance() const { return _instance; }

private: // Private Functions

	void createInstance();

private: // Private Values

	const char* _applicationName;
	VkInstance _instance;
};

