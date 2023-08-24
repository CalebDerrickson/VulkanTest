#include "SurfaceManager.h"

SurfaceManager::SurfaceManager(GLFWwindow* window, VkInstance instance)
{

}

SurfaceManager::~SurfaceManager()
{
	// How do I destroy?
	// vkDestroySurfaceKHR(, _surface, nullptr);
}

void SurfaceManager::createSurface(GLFWwindow* window, VkInstance instance)
{

	// App can only be used on Windows 
	// TODO : Query for device architecture
	// and modify as such
	VkWin32SurfaceCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = glfwGetWin32Window(window);
	createInfo.hinstance = GetModuleHandle(nullptr);

	if (glfwCreateWindowSurface(instance, window, nullptr, &_surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}

}