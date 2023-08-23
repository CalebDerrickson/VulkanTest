#pragma once
#include <vector>

extern const int MAX_FRAMES_IN_FLIGHT;
extern const char* MODEL_PATH;
extern const char* TEXTURE_PATH;

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
	//Enable Best Practices vaildation in the future
};