#pragma once
#include "constants.h"
#include "Utils.h"
#include "TextureManager.h"
class DescriptorManager
{
public:

	DescriptorManager();
	~DescriptorManager();

	void createDescriptorPool(VkDevice device);
	void createDescriptorSets(VkDevice device, std::vector<VkBuffer>&& uniformBuffers, TextureManager& textureManager);
	void createDescriptorSetLayout(VkDevice device);

	void destroyDescriptorPool(VkDevice device);
	void destroyDescriptorSetLayout(VkDevice device);

	inline VkDescriptorPool getDecriptorPool() const noexcept { return _descriptorPool; }
	inline std::vector<VkDescriptorSet> getDescriptorSets() const noexcept { return _descriptorSets; }
	inline VkDescriptorSetLayout getDescriptorSetLayout() const noexcept { return _descriptorSetLayout; }
	
private:

	VkDescriptorPool _descriptorPool;
	std::vector<VkDescriptorSet> _descriptorSets;
	VkDescriptorSetLayout _descriptorSetLayout;
};

