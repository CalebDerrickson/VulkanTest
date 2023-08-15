#include "HelloTriangleApplication.h"

void HelloTriangleApplication::mainLoop() 
{

	while (!glfwWindowShouldClose(_window)) {
		glfwPollEvents();
		drawFrame();
	}

	vkDeviceWaitIdle(_device);
}

void HelloTriangleApplication::drawFrame()
{

	vkWaitForFences(_device, 1, &_inFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);
	vkResetFences(_device, 1, &_inFlightFences[_currentFrame]);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(_device, _swapChain, UINT64_MAX, _imageAvailableSemaphores[_currentFrame],
		VK_NULL_HANDLE, &imageIndex);

	vkResetCommandBuffer(_commandBuffers[_currentFrame], 0);
	recordCommandBuffer(_commandBuffers[_currentFrame], imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { _imageAvailableSemaphores[_currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &_commandBuffers[_currentFrame];

	VkSemaphore signalSemaphores[] = { _renderFinishedSemaphores[_currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(_graphicsQueue, 1, &submitInfo, _inFlightFences[_currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { _swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	vkQueuePresentKHR(_presentQueue, &presentInfo);

	_currentFrame = ++_currentFrame % MAX_FRAMES_IN_FLIGHT;
}

