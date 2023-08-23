#pragma once

#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#define VK_USE_PLATFORM_WIN32_KHR

#include <cstdint>
#include <iostream>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>



class WindowManager
{
public:
	WindowManager(uint16_t width = 800, uint16_t height = 600, const char* windowName = "Default Window Name");
	~WindowManager();

	WindowManager(const WindowManager&) = delete;
	WindowManager& operator=(const WindowManager&) = delete;

	//setters
	void setFramebufferResized(bool framebufferResized) { _framebufferResized = framebufferResized; }

	//getters
	GLFWwindow* getWindow() const { return _window; }
	bool getFramebufferResized() const { return _framebufferResized; }

private:

	void initWindow();

	uint16_t _windowWidth;
	uint16_t _windowHeight;
	const char* _windowName;
	bool _framebufferResized;


	GLFWwindow* _window;
};

static void framebufferResizeCallback(GLFWwindow* window, int width, int height);