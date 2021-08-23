#ifndef PHYSICAL_DEVICES_CLASS
#define PHYSICAL_DEVICES_CLASS

#include <vulkan/vulkan.hpp>
#include <vector>

namespace gibvk::vulkan::devices {
	class PhysicalDevices {
	public:
		PhysicalDevices();

		bool isDeviceSuitable(vk::PhysicalDevice device);

		[[nodiscard]] const vk::PhysicalDevice& getPhysicalDevice() const;

	private:
		vk::PhysicalDevice physicalDevice = VK_NULL_HANDLE;
	};

	std::unique_ptr<PhysicalDevices> pickPhysicalDevice();
}

#endif