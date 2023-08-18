#ifndef HELLO__TRIANGLE__APPLICATION__H
#define HELLO__TRIANGLE__APPLICATION__H

#include "BaseApp.h"


class HelloTriangleApplication : public BaseApp {

public:

    HelloTriangleApplication(){}

    ~HelloTriangleApplication(){}

private: // Private Functions

    // Main Loop of the Program
    void mainLoop() override;

    void drawFrame();

    void updateUniformBuffer(uint32_t currentImage);
    

private: // Private Values
    


};


#endif