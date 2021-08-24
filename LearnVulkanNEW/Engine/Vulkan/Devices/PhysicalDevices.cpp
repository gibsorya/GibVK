#include "PhysicalDevices.hpp"
#include "../../Graphics.hpp"
#include "QueueFamilies.hpp"

namespace gibvk::vulkan::devices {
	PhysicalDevices::PhysicalDevices()
	{
		uint32_t deviceCount = 0;
		graphics::get()->getInstance().getInstance().enumeratePhysicalDevices(&deviceCount, nullptr);

		if (deviceCount == 0) {
			throw std::runtime_error("Failed to find GPU's with Vulkan support!");
		}

		std::vector<vk::PhysicalDevice> physicalDevices(deviceCount);
		graphics::get()->getInstance().getInstance().enumeratePhysicalDevices(&deviceCount, physicalDevices.data());

		for (const auto& device : physicalDevices) {
			if (isDeviceSuitable(device)) {
				physicalDevice = device;
				break;
			}
		}

		if (&physicalDevice == VK_NULL_HANDLE) {
			throw std::runtime_error("Failed to find a suitable GPU!");
		}
	}

	bool PhysicalDevices::isDeviceSuitable(vk::PhysicalDevice device)
	{
		QueueFamilyIndices indices = devices::QueueFamilies::findQueueFamilies(device);

		return indices.isComplete();
	}

	const vk::PhysicalDevice& PhysicalDevices::getPhysicalDevice() const
	{
		return physicalDevice;
	}

	std::unique_ptr<PhysicalDevices> pickPhysicalDevice()
	{
		return std::make_unique<PhysicalDevices>();
	}
}