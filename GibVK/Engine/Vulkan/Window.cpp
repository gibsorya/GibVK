#include "Window.hpp"
#include "../Graphics.hpp"
namespace gibvk::vulkan {
	VulkanWindow::VulkanWindow(uint32_t width, uint32_t height, const std::string& title)
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, graphics::get()->framebufferResizedCallback);
	}
	GLFWwindow* VulkanWindow::getWindow() const
	{
		return window;
	}
	GLFWwindow* VulkanWindow::getWindow()
	{
		return window;
	}
	std::unique_ptr<VulkanWindow> create(uint32_t width, uint32_t height, const std::string& title)
	{
		return std::make_unique<VulkanWindow>(width, height, title);
	}
}