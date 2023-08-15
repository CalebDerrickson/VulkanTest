#ifndef __BASE_APPLICATION__
#define __BASE_APPLICATION__

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

#ifdef NDEBUG 
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


class BaseApp
{
public:
    
    void run()
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

protected:

    // Initializes the Main Window without a call to OpenGL
    virtual void initWindow();

    // Initializes Vulkan
    virtual void initVulkan();


    virtual void createInstance();


    virtual void createSurface();


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

    virtual void createCommandBuffer();

    virtual void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    virtual void createSyncObjects();

    // Main Loop of the Program
    // This should be overwritten by child classes
    virtual void mainLoop();

    // The Draw Frame function is done by child classes

    // Memory Release 
    virtual void cleanup();

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
    VkCommandBuffer _commandBuffer;
    VkSemaphore _imageAvailableSemaphore;
    VkSemaphore _renderFinishedSemaphore;
    VkFence _inFlightFence;


};

#endif //!__BASE_APPLICATION