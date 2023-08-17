#ifndef __BASE_APPLICATION__
#define __BASE_APPLICATION__

/// <summary>
/// The BaseApp class provides the "bare-bones" initilization and setup
/// of an application using the Vulkan API. The expected use of this class 
/// is to create an instance and invoke run().
/// </summary>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <set>

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#include "Vertex.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
};

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

    virtual void createGraphicsPipeline();

    virtual void createFramebuffers();

    virtual void createCommandPool();

    // TODO: Use only one VkBuffer to store the buffer and use offsets.
    // Reason being is that they are continuous within memory
    virtual void createVertexBuffer();
    virtual void createIndexBuffer();

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

    uint16_t _WINDOW_WIDTH;
    uint16_t _WINDOW_HEIGHT;
    GLFWwindow* _window;
    VkInstance _instance;
    VkSurfaceKHR _surface;
    VkQueue _presentQueue;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice _physicalDevice;
    VkDevice _device;
    VkQueue _graphicsQueue;
    VkSwapchainKHR _swapChain;
    std::vector<VkImage> _swapChainImages;
    VkFormat _swapChainImageFormat;
    VkExtent2D _swapChainExtent;
    std::vector<VkImageView> _swapChainImageViews;
    VkRenderPass _renderPass;
    VkPipelineLayout _pipelineLayout;
    VkPipeline _graphicsPipeline;
    std::vector<VkFramebuffer> _swapChainFramebuffers;
    VkCommandPool _commandPool;
    VkBuffer _vertexBuffer;
    VkDeviceMemory _vertexBufferMemory;
    VkBuffer _indexBuffer;
    VkDeviceMemory _indexBufferMemory;


    std::vector<VkCommandBuffer> _commandBuffers;
    std::vector<VkSemaphore> _imageAvailableSemaphores;
    std::vector<VkSemaphore> _renderFinishedSemaphores;
    std::vector<VkFence> _inFlightFences;

    bool _framebufferResized;

    
};

#endif //!__BASE_APPLICATION


