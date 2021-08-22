#include "instance.h"
#include <GLFW/glfw3.h>

#define GLFW_INCLUDE_VULKAN

ValidLayers validLayers;

void VulkanInstance::createInstance() {
	if (validLayers.enableValidationLayers && !validLayers.checkValidationLayerSupport()) {
		throw std::runtime_error("Validation layers requested, but none are available!");
	}
	//vk::ApplicationInfo appInfo("Hello Triangle", 1, "Vulkan Engine", 1, VK_API_VERSION_1_1);
	vk::ApplicationInfo appInfo("Hello Triangle", VK_MAKE_API_VERSION(1, 1, 0, 0), "Vulkan Engine", VK_MAKE_API_VERSION(1, 1, 0, 0), VK_API_VERSION_1_1);

	auto extensions = getRequiredExtensions();

	vk::InstanceCreateInfo createInfo({}, &appInfo, 0, nullptr, static_cast<uint32_t>(extensions.size()), extensions.data());

	vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo = DebugUtils::populateDebugMessengerCreateInfo();

	if (validLayers.enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validLayers.validationLayers.size());
		createInfo.ppEnabledLayerNames = validLayers.validationLayers.data();
		createInfo.pNext = static_cast<vk::DebugUtilsMessengerCreateInfoEXT*>(&debugCreateInfo);
	}

	/*vk::Result result = */

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

	if (validLayers.enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

//vk::Instance VulkanInstance::getInstance() {
//	return VulkanInstance::instance;
//}

