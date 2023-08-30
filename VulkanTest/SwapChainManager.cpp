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

void SwapChainManager::createSwapChain(GLFWwindow* window, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkDevice device, VkSampleCountFlagBits msaaSamples)
{
	
	
	SwapChainSupportDetails	swapChainSupport = SwapChainUtils::querySwapChainSupport(physicalDevice, surface);
	
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

	colorResources.createResources(_swapChainExtent.width, _swapChainExtent.height,
		_swapChainImageFormat, physicalDevice, msaaSamples, device);

	depthResources.createResources(_swapChainExtent.width, _swapChainExtent.height,
		physicalDevice, msaaSamples, device);

}

void SwapChainManager::createImageViews(VkDevice device)
{
	_swapChainImageViews.resize(_swapChainImages.size());

	for (uint32_t i = 0; i < _swapChainImages.size(); i++) {
		_swapChainImageViews[i] = MainUtils::createImageView(_swapChainImages[i], _swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, device);
	}

}

void SwapChainManager::createFramebuffers(VkDevice device, VkRenderPass renderPass)
{

	_swapChainFramebuffers.resize(_swapChainImageViews.size());

	for (size_t i = 0; i < _swapChainImageViews.size(); i++) {
		std::array<VkImageView, 3> attachments = {
			colorResources.colorImageView,
			depthResources.depthImageView,
			_swapChainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;

		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = _swapChainExtent.width;
		framebufferInfo.height = _swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &_swapChainFramebuffers[i])
			!= VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

void SwapChainManager::recreateSwapChain(GLFWwindow* window, VkDevice device, VkPhysicalDevice physicalDevice, 
	VkSurfaceKHR surface, VkSampleCountFlagBits msaaSamples, VkRenderPass renderPass)
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(device);

	cleanupSwapChain(device);

	createSwapChain(window, physicalDevice, surface, device, msaaSamples);
	createImageViews(device);

	colorResources.createResources(_swapChainExtent.width, _swapChainExtent.height,
		_swapChainImageFormat, physicalDevice, msaaSamples, device);

	depthResources.createResources(_swapChainExtent.width, _swapChainExtent.height,
		physicalDevice, msaaSamples, device);

	createFramebuffers(device, renderPass);

}

void SwapChainManager::cleanupSwapChain(VkDevice device)
{

	vkDestroyImageView(device, depthResources.depthImageView, nullptr);
	vkDestroyImage(device, depthResources.depthImage, nullptr);
	vkFreeMemory(device, depthResources.depthImageMemory, nullptr);

	vkDestroyImageView(device, colorResources.colorImageView, nullptr);
	vkDestroyImage(device, colorResources.colorImage, nullptr);
	vkFreeMemory(device, colorResources.colorImageMemory, nullptr);

	
	for (VkFramebuffer framebuffer : _swapChainFramebuffers) {
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}

	for (VkImageView imageView : _swapChainImageViews) {
		vkDestroyImageView(device, imageView, nullptr);
	}

	vkDestroySwapchainKHR(device, _swapChain, nullptr);
}

