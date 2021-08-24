#include "LogicalDevices.hpp"
#include "../../Graphics.hpp"

namespace gibvk::vulkan::devices {
	LogicalDevices::LogicalDevices()
	{
		float queuePriority = 1.0f;

		QueueFamilyIndices indices = devices::QueueFamilies::findQueueFamilies(graphics::get()->getPhysicalDevice().getPhysicalDevice());

		vk::DeviceQueueCreateInfo queueCreateInfo({}, indices.graphicsFamily.value(), 1, &queuePriority);

		vk::PhysicalDeviceFeatures deviceFeatures{};

		vk::DeviceCreateInfo createInfo({}, 1, &queueCreateInfo, 0, nullptr, 0, nullptr, &deviceFeatures);

		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(vulkan::validlayers::validationLayers.size());
			createInfo.ppEnabledLayerNames = vulkan::validlayers::validationLayers.data();
		}

		if (graphics::get()->getPhysicalDevice().getPhysicalDevice().createDevice(&createInfo, nullptr, &device) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create logical device!");
		}

		//vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, reinterpret_cast<VkQueue>(*graphics::get()->getGraphicsQueue().getGraphicsQueue()));
		vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, const_cast<VkQueue*>(reinterpret_cast<const VkQueue*>(&graphics::get()->getGraphicsQueue().getGraphicsQueue())));
	}	

	const vk::Device& LogicalDevices::getLogicalDevice() const
	{
		return device;
	}

	const vk::Queue& LogicalDevices::getGraphicsQueue() const
	{
		return graphicsQueue;
	}

	std::unique_ptr<LogicalDevices> createLogicalDevice()
	{
		return std::make_unique<LogicalDevices>();
	}
}