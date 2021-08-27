#ifndef DEBUG_UTILS_CLASS
#define DEBUG_UTILS_CLASS

#include <vulkan/vulkan.hpp>

namespace gibvk::vulkan::debugutils {

	VkResult CreateDebugUtilsMessengerEXT(vk::Instance instance, const vk::DebugUtilsMessengerCreateInfoEXT* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::DebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(vk::Instance instance, vk::DebugUtilsMessengerEXT debugMessenger, const vk::AllocationCallbacks* pAllocator);

	class DebugUtils {
	public:
		DebugUtils();

		[[nodiscard]] const vk::DebugUtilsMessengerEXT& getDebugMessenger() const;

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

		static vk::DebugUtilsMessengerCreateInfoEXT populateDebugMessengerCreateInfo();

	private:
		vk::DebugUtilsMessengerEXT debugMessenger;
	};

	std::unique_ptr<DebugUtils> setupDebugMessenger();
}

#endif
