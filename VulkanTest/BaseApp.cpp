
#define TINYOBJLOADER_IMPLEMENTATION

#include "BaseApp.h"
#include "Utils.h"
extern const int MAX_FRAMES_IN_FLIGHT = 2;
extern const char* MODEL_PATH = "models/viking_room.obj";
extern const char* TEXTURE_PATH = "textures/viking_room.png";
uint32_t QueueFamilyIndices::InstanceCount = 0;

BaseApp::BaseApp()
	: _currentFrame(NULL)
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
	
	_descriptorManager.createDescriptorSetLayout(device());

	_graphicsPipelineManager.createGraphicsPipeline(device(), physicalDevice(), 
		_descriptorManager.getDescriptorSetLayout(), renderPass(),
		_physicalDeviceManager.getMsaaSamples());

	
	_swapChainManager.createFramebuffers(device(), renderPass());

	_commandManager.createCommandPool(device(), physicalDevice(), surface());

	_textureManager.createTextureImage(device(), physicalDevice(), commandPool(), _deviceManager.getGraphicsQueue());
	_textureManager.createTextureImageView(device());
	_textureManager.createTextureSampler(device(), physicalDevice());

	loadModel();

	_vertices.createVkBuffer( VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, physicalDevice(), device(), 
		_deviceManager.getGraphicsQueue(), _commandManager.getCommandPool());

	_indices.createVkBuffer( VK_BUFFER_USAGE_INDEX_BUFFER_BIT, physicalDevice(), device(), 
		_deviceManager.getGraphicsQueue(), _commandManager.getCommandPool());

	_uniformBufferManager.createUniformBuffers(device(), physicalDevice());
	_descriptorManager.createDescriptorPool(device());
	_descriptorManager.createDescriptorSets(device(), _uniformBufferManager.getUniformBuffers(), _textureManager);

	_commandManager.createCommandBuffers(device());

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
				uniqueVertices[vertex] = static_cast<uint32_t>(_vertices.bufferObject.size());
				_vertices.bufferObject.push_back(vertex);
			}
			_indices.bufferObject.push_back(uniqueVertices[vertex]);
		}
	}

}

void BaseApp::mainLoop()
{
	
	while (!glfwWindowShouldClose(window())) {
		glfwPollEvents();
	}

	vkDeviceWaitIdle(device());
}

void BaseApp::cleanup()
{
	
	_swapChainManager.cleanupSwapChain(device());

	_graphicsPipelineManager.destroyGraphicsPipelineAndLayout(device());

	_renderPassManager.destroyRenderPass(device());

	_uniformBufferManager.destroyUniformBuffers(device());

	_descriptorManager.destroyDescriptorPool(device());

	_textureManager.destroyTexture(device());

	_descriptorManager.destroyDescriptorSetLayout(device());
	

	_indices.destroyBuffer(device());
	_vertices.destroyBuffer(device());

	_syncManager.destroySyncObjects(device());

	_commandManager.destroyCommandPool(device());

	_deviceManager.destroyDevice();

	if (enableValidationLayers) _debugManager.destroyDebugMessenger(instance());

	_surfaceManager.destroySurface(instance());
	_instanceManager.destroyInstance();
	_windowManager.destroyWindowAndTerminate();
}



