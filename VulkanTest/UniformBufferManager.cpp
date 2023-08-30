#include "UniformBufferManager.h"

UniformBufferManager::UniformBufferManager()
	: _uniformBuffers({}),
	  _uniformBuffersMemory({}),
	  _uniformBuffersMapped({})
{

}


UniformBufferManager::~UniformBufferManager()
{


}


void UniformBufferManager::createUniformBuffers(VkDevice device, VkPhysicalDevice physicalDevice)
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	_uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	_uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
	_uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		MainUtils::createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			_uniformBuffers[i], _uniformBuffersMemory[i], physicalDevice, device);

		vkMapMemory(device, _uniformBuffersMemory[i], 0, bufferSize, 0, &_uniformBuffersMapped[i]);
	}

}

void UniformBufferManager::destroyUniformBuffers(VkDevice device)
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroyBuffer(device, _uniformBuffers[i], nullptr);
		vkFreeMemory(device, _uniformBuffersMemory[i], nullptr);
	}
}