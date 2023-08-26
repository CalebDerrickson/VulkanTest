#include "CommandPoolManager.h"

CommandPoolManager::CommandPoolManager()
	:_commandPool(nullptr), _queueFamilyIndices()
{


}

CommandPoolManager::~CommandPoolManager()
{


}

void CommandPoolManager::createCommandPool(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{

	_queueFamilyIndices = CommonUtils::findQueueFamilies(physicalDevice, surface);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = _queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(device, &poolInfo, nullptr, &_commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
	
}

void CommandPoolManager::destroyCommandPool(VkDevice device)
{

	vkDestroyCommandPool(device, _commandPool, nullptr);
}