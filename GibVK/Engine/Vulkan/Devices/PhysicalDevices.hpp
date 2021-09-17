#ifndef PHYSICAL_DEVICES_CLASS
#define PHYSICAL_DEVICES_CLASS

#include <vulkan/vulkan.hpp>
#include <vector>
#include <set>

namespace gibvk::vulkan::devices {
	class PhysicalDevices {
	public:
		PhysicalDevices();

		bool isDeviceSuitable(vk::PhysicalDevice device);

		bool checkDeviceExtensionSupport(vk::PhysicalDevice device);

		vk::SampleCountFlagBits getMaxUsableSampleCount();

		[[nodiscard]] const vk::PhysicalDevice& getPhysicalDevice() const;

	private:
		vk::PhysicalDevice physicalDevice = VK_NULL_HANDLE;
	};

	std::unique_ptr<PhysicalDevices> pickPhysicalDevice();
}

#endif