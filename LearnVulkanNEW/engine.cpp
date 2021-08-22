#include "engine.h"
#include "Application/HelloTriangle.h"
#include <iostream>
HelloTriangle application;

void VulkanEngine::run() {
	std::cout << "Hello World" << std::endl;
	application.initWindow();
	application.initVulkan();
	application.mainLoop();
	application.cleanUp();
}