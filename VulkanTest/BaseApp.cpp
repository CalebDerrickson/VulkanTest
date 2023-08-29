
#define TINYOBJLOADER_IMPLEMENTATION

#include "BaseApp.h"
#include "Utils.h"
extern const int MAX_FRAMES_IN_FLIGHT = 2;
extern const char* MODEL_PATH = "models/viking_room.obj";
extern const char* TEXTURE_PATH = "textures/viking_room.png";
uint32_t QueueFamilyIndices::InstanceCount = 0;

BaseApp::BaseApp()
{

}

BaseApp::~BaseApp()
{

}

void BaseApp::initVulkan()
{

	_windowManager.initWindow(WIDTH, HEIGHT, "VULKAN");
	_instanceManager.createInstance("HELLO VULKAN");
	if(enableValidationLayers) _debugManager.setupDebugMessenger(instance());
	_surfaceManager.createSurface(window(), instance());
	_physicalDeviceManager.pickPhysicalDevice(instance(), surface());
	_deviceManager.createLogicalDevice(physicalDevice(), surface());
	
	_swapChainManager.createSwapChain(window(), physicalDevice(), 
		surface(), device(), _physicalDeviceManager.getMsaaSamples());

	_swapChainManager.createImageViews(device());


	_renderPassManager.createRenderPass(device(), physicalDevice(), 
		_swapChainManager.getSwapChainImageFormat(), _physicalDeviceManager.getMsaaSamples());
	
	_graphicsPipelineManager.createDescriptorSetLayout(device());

	_graphicsPipelineManager.createGraphicsPipeline(device(), physicalDevice(), renderPass(),
		_physicalDeviceManager.getMsaaSamples());

	
	_swapChainManager.createFramebuffers(device(), renderPass());

	_commandManager.createCommandPool(device(), physicalDevice(), surface());

	_textureManager.createTextureImage(device(), physicalDevice(), commandPool(), _deviceManager.getGraphicsQueue());
	_textureManager.createTextureImageView(device());
	_textureManager.createTextureSampler(device(), physicalDevice());

	loadModel();
	createVertexBuffer();
	createIndexBuffer();
	_uniformBufferManager.createUniformBuffers(device(), physicalDevice());
	createDescriptorPool();
	createDescriptorSets();
	createCommandBuffers();

	_syncManager.createSyncObjects(device());

}

void BaseApp::loadModel()
{

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::unordered_map<Vertex, uint32_t> uniqueVertices{};
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH)) {
		throw std::runtime_error(warn + err);
	}

	// combining all faces in the file into a single model
	for (const tinyobj::shape_t& shape : shapes) {
		for (const tinyobj::index_t& index : shape.mesh.indices) {
			Vertex vertex{};

			// attrib.vertices array is an array of float values 
			// instead of glm::vec3, hence the 3
			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				       attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = glm::vec3(1.0f);

			//identifying the unique vertices
			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(_vertices.size());
				_vertices.push_back(vertex);
			}
			_indices.push_back(uniqueVertices[vertex]);
		}
	}

}

void BaseApp::createVertexBuffer() 
{
	
	MainUtils::createVkBuffer(_vertices, _vertexBuffer, _vertexBufferMemory, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
		physicalDevice(), device(), _deviceManager.getGraphicsQueue(), commandPool());
}

void BaseApp::createIndexBuffer()
{

	MainUtils::createVkBuffer(_indices, _indexBuffer, _indexBufferMemory, VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		physicalDevice(), device(), _deviceManager.getGraphicsQueue(), commandPool());
}


void BaseApp::createDescriptorPool()
{

	std::array<VkDescriptorPoolSize, 2> poolSizes{};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	if (vkCreateDescriptorPool(device(), &poolInfo, nullptr, &_descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}

}

void BaseApp::createDescriptorSets()
{

	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout());
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = _descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	_descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(device(), &allocInfo, _descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = _uniformBufferManager.getUniformBuffers()[i];
		//bufferInfo.buffer = _uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = _textureManager.textureImageView;
		imageInfo.sampler = _textureManager.textureSampler;

		std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
		// first for ubo
		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = _descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		// then for image
		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = _descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(device(), static_cast<uint32_t>(descriptorWrites.size()),
			descriptorWrites.data(), 0, nullptr);
	}
}

void BaseApp::createCommandBuffers()
{

	_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)_commandBuffers.size();

	if (vkAllocateCommandBuffers(device(), &allocInfo, _commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void BaseApp::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{

	_commandManager.beginRecordCommandBuffer(commandBuffer);
	
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass(); 
	renderPassInfo.framebuffer = _swapChainManager.getSwapChainFrameBuffers()[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = _swapChainManager.getSwapChainExtent();

	//  the order of clearValues should be identical to the order of your attachments.
	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
	clearValues[1].depthStencil = { 1.0f, 0 };
	
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();


	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline());

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)_swapChainManager.getSwapChainExtent().width;
		viewport.height = (float)_swapChainManager.getSwapChainExtent().height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = _swapChainManager.getSwapChainExtent();
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		VkBuffer vertexBuffers[] = { _vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(commandBuffer, _indexBuffer, 0, VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
			graphicsPipelineLayout(), 0, 1, &_descriptorSets[_currentFrame], 0, nullptr);

		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(_indices.size()), 1, 0, 0, 0);

	vkCmdEndRenderPass(commandBuffer);

	_commandManager.endRecordCommandBuffer(commandBuffer);
}


void BaseApp::mainLoop()
{
	GLFWwindow* window = window();
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}

	window = nullptr;
	vkDeviceWaitIdle(device());
}






void BaseApp::cleanup()
{
	
	_swapChainManager.cleanupSwapChain(device());

	_graphicsPipelineManager.destroyGraphicsPipelineAndLayout(device());

	_renderPassManager.destroyRenderPass(device());

	_uniformBufferManager.destroyUniformBuffers(device());

	vkDestroyDescriptorPool(device(), _descriptorPool, nullptr);

	_textureManager.destroyTexture(device());

	_graphicsPipelineManager.destroyDescriptorSetLayout(device());
	

	vkDestroyBuffer(device(), _indexBuffer, nullptr);
	vkFreeMemory(device(), _indexBufferMemory, nullptr);

	vkDestroyBuffer(device(), _vertexBuffer, nullptr);
	vkFreeMemory(device(), _vertexBufferMemory, nullptr);



	_syncManager.destroySyncObjects(device());

	_commandManager.destroyCommandPool(device());

	_deviceManager.destroyDevice();

	if (enableValidationLayers) _debugManager.destroyDebugMessenger(instance());

	_surfaceManager.destroySurface(instance());
	_instanceManager.destroyInstance();
	_windowManager.destroyWindowAndTerminate();
}



