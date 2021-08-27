#ifndef VULKAN_WINDOW_CLASS
#define VULKAN_WINDOW_CLASS

#include <GLFW/glfw3.h>
#include <string>
#include <memory>

namespace gibvk::vulkan {
	class VulkanWindow {
	public:
		VulkanWindow(uint32_t width, uint32_t height, const std::string& title);
		[[nodiscard]] GLFWwindow* getWindow() const;
		GLFWwindow *getWindow();
	private:
		GLFWwindow *window;
	};

	std::unique_ptr<VulkanWindow> create(uint32_t width, uint32_t height, const std::string& title);
}
#endif