#pragma once
#include <cstdint>
#include <iostream>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	WindowManager(const WindowManager&) = delete;
	WindowManager& operator=(const WindowManager&) = delete;

	//setters
	void setFramebufferResized(bool framebufferResized) { _framebufferResized = framebufferResized; }

	//getters
	inline GLFWwindow* getWindow() const noexcept { return _window; }
	inline bool getFramebufferResized() const noexcept{ return _framebufferResized; }

	void initWindow(uint16_t width = 800, uint16_t height = 600, const char* windowName = "Default Window Name");

	void destroyWindowAndTerminate();

private:

	

	uint16_t _windowWidth;
	uint16_t _windowHeight;
	const char* _windowName;
	bool _framebufferResized;


	GLFWwindow* _window;
};

// Function to detect window resizes
static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
