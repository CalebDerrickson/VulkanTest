#ifndef HELLO__TRIANGLE__APPLICATION__H
#define HELLO__TRIANGLE__APPLICATION__H



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

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;



#ifdef NDEBUG 
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class HelloTriangleApplication {
public:

    // Explicit definition of Run
    void run() 
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private: // Private Functions
    
    // Initializes the Main Window without a call to OpenGL
    void initWindow();

    // Initializes Vulkan
    void initVulkan();
    

    void createInstance();


    void createSurface();


    void setupDebugMessenger();


    void pickPhysicalDevice();


    void createLogicalDevice();

    
    void createSwapChain();


    void createImageViews();

    // Description of the number of framebuffer attachments we will be rendering.
    // Ie, we need to specify how many color and depth buffers there will be,
    // how many samples to use for each of them, 
    // and how their contents should be handled throughout the rendering operations.

    void createRenderPass();

    void createGraphicsPipeline();

    // Main Loop of the Program
    void mainLoop();

    // Memory Release 
    void cleanup();

    

private: // Private Values

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
};


#endif