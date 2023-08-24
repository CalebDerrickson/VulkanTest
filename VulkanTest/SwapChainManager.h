#pragma once
#include "constants.h"
#include "Utils.h"

class SwapChainManager
{
public:

    SwapChainManager();
    ~SwapChainManager();

    void createSwapChain(GLFWwindow* window, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkDevice device);
    void createImageViews(VkDevice device);

    void createFramebuffers();

private:
    VkSwapchainKHR _swapChain;
    std::vector<VkImage> _swapChainImages;
    VkFormat _swapChainImageFormat;
    VkExtent2D _swapChainExtent;
    std::vector<VkImageView> _swapChainImageViews;

    // created separately, since it needs additional info not available
    // as soon. 
    std::vector<VkFramebuffer> _swapChainFramebuffers;
	
};

