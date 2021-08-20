#include <iostream>
#include "HelloTriangle.h"

void HelloTriangle::initWindow() {
	window.createWindow();
	std::cout << "Initialized Window!" << std::endl;
}

void HelloTriangle::initVulkan() {
	instance.createInstance();
	std::cout << "Initialized Vulkan!" << std::endl;
}

void HelloTriangle::mainLoop() {
	std::cout << "Started main loop!" << std::endl;

	while (!glfwWindowShouldClose(window.getWindow())) {
		glfwPollEvents();
	}

	std::cout << "Ended main loop!" << std::endl;
}

void HelloTriangle::cleanUp() {
	std::cout << "Cleaning up..." << std::endl;

	instance.getInstance().destroy();

	glfwDestroyWindow(window.getWindow());
	glfwTerminate();
}