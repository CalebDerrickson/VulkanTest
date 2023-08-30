#pragma once

#include "constants.h"
#include "Utils.h"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>


class SurfaceManager
{
public:

	SurfaceManager();
	~SurfaceManager();

	SurfaceManager(const SurfaceManager&) = delete;
	SurfaceManager& operator=(const SurfaceManager&) = delete;

	//getters
	inline VkSurfaceKHR getSurface() const noexcept { return _surface; }

	void createSurface(GLFWwindow* window, VkInstance instance);
	void destroySurface(VkInstance instance);

	
private:

	VkSurfaceKHR _surface;
};

