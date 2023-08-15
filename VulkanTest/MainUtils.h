#ifndef __MAIN_UTILS__
#define __MAIN_UTILS__


#include <iostream>
#include <fstream>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <optional>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <set>
#include <string>
#include <vulkan/vk_enum_string_helper.h>

#ifdef NDEBUG 
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};


#include "CommonUtils.h"
#include "DebugUtils.h"
#include "DeviceUtils.h"
#include "InstanceUtils.h"
#include "SwapChainUtils.h"
#include "ShaderUtils.h"


#endif // !__MAIN_UTILS__
