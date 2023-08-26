#pragma once
#include "constants.h"
#include "Utils.h"
#include <stb_image.h>
struct TextureManager
{
    //Functions
    
    TextureManager();
    ~TextureManager();

    void createTextureImage(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue);
    void createTextureImageView(VkDevice device);
    void createTextureSampler(VkDevice device, VkPhysicalDevice physicalDevice);

    void destroyTexture(VkDevice device);

    //Values

    uint32_t mipLevels;
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;

    VkImageView textureImageView;
    VkSampler textureSampler;
};

