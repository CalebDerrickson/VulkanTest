#pragma once
#include "constants.h"
#include "Utils.h"

template<typename T>
struct BufferObject
{
	// Functions

	BufferObject();
	~BufferObject();

	void createVkBuffer(VkBufferUsageFlags usage, VkPhysicalDevice physicalDevice, VkDevice device,
		VkQueue graphicsQueue, VkCommandPool commandPool);

	void destroyBuffer(VkDevice device);

	// Values
	VkBuffer buffer;
	VkDeviceMemory bufferMemory;
	std::vector<T> bufferObject;

};

template<typename T>
BufferObject<T>::BufferObject()
	: buffer(VK_NULL_HANDLE),
	bufferMemory(VK_NULL_HANDLE),
	bufferObject({})
{


}

template<typename T>
BufferObject<T>::~BufferObject()
{


}

template<typename T>
void BufferObject<T>::destroyBuffer(VkDevice device)
{

	vkDestroyBuffer(device, buffer, nullptr);
	vkFreeMemory(device, bufferMemory, nullptr);
}

template<typename T>
void BufferObject<T>::createVkBuffer(VkBufferUsageFlags usage, VkPhysicalDevice physicalDevice, VkDevice device,
	VkQueue graphicsQueue, VkCommandPool commandPool)
{

	VkDeviceSize bufferSize = sizeof(bufferObject[0]) * bufferObject.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	MainUtils::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory, physicalDevice, device);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, bufferObject.data(), (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	MainUtils::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		buffer, bufferMemory, physicalDevice, device);

	MainUtils::copyBuffer(stagingBuffer, buffer, bufferSize, commandPool, graphicsQueue, device);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}
