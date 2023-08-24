#pragma once
#include "constants.h"
#include "Utils.h"

class DebugManager
{
public:
	DebugManager();
	~DebugManager();

	void setupDebugMessenger(VkInstance instance);
	void destroyDebugMessenger(VkInstance instance);

private:
	VkDebugUtilsMessengerEXT _debugMessenger;
};

