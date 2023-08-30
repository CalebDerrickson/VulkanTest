#pragma once
#include "constants.h"
#include "Utils.h"


class RenderPassManager
{
public:

	RenderPassManager(); 
	~RenderPassManager();

	void createRenderPass(VkDevice device, VkPhysicalDevice physicalDevice, VkFormat swapChainImageformat, VkSampleCountFlagBits msaaSamples);
	void destroyRenderPass(VkDevice device);

	// Getters
	inline VkRenderPass getRenderPass() const noexcept { return _renderPass; }

private:

	VkRenderPass _renderPass;
};

