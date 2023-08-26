#pragma once
#include "constants.h"
#include "Utils.h"
#include "ColorResources.h"
#include "DepthResources.h"

class SwapChainManager
{
public:

    SwapChainManager();
    ~SwapChainManager();

    // Also creates color and depth resources
    void createSwapChain(GLFWwindow* window, VkPhysicalDevice physicalDevice, 
        VkSurfaceKHR surface, VkDevice device, VkSampleCountFlagBits msaaSamples);
    
    void createImageViews(VkDevice device);

    void createFramebuffers(VkDevice device, VkRenderPass renderPass);

    void recreateSwapChain(GLFWwindow* window, VkDevice device, VkPhysicalDevice physicalDevice,
        VkSurfaceKHR surface, VkSampleCountFlagBits msaaSamples, VkRenderPass renderPass);
    void cleanupSwapChain(VkDevice device);

    // getters

    inline VkSwapchainKHR getSwapChain() const noexcept {return _swapChain;}

    inline std::vector<VkImage> getSwapChainImages() const noexcept {return _swapChainImages;}

    inline VkFormat getSwapChainImageFormat() const noexcept {return _swapChainImageFormat;}

    inline VkExtent2D getSwapChainExtent() const noexcept {return _swapChainExtent;}

    inline std::vector<VkImageView> getSwapChainImageViews() const noexcept {return _swapChainImageViews;}

    inline std::vector<VkFramebuffer> getSwapChainFrameBuffers() const noexcept{ return _swapChainFramebuffers; }

    

private:
    VkSwapchainKHR _swapChain;
    VkFormat _swapChainImageFormat;
    VkExtent2D _swapChainExtent;
    std::vector<VkImage> _swapChainImages;
    std::vector<VkImageView> _swapChainImageViews;

    ColorResources colorResources;
    DepthResources depthResources;

    // created separately, since it needs additional info not available
    // as soon. 
    std::vector<VkFramebuffer> _swapChainFramebuffers;
	
};

