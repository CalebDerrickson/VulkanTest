#include "HelloTriangleApplication.h"

void HelloTriangleApplication::mainLoop() 
{
	GLFWwindow* window = window();
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		
		drawFrame();
		
	}

	window = nullptr;
	vkDeviceWaitIdle(device());
}

void HelloTriangleApplication::drawFrame()
{

	vkWaitForFences(device(), 1, &_inFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);
	

	uint32_t imageIndex;

	VkResult result = vkAcquireNextImageKHR(device(), swapChain(), UINT64_MAX,
		_imageAvailableSemaphores[_currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		_swapChainManager.recreateSwapChain(window(), device(), 
			physicalDevice(), surface(), 
			_physicalDeviceManager.getMsaaSamples(), renderPass());
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	// Only reset the fence if we are submitting work
	vkResetFences(device(), 1, &_inFlightFences[_currentFrame]);

	vkResetCommandBuffer(commandBuffers()[_currentFrame], 0);
	recordCommandBuffer(commandBuffers()[_currentFrame], imageIndex);

	updateUniformBuffer(_currentFrame);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { _imageAvailableSemaphores[_currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers()[_currentFrame];

	VkSemaphore signalSemaphores[] = { _renderFinishedSemaphores[_currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(_deviceManager.getGraphicsQueue(), 1, &submitInfo, _inFlightFences[_currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapChain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	result = vkQueuePresentKHR(_deviceManager.getPresentQueue(), &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _windowManager.getFramebufferResized()) {
		_windowManager.setFramebufferResized(false);
		_swapChainManager.recreateSwapChain(window(), device(),
			physicalDevice(), surface(),
			_physicalDeviceManager.getMsaaSamples(), renderPass());
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	_currentFrame = ++_currentFrame % MAX_FRAMES_IN_FLIGHT;
}

void HelloTriangleApplication::updateUniformBuffer(uint32_t currentImage)
{

	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();

	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo{};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), _swapChainManager.getSwapChainExtent().width / (float)_swapChainManager.getSwapChainExtent().height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	memcpy(_uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}