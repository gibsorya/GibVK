#ifndef VULKAN_WINDOW_CLASS
#define VULKAN_WINDOW_CLASS
#include <GLFW/glfw3.h>
#define GLFW_INCLUDE_VULKAN



class VulkanWindow {
public:
	GLFWwindow* createWindow();

	GLFWwindow* getWindow();
private:
	GLFWwindow* window;
};

#endif