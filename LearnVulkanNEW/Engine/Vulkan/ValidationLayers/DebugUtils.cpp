#include "DebugUtils.hpp"
#include "ValidLayers.hpp"
#include "../../Graphics.hpp"

namespace gibvk::vulkan::debugutils {
	DebugUtils::DebugUtils()
	{
		if (!enableValidationLayers) return;

		auto createInfo = populateDebugMessengerCreateInfo();

		if (CreateDebugUtilsMessengerEXT(graphics::get()->getInstance().getInstance(), &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
			throw std::runtime_error("Failed to set up debug messenger!");
		}
	}

	const vk::DebugUtilsMessengerEXT& DebugUtils::getDebugMessenger() const
	{
		return debugMessenger;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtils::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{

		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;

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

	VkResult CreateDebugUtilsMessengerEXT(vk::Instance instance, const vk::DebugUtilsMessengerCreateInfoEXT* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::DebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func{ reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance.getProcAddr("vkCreateDebugUtilsMessengerEXT")) };

		if (func != nullptr) {
			return func(instance, reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDebugUtilsMessengerEXT*>(pDebugMessenger));
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(vk::Instance instance, vk::DebugUtilsMessengerEXT debugMessenger, const vk::AllocationCallbacks* pAllocator)
	{
		auto func{ reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT")) };

		if (func != nullptr) {
			func(instance, debugMessenger, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
		}
	}

	std::unique_ptr<DebugUtils> setupDebugMessenger()
	{
		return std::make_unique<DebugUtils>();
	}
}