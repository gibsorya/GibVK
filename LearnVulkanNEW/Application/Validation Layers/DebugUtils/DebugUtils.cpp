#include "DebugUtils.h"
#include "../ValidationLayers.h"
#include "../../Instance/instance.h"

VulkanInstance* instance;

VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtils::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageSeverityFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

void DebugUtils::setupDebugMessenger() {
	if (!ValidLayers::enableValidationLayers) {
		return;
	}

	vk::DebugUtilsMessengerCreateInfoEXT createInfo = populateDebugMessengerCreateInfo();
	std::cout << "Hello World" << std::endl;

	if (createDebugUtilsMessengerEXT(instance->instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("Failed to set up debug messenger!");
	}
}

VkResult DebugUtils::createDebugUtilsMessengerEXT(vk::Instance instance, vk::DebugUtilsMessengerCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, vk::DebugUtilsMessengerEXT* pCallback) {
	auto func{ reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance.getProcAddr("vkCreateDebugUtilsMessengerEXT")) };

	if (func != nullptr) {
		return func(instance, reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT*>(pCreateInfo), reinterpret_cast<VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDebugUtilsMessengerEXT*>(pCallback));
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DebugUtils::destroyDebugUtilsMessengerEXT(vk::Instance instance, vk::DebugUtilsMessengerEXT debugMessenger, vk::AllocationCallbacks* pAllocator)
{
	auto func{ reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT")) };

	if (func != nullptr) {
		func(instance, debugMessenger, reinterpret_cast<VkAllocationCallbacks*>(pAllocator));
	}
}

vk::DebugUtilsMessengerCreateInfoEXT DebugUtils::populateDebugMessengerCreateInfo()
{
	auto createInfo = vk::DebugUtilsMessengerCreateInfoEXT(
		{},
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
		vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
		debugCallback,
		nullptr
	);
	return createInfo;
}


//vk::DebugUtilsMessengerEXT DebugUtils::getDebugMessenger()
//{
//	return DebugUtils::debugMessenger;
//}





