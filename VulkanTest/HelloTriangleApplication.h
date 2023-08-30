#pragma once

#include "BaseApp.h"


class HelloTriangleApplication : public BaseApp{

public:

    HelloTriangleApplication() : BaseApp() {}

    ~HelloTriangleApplication(){}

    void run()
    {
        initVulkan();
        mainLoop();
        cleanup();
    }

private: // Private Functions

    // Main Loop of the Program
    void mainLoop();

    void drawFrame();

    void updateUniformBuffer(uint32_t currentImage);
    


};


