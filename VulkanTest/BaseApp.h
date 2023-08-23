#pragma once
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <set>
#include <chrono>
#include <unordered_map>

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE


#include <stb_image.h>
#include <tiny_obj_loader.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#include "common/Vertex.h"
#include "common/QueueFamilyIndices.h"
#include <glm/gtc/matrix_transform.hpp>
#include "constants.h"





class BaseApp
{
public:
    
    BaseApp() = default;
    ~BaseApp() = default;
    
    void run()
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }


    //setters
    void setFramebufferResized(bool framebufferResized) { _framebufferResized = framebufferResized; }

protected:

    // Initializes the Main Window without a call to OpenGL
    virtual void initWindow();

    // Initializes Vulkan, 
    // I.e. runs all the commands
    // below in order
    virtual void initVulkan();

    // Provides a basic description of an application to
    // create an Instance
    virtual void createInstance();

    // App can only be used on Windows 
    // TODO : Query for device architecture
    // and modify as such
    virtual void createSurface();

    // Applies any validation layers and debugging applications
    // if provided
    virtual void setupDebugMessenger();

    
    virtual void pickPhysicalDevice();


    virtual void createLogicalDevice();


    virtual void createSwapChain();


    virtual void createImageViews();

    // Description of the number of framebuffer attachments we will be rendering.
    // Ie, we need to specify how many color and depth buffers there will be,
    // how many samples to use for each of them, 
    // and how their contents should be handled throughout the rendering operations.
    virtual void createRenderPass();

    virtual void createDescriptorSetLayout();

    virtual void createGraphicsPipeline();

    virtual void createFramebuffers();

    virtual void createCommandPool();

    virtual void createColorResources();

    virtual void createDepthResources();


    virtual void createTextureImage();
    virtual void createTextureImageView();
    virtual void createTextureSampler();

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
    void recreateSwapChain();

    // A separate cleanup function for the swap chain is necessary to
    // ensure proper handling of the objects inside the swap chain
    void cleanupSwapchain();

protected:

    uint32_t _currentFrame;
    uint16_t _WINDOW_WIDTH;
    uint16_t _WINDOW_HEIGHT;
    GLFWwindow* _window;
    VkInstance _instance;
    VkSurfaceKHR _surface;
    VkQueue _presentQueue;
    VkDebugUtilsMessengerEXT _debugMessenger;
    VkPhysicalDevice _physicalDevice;
    VkDevice _device;
    VkQueue _graphicsQueue;
    VkSwapchainKHR _swapChain;
    std::vector<VkImage> _swapChainImages;
    VkFormat _swapChainImageFormat;
    VkExtent2D _swapChainExtent;
    std::vector<VkImageView> _swapChainImageViews;
    VkRenderPass _renderPass;
    VkDescriptorSetLayout _descriptorSetLayout;
    VkPipelineLayout _pipelineLayout;
    VkPipeline _graphicsPipeline;
    std::vector<VkFramebuffer> _swapChainFramebuffers;
    VkCommandPool _commandPool;

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

    VkBuffer _stagingBuffer;
    VkDeviceMemory _stagingBufferMemory;

    uint32_t _mipLevels;
    VkImage _textureImage;
    VkDeviceMemory _textureImageMemory;

    VkImageView _textureImageView;
    VkSampler _textureSampler;

    VkImage _depthImage;
    VkDeviceMemory _depthImageMemory;
    VkImageView _depthImageView;

    // defined here as a default value. not the best practice, but
    // it will do for now. 
    VkSampleCountFlagBits _msaaSamples = VK_SAMPLE_COUNT_1_BIT;
    VkImage _colorImage;
    VkDeviceMemory _colorImageMemory;
    VkImageView _colorImageView;

    bool _framebufferResized;

    
};




