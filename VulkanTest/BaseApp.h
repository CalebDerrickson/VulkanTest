#pragma once
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <set>
#include <chrono>
#include <unordered_map>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#define VK_USE_PLATFORM_WIN32_KHR


#include <tiny_obj_loader.h>

#include "WindowManager.h"
#include "InstanceManager.h"
#include "PhysicalDeviceManager.h"
#include "SurfaceManager.h"
#include "DeviceManager.h"
#include "DebugManager.h"
#include "SwapChainManager.h"
#include "RenderPassManager.h"
#include "GraphicsPipelineManager.h"
#include "CommandPoolManager.h"
#include "TextureManager.h"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <glm/gtc/matrix_transform.hpp>




class BaseApp
{
public:
    
    BaseApp();
    ~BaseApp();
    
    void run()
    {
        initVulkan();
        mainLoop();
        cleanup();
    }

    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;


protected:

    virtual void initVulkan();

    virtual void loadModel();
    // TODO: Use only one VkBuffer to store the buffer and use offsets.
    // Reason being is that they are continuous within memory
    virtual void createVertexBuffer();
    virtual void createIndexBuffer();
    virtual void createUniformBuffers();
    virtual void createDescriptorPool();
    virtual void createDescriptorSets();

    virtual void createCommandBuffers();

    virtual void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    virtual void createSyncObjects();

    // Main Loop of the Program
    // This should be overwritten by child classes
    virtual void mainLoop();

    // The Draw Frame function is done by child classes

    // Memory Release 
    virtual void cleanup();


    // Recreation of the Swapchain is sometimes necessary due to 
    // changes in the window surface (resizing is an example).
    // As such, we need a way to create a new swap chain.
    // void recreateSwapChain();

    // A separate cleanup function for the swap chain is necessary to
    // ensure proper handling of the objects inside the swap chain
    // void cleanupSwapchain();

protected:

    uint32_t _currentFrame;

    WindowManager _windowManager;

    InstanceManager _instanceManager;

    SurfaceManager _surfaceManager;

    PhysicalDeviceManager _physicalDeviceManager;

    DeviceManager _deviceManager;
    
    DebugManager _debugManager;
 
    SwapChainManager _swapChainManager;

    RenderPassManager _renderPassManager;

    GraphicsPipelineManager _graphicsPipelineManager;
   

    CommandPoolManager _commandPoolManager;

    TextureManager _textureManager;

    std::vector<Vertex> _vertices;
    std::vector<uint32_t> _indices;
    
    VkBuffer _vertexBuffer;
    VkDeviceMemory _vertexBufferMemory;

    VkBuffer _indexBuffer;
    VkDeviceMemory _indexBufferMemory;
    

    std::vector<VkCommandBuffer> _commandBuffers;
    std::vector<VkSemaphore> _imageAvailableSemaphores;
    std::vector<VkSemaphore> _renderFinishedSemaphores;
    std::vector<VkFence> _inFlightFences;

    std::vector<VkBuffer> _uniformBuffers;
    std::vector<VkDeviceMemory> _uniformBuffersMemory;
    std::vector<void*> _uniformBuffersMapped;

    VkDescriptorPool _descriptorPool;
    std::vector<VkDescriptorSet> _descriptorSets;

    // VkBuffer _stagingBuffer;
    // VkDeviceMemory _stagingBufferMemory;
};




