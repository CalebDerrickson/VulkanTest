#ifndef HELLO__TRIANGLE__APPLICATION__H
#define HELLO__TRIANGLE__APPLICATION__H

#include "BaseApp.h"



class HelloTriangleApplication : public BaseApp {

public:

    HelloTriangleApplication() : _currentFrame(0)
    {}

    ~HelloTriangleApplication(){}

private: // Private Functions

    // Main Loop of the Program
    void mainLoop() override;

    void drawFrame();


    

private: // Private Values
    
    uint32_t _currentFrame;

};


#endif