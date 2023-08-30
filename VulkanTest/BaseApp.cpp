


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

	MainUtils::loadModel(_indices.bufferObject, _vertices.bufferObject);

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



