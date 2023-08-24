#include "DepthResources.h"


DepthResources::DepthResources()
	: depthImage(VK_NULL_HANDLE),
	  depthImageMemory(VK_NULL_HANDLE),
	  depthImageView(VK_NULL_HANDLE)
{


}

DepthResources::~DepthResources()
{


}

void DepthResources::createResources(uint32_t width, uint32_t height, VkPhysicalDevice physicalDevice, VkSampleCountFlagBits msaaSamples, VkDevice device)
{

	VkFormat depthFormat = MainUtils::findDepthFormat(physicalDevice);

	MainUtils::createImage( width, height, 1, msaaSamples,
		depthFormat, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		depthImage, depthImageMemory,
		physicalDevice, device
	);

	depthImageView = MainUtils::createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1,device);

}

void DepthResources::destroyResources(VkDevice device)
{

	vkDestroyImageView(device, depthImageView, nullptr);
	vkDestroyImage(device, depthImage, nullptr);
	vkFreeMemory(device, depthImageMemory, nullptr);
}
