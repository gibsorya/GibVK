#ifndef DEVICES_CLASS
#define DEVICES_CLASS

#include "PhysicalDevices.hpp"
#include "QueueFamilies.hpp"
#include "../../Graphics.hpp"

#include <memory>

namespace gibvk::vulkan::devices {
	class Devices {
	public:
		Devices() = default;
		Devices(bool enabledValidationLayers = true);

		static Devices* _get();

		[[nodiscard]] const devices::PhysicalDevices& getPhysicalDevice() const;
		[[nodiscard]] const devices::QueueFamilies& getQueueFamilies() const;

	private:
		static std::unique_ptr<Devices> devices;

		std::unique_ptr<devices::PhysicalDevices> physicalDevices{};
		std::unique_ptr<devices::QueueFamilies> queueFamilies{};
	};

	Devices* get();


}

#endif // !DEVICES_CLASS
