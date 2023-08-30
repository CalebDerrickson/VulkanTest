#pragma once
#include "constants.h"
#include "Utils.h"
class UniformBufferManager
{
	// A Uniform Buffer Object (UBO) in Vulkan C++ is a data structure you 
	// use to pass uniform data to shaders. Think of it as a way to send 
	// information like transformation matrices, lighting parameters, 
	// and other constant data from your CPU to your shaders in the GPU.


public:
	UniformBufferManager();
	~UniformBufferManager();


	void createUniformBuffers(VkDevice device, VkPhysicalDevice physicalDevice);
	void destroyUniformBuffers(VkDevice device);


	inline std::vector<VkBuffer> getUniformBuffers() const noexcept { return _uniformBuffers; }
	inline std::vector<VkDeviceMemory> getUniformBuffersMemory() const noexcept { return _uniformBuffersMemory; }
	inline std::vector<void*> getUniformBuffersMapped() const noexcept { return _uniformBuffersMapped; }

private:

	std::vector<VkBuffer> _uniformBuffers;
	std::vector<VkDeviceMemory> _uniformBuffersMemory;
	std::vector<void*> _uniformBuffersMapped;
};

