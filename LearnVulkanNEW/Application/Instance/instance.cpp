#include "instance.h";
#include <GLFW/glfw3.h>

#define GLFW_INCLUDE_VULKAN

void VulkanInstance::createInstance() {
	//vk::ApplicationInfo appInfo("Hello Triangle", 1, "Vulkan Engine", 1, VK_API_VERSION_1_1);
	vk::ApplicationInfo appInfo("Hello Triangle", VK_MAKE_API_VERSION(1, 1, 0, 0), "Vulkan Engine", VK_MAKE_API_VERSION(1, 1, 0, 0), VK_API_VERSION_1_1);

	

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	vk::InstanceCreateInfo createInfo({}, &appInfo, 0);

	/*vk::Result result = */

	if (vk::createInstance(&createInfo, nullptr, &instance) != vk::Result::eSuccess) {
		throw std::runtime_error("Failed to create instance! :(");
	}
}

vk::Instance VulkanInstance::getInstance() {
	return VulkanInstance::instance;
}