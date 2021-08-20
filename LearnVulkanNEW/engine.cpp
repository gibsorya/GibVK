#include "engine.h"
#include "Application/HelloTriangle.h"
#include <iostream>
HelloTriangle application;

void VulkanEngine::run() {
	application.initWindow();
	application.initVulkan();
	application.mainLoop();
	application.cleanUp();

	std::cout << "Hello World" << std::endl;
}