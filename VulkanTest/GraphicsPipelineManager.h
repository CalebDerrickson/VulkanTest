#pragma once
#include "constants.h"
#include "Utils.h"

class GraphicsPipelineManager
{
public:
	GraphicsPipelineManager();
	~GraphicsPipelineManager();

	void createGraphicsPipeline(VkDevice device, VkPhysicalDevice physicalDevice,
		VkDescriptorSetLayout descriptorSetLayout, VkRenderPass renderPass,
		VkSampleCountFlagBits msaaSamples);

	void destroyGraphicsPipelineAndLayout(VkDevice device);

	inline VkPipelineLayout getGraphicsPipelineLayout() const noexcept { return _pipelineLayout; }
	inline VkPipeline getGraphicsPipeline() const noexcept { return _graphicsPipeline; }

private:

	VkPipelineLayout _pipelineLayout;
	VkPipeline _graphicsPipeline;
};

