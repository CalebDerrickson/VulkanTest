#pragma once

#include "constants.h"
#include "Utils.h"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>


class SurfaceManager
{
public:

	SurfaceManager(GLFWwindow* window, VkInstance instance);
	~SurfaceManager();

	SurfaceManager(const SurfaceManager&) = delete;
	SurfaceManager& operator=(const SurfaceManager&) = delete;

	//getters
	VkSurfaceKHR getSurface() { return _surface; }

private:

	void createSurface(GLFWwindow* window, VkInstance instance);

private:

	VkSurfaceKHR _surface;
};

