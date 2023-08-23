#include "WindowManager.h"


WindowManager::WindowManager(uint16_t width, uint16_t height, const char* windowName) :
	_windowWidth(width), _windowHeight(height), _windowName(windowName)
{
	initWindow();
}

WindowManager::~WindowManager()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void WindowManager::initWindow()
{
	std::cout << "Initializing Window" << std::endl;
	// Initialize GLFW
	glfwInit();

	// Tells GLFW not to create an OpenGL context with next call
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Create Window

	_window = glfwCreateWindow(
		_windowWidth, _windowHeight,	// Width, Height
		_windowName,					// Title
		nullptr,						// Which Monitor to display
		nullptr							// Relevant to only OpenGL
	);


	glfwSetWindowUserPointer(_window, this);
	glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);

	std::cout << "Window Initialized." << std::endl;
}



// Function to detect window resizes
static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	app->setFramebufferResized(true);
}