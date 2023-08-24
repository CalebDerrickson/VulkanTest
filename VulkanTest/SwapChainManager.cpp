#include "SwapChainManager.h"

SwapChainManager::SwapChainManager()
	: _swapChain(VK_NULL_HANDLE), _swapChainImages({}),
	  _swapChainImageFormat(VK_FORMAT_UNDEFINED), 
	  _swapChainExtent(/*NULL*/),
	  _swapChainImageViews({}),
	  _swapChainFramebuffers({})
{


}

SwapChainManager::~SwapChainManager()
{


}

void SwapChainManager::createSwapChain(GLFWwindow* window, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkDevice device)
{

	SwapChainSupportDetails swapChainSupport = SwapChainUtils::querySwapChainSupport(physicalDevice, surface);

	VkSurfaceFormatKHR surfaceFormat = SwapChainUtils::chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = SwapChainUtils::chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = SwapChainUtils::chooseSwapExtent(swapChainSupport.capabilities, window);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = CommonUtils::findQueueFamilies(physicalDevice, surface);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	VkResult res = vkCreateSwapchainKHR(device, &createInfo, nullptr, &_swapChain);
	if (res != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(device, _swapChain, &imageCount, nullptr);
	_swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, _swapChain, &imageCount, _swapChainImages.data());

	_swapChainImageFormat = surfaceFormat.format;
	_swapChainExtent = extent;
}

void SwapChainManager::createImageViews(VkDevice device)
{
	_swapChainImageViews.resize(_swapChainImages.size());

	for (uint32_t i = 0; i < _swapChainImages.size(); i++) {
		_swapChainImageViews[i] = MainUtils::createImageView(_swapChainImages[i], _swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, device);
	}

}
