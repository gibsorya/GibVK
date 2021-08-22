#ifndef DEBUG_UTILS_CLASS
#define DEBUG_UTILS_CLASS

#include <vulkan/vulkan.hpp>
#include <iostream>

class DebugUtils {
public:
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageSeverityFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

	void setupDebugMessenger();

	//static vk::DebugUtilsMessengerEXT getDebugMessenger();

	VkResult createDebugUtilsMessengerEXT(vk::Instance instance, vk::DebugUtilsMessengerCreateInfoEXT* pCreateInfo, VkAllocationCallbacks* pAllocator, vk::DebugUtilsMessengerEXT* pCallback);

	void destroyDebugUtilsMessengerEXT(vk::Instance instance, vk::DebugUtilsMessengerEXT debugMessenger, vk::AllocationCallbacks* pAllocator);

	static vk::DebugUtilsMessengerCreateInfoEXT populateDebugMessengerCreateInfo();

	vk::DebugUtilsMessengerEXT debugMessenger;

private:
	
};

#endif
