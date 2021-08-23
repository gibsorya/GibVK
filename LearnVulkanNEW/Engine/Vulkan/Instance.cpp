#include "Instance.hpp"
#include <GLFW/glfw3.h>

namespace gibvk::vulkan {
	VulkanInstance::VulkanInstance()
	{
		if (enableValidationLayers && !validlayers::ValidLayers::checkValidationLayerSupport()) {
			throw std::runtime_error("Validation layers requested, but none are available!");
		}
		vk::ApplicationInfo appInfo("Hello Triangle", VK_MAKE_API_VERSION(1, 1, 0, 0), "Vulkan Engine", VK_MAKE_API_VERSION(1, 1, 0, 0), VK_API_VERSION_1_1);

		auto extensions = getRequiredExtensions();
		vk::InstanceCreateInfo createInfo({}, &appInfo, 0, nullptr, static_cast<uint32_t>(extensions.size()), extensions.data());

		//vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validlayers::validationLayers.size());
			createInfo.ppEnabledLayerNames = validlayers::validationLayers.data();

			auto debugCreateInfo = vulkan::debugutils::DebugUtils::populateDebugMessengerCreateInfo();
			createInfo.pNext = static_cast<vk::DebugUtilsMessengerCreateInfoEXT*>(&debugCreateInfo);
		}

		if (vk::createInstance(&createInfo, nullptr, &instance) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create instance! :(");
		}
	}
	std::vector<const char*> VulkanInstance::getRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}
	//void VulkanInstance::createInstance()
	//{
	//	
	//}
	const vk::Instance& VulkanInstance::getInstance() const
	{
		return instance;
	}

	std::unique_ptr<VulkanInstance> createInstance()
	{
		return std::make_unique<VulkanInstance>();
	}
}