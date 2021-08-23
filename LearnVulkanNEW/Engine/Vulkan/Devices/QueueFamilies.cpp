#include "QueueFamilies.hpp"

namespace gibvk::vulkan::devices {
	QueueFamilyIndices QueueFamilies::findQueueFamilies(vk::PhysicalDevice device)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		device.getQueueFamilyProperties(&queueFamilyCount, nullptr);

		std::vector<vk::QueueFamilyProperties> queueFamilies(queueFamilyCount);
		device.getQueueFamilyProperties(&queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
				indices.graphicsFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}

			i++;
		}

		return indices;
	}
}