#include "LogicalDevices.hpp"
#include "../../Graphics.hpp"
#include <set>

namespace gibvk::vulkan::devices {
	LogicalDevices::LogicalDevices()
	{
		float queuePriority = 1.0f;

		QueueFamilyIndices indices = devices::QueueFamilies::findQueueFamilies(graphics::get()->getPhysicalDevice().getPhysicalDevice());

		std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamiles = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		for (uint32_t queueFamily : uniqueQueueFamiles) {
			vk::DeviceQueueCreateInfo queueCreateInfo({}, queueFamily, 1, &queuePriority);
			queueCreateInfos.push_back(queueCreateInfo);
		}
		
		vk::PhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.samplerAnisotropy = VK_TRUE;
		deviceFeatures.sampleRateShading = VK_TRUE;

		vk::DeviceCreateInfo createInfo({}, static_cast<uint32_t>(queueCreateInfos.size()), queueCreateInfos.data(), 0, nullptr, static_cast<uint32_t>(swapchains::deviceExtensions.size()), swapchains::deviceExtensions.data(), &deviceFeatures);
		
		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(vulkan::validlayers::validationLayers.size());
			createInfo.ppEnabledLayerNames = vulkan::validlayers::validationLayers.data();
		}

		if (graphics::get()->getPhysicalDevice().getPhysicalDevice().createDevice(&createInfo, nullptr, &device) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create logical device!");
		}

		vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, const_cast<VkQueue*>(reinterpret_cast<const VkQueue*>(&graphics::get()->getGraphicsQueue().getGraphicsQueue())));
		vkGetDeviceQueue(device, indices.presentFamily.value(), 0, const_cast<VkQueue*>(reinterpret_cast<const VkQueue*>(&graphics::get()->getPresentQueue().getPresentQueue())));
	}	

	const vk::Device& LogicalDevices::getLogicalDevice() const
	{
		return device;
	}

	std::unique_ptr<LogicalDevices> createLogicalDevice()
	{
		return std::make_unique<LogicalDevices>();
	}
}