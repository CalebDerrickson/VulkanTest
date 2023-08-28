#pragma once
#include "constants.h"
#include "Utils.h"

class CommandManager 
{
public:

	CommandManager();
	~CommandManager();


	void createCommandPool(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	void destroyCommandPool(VkDevice device);

	void createCommandBuffers(VkDevice device);


	void beginRecordCommandBuffer(VkCommandBuffer commandBuffer);
	void endRecordCommandBuffer(VkCommandBuffer commandBuffer);

	// getters
	inline VkCommandPool getCommandPool() noexcept { return _commandPool; }
	inline std::vector<VkCommandBuffer> getCommandBuffers() noexcept { return _commandBuffers; }

private:
	
	VkCommandPool _commandPool;
	QueueFamilyIndices _queueFamilyIndices;
	std::vector<VkCommandBuffer> _commandBuffers;
};