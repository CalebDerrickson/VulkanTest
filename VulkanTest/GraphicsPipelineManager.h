#pragma once
#include "constants.h"
#include "Utils.h"

class GraphicsPipelineManager
{
public:
	GraphicsPipelineManager();
	~GraphicsPipelineManager();

	void createDescriptorSetLayout(VkDevice device);

	void createGraphicsPipeline(VkDevice device, VkPhysicalDevice physicalDevice,
		VkRenderPass renderPass, VkSampleCountFlagBits msaaSamples);

	void destroyGraphicsPipelineAndLayout(VkDevice device);

	void destroyDescriptorSetLayout(VkDevice device);


	VkDescriptorSetLayout getDescriptorSetLayout() { return _descriptorSetLayout; }
	VkPipelineLayout getGraphicsPipelineLayout() { return _pipelineLayout; }
	VkPipeline getGraphicsPipeline() { return _graphicsPipeline; }

private:

	VkPipelineLayout _pipelineLayout;
	VkPipeline _graphicsPipeline;
	VkDescriptorSetLayout _descriptorSetLayout;
};

