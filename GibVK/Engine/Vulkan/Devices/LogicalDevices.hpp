#ifndef LOGICAL_DEVICES_CLASS
#define LOGICAL_DEVICES_CLASS

#include <vulkan/vulkan.hpp>

namespace gibvk::vulkan::devices {
	class LogicalDevices {
	public:
		LogicalDevices();

		[[nodiscard]] const vk::Device& getLogicalDevice() const;

	private:
		vk::Device device;
	};

	std::unique_ptr<LogicalDevices> createLogicalDevice();
}

#endif // !LOGICAL_DEVICES_CLASS
