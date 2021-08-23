#include "Instance.hpp"
#include <GLFW/glfw3.h>

namespace gibvk::vulkan {
	VulkanInstance::VulkanInstance()
	{
		vk::ApplicationInfo appInfo("Hello Triangle", VK_MAKE_API_VERSION(1, 1, 0, 0), "Vulkan Engine", VK_MAKE_API_VERSION(1, 1, 0, 0), VK_API_VERSION_1_1);

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		vk::InstanceCreateInfo createInfo({}, &appInfo, 0, nullptr, glfwExtensionCount, glfwExtensions);

		if (vk::createInstance(&createInfo, nullptr, &instance) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create instance! :(");
		}
	}
	//void VulkanInstance::createInstance()
	//{
	//	
	//}
	const vk::Instance& VulkanInstance::getInstance() const
	{
		return instance;
	}

	std::unique_ptr<VulkanInstance> create()
	{
		return std::make_unique<VulkanInstance>();
	}
}