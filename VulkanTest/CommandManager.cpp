#include "CommandManager.h"

CommandManager::CommandManager()
	:_commandPool(nullptr), _queueFamilyIndices()
{


}

CommandManager::~CommandManager()
{


}

void CommandManager::createCommandPool(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{

	_queueFamilyIndices = CommonUtils::findQueueFamilies(physicalDevice, surface);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = _queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(device, &poolInfo, nullptr, &_commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
	
}

void CommandManager::destroyCommandPool(VkDevice device)
{

	vkDestroyCommandPool(device, _commandPool, nullptr);
}


void CommandManager::createCommandBuffers(VkDevice device)
{
		_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = _commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)_commandBuffers.size();

		if (vkAllocateCommandBuffers(device, &allocInfo, _commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}
}

void CommandManager::beginRecordCommandBuffer(VkCommandBuffer commandBuffer)
{
	// Begins record of command buffer
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional
	
	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}
}

void CommandManager::recordCommandBuffer(uint32_t currentFrame, uint32_t imageIndex,
	BufferObject<uint32_t> indices, BufferObject<Vertex> vertices, VkPipeline graphicsPipeline, 
	VkPipelineLayout graphicsPipelineLayout,std::vector<VkFramebuffer> swapchainFrameBuffers,
	VkExtent2D swapChainExtent, std::vector<VkDescriptorSet> descriptorSet, 
	VkRenderPass renderPass)
{


	beginRecordCommandBuffer(_commandBuffers[currentFrame]);

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer = swapchainFrameBuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent;

	//  the order of clearValues should be identical to the order of your attachments.
	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
	clearValues[1].depthStencil = { 1.0f, 0 };

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();


	vkCmdBeginRenderPass(_commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(_commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(_commandBuffers[currentFrame], 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;
	vkCmdSetScissor(_commandBuffers[currentFrame], 0, 1, &scissor);

	VkBuffer vertexBuffers[] = { vertices.buffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(_commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);

	vkCmdBindIndexBuffer(_commandBuffers[currentFrame], indices.buffer, 0, VK_INDEX_TYPE_UINT32);

	vkCmdBindDescriptorSets(_commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS,
		graphicsPipelineLayout, 0, 1, &descriptorSet[currentFrame], 0, nullptr);

	vkCmdDrawIndexed(_commandBuffers[currentFrame], static_cast<uint32_t>(indices.bufferObject.size()), 1, 0, 0, 0);

	vkCmdEndRenderPass(_commandBuffers[currentFrame]);

	endRecordCommandBuffer(_commandBuffers[currentFrame]);
}


void CommandManager::endRecordCommandBuffer(VkCommandBuffer commandBuffer)
{
	//command buffer record end
	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}
}