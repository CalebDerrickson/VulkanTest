#pragma once

#include "constants.h"
#include "Utils.h"

// I don't know if this is fine as a struct 
// or if I should use a class
struct DepthResources
{
	// Functions

	DepthResources();
	~DepthResources();

	void createResources(uint32_t width, uint32_t height, VkPhysicalDevice physicalDevice, VkSampleCountFlagBits msaaSamples, VkDevice device);
	void destroyResources(VkDevice device);

	// Values
	VkFormat depthFormat;
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

};

