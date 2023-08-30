#pragma once
#include "constants.h"
#include "Utils.h"

#include "BufferObject.h"
class CommandManager 
{
public:

	CommandManager();
	~CommandManager();


	void createCommandPool(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	void destroyCommandPool(VkDevice device);

	void createCommandBuffers(VkDevice device);


	void beginRecordCommandBuffer(VkCommandBuffer commandBuffer);
	void endRecordCommandBuffer(VkCommandBuffer commandBuffer);
	void recordCommandBuffer(uint32_t currentFrame, uint32_t imageIndex,
		BufferObject<uint32_t> indices, BufferObject<Vertex> vertices, VkPipeline graphicsPipeline,
		VkPipelineLayout graphicsPipelineLayout, std::vector<VkFramebuffer> swapchainFrameBuffers,
		VkExtent2D swapChainExtent, std::vector<VkDescriptorSet> descriptorSet,
		VkRenderPass renderPass);

	// getters
	inline VkCommandPool getCommandPool() const noexcept { return _commandPool; }
	inline std::vector<VkCommandBuffer> getCommandBuffers() const noexcept { return _commandBuffers; }

private:
	
	VkCommandPool _commandPool;
	QueueFamilyIndices _queueFamilyIndices;
	std::vector<VkCommandBuffer> _commandBuffers;
};