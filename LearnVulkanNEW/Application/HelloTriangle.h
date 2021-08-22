#ifndef HELLO_TRIANGLE_CLASS
#define HELLO_TRIANGLE_CLASS

#include "Window/GLFWWindow.h"
#include "Instance/instance.h"
#include "Validation Layers/ValidationLayers.h"

class HelloTriangle {
public:
	void initWindow();

	void initVulkan();

	void mainLoop();

	void cleanUp();

private:
	VulkanWindow window;
	VulkanInstance instance;
	ValidLayers layers;
	DebugUtils debugUtils;
};

#endif