#include "Devices.hpp"

namespace gibvk::vulkan::devices {
	Devices::Devices(bool enabledValidationLayers)
	{
		this->physicalDevices = pickPhysicalDevice();
		this->queueFamilies->findQueueFamilies(physicalDevices.get()->getPhysicalDevice());
	}

	Devices* Devices::_get()
	{
		if (devices == nullptr) {
			devices = std::make_unique<Devices>();

			return devices.get();
		}

		return devices.get();
	}

	const devices::PhysicalDevices& Devices::getPhysicalDevice() const
	{
		if (physicalDevices == nullptr) {
			throw std::runtime_error("Physical Device has not been initalized");
		}

		return *physicalDevices;
	}

	const devices::QueueFamilies& Devices::getQueueFamilies() const
	{
		if (queueFamilies == nullptr) {
			throw std::runtime_error("Queue families have not been initalized");
		}

		return *queueFamilies;
	}

	Devices* get()
	{
		return Devices::_get();
	}
}