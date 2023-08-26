#pragma once

#include "BaseApp.h"


class HelloTriangleApplication : public BaseApp {

public:

    HelloTriangleApplication() : BaseApp() {}

    ~HelloTriangleApplication(){}

private: // Private Functions

    // Main Loop of the Program
    void mainLoop() override;

    void drawFrame();

    void updateUniformBuffer(uint32_t currentImage);
    

private: // Private Values
    


};


