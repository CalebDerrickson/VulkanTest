#pragma once

#include "constants.h"
#include "Utils.h"

// I don't know if this is fine as a struct 
// or if I should use a class
struct ColorResources
{
	// Functions

	ColorResources();
	~ColorResources();

	void createResources(uint32_t width, uint32_t height, VkFormat swapChainImageformat, VkPhysicalDevice physicalDevice, VkSampleCountFlagBits msaaSamples, VkDevice device);
	void destroyResources(VkDevice device);

	// Values
	VkFormat colorFormat;
	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;

};

