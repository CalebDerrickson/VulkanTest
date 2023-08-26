#pragma once
#include "constants.h"
#include "Utils.h"

class CommandPoolManager 
{
public:

	CommandPoolManager();
	~CommandPoolManager();


	void createCommandPool(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	void destroyCommandPool(VkDevice device);


	VkCommandPool getCommandPool() { return _commandPool; }


private:
	
	VkCommandPool _commandPool;
	QueueFamilyIndices _queueFamilyIndices;
};