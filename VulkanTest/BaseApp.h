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
#include "CommandManager.h"
#include "TextureManager.h"
#include "SyncManager.h"
#include "DescriptorManager.h"
#include "UniformBufferManager.h"
#include "BufferObject.h"

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

    // should initVulkan be a protected function or private?
    virtual void initVulkan();


    // To be moved to their own classes
    virtual void loadModel();


    // Main Loop of the Program
    // This should be overwritten by child classes
    virtual void mainLoop();

    // The Draw Frame function is done by child classes

    // Memory Release 
    virtual void cleanup();

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
   
    CommandManager _commandManager;

    TextureManager _textureManager;

    SyncManager _syncManager;

    // TODO: Use only one VkBuffer to store the buffer and use offsets.
    // Reason being is that they are continuous within memory, so 
    // easier to access

    BufferObject<Vertex> _vertices;
    BufferObject<uint32_t> _indices;

    DescriptorManager _descriptorManager;
  
    UniformBufferManager _uniformBufferManager;

};




