#include "ColorResources.h"


ColorResources::ColorResources()
	: colorImage(VK_NULL_HANDLE),
	  colorImageMemory(VK_NULL_HANDLE),
	  colorImageView(VK_NULL_HANDLE),
	  colorFormat(VK_FORMAT_UNDEFINED)
{


}

ColorResources::~ColorResources()
{


}

void ColorResources::createResources(uint32_t width, uint32_t height, VkFormat swapChainImageformat, VkPhysicalDevice physicalDevice, 
	VkSampleCountFlagBits msaaSamples, VkDevice device)
{

	VkFormat colorFormat = swapChainImageformat;
	MainUtils::createImage(width, height, 1, msaaSamples,
		colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory,
		physicalDevice, device
	);

	colorImageView = MainUtils::createImageView(colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, device);
}

void ColorResources::destroyResources(VkDevice device)
{

	vkDestroyImageView(device, colorImageView, nullptr);
	vkDestroyImage(device, colorImage, nullptr);
	vkFreeMemory(device, colorImageMemory, nullptr);
}
