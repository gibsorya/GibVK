#ifndef QUEUE_FAMILIES_CLASS
#define QUEUE_FAMILIES_CLASS

#include <iostream>
#include <optional>
#include <vulkan/vulkan.hpp>

namespace gibvk::vulkan::devices {
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	class QueueFamilies {
	public:
		static QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device);

	};
}

#endif // !QUEUE_FAMILIES_CLASS

