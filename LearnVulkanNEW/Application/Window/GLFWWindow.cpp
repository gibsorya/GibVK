#include "GLFWWindow.h";

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

//GLFWwindow* createWindow() {
//    glfwInit();
//
//    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
//    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
//
//    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
//
//    return window;
//}

GLFWwindow* VulkanWindow::createWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	VulkanWindow::window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

	return window;
}

GLFWwindow* VulkanWindow::getWindow() {
	return VulkanWindow::window;
}