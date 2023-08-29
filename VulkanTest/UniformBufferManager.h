#pragma once
#include "constants.h"
#include "Utils.h"
class UniformBufferManager
{
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

