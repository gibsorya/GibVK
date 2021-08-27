#include "CommandPool.hpp"
#include "../../../Graphics.hpp"

namespace gibvk::vulkan::drawing::commandpools {
	CommandPool::CommandPool()
	{
		devices::QueueFamilyIndices indices = devices::QueueFamilies::findQueueFamilies(graphics::get()->getPhysicalDevice().getPhysicalDevice());

		auto poolInfo = vk::CommandPoolCreateInfo({}, indices.graphicsFamily.value());

		if (graphics::get()->getLogicalDevice().getLogicalDevice().createCommandPool(&poolInfo, nullptr, &commandPool) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create command pool!");
		}
	}

	const vk::CommandPool& CommandPool::getCommandPool() const
	{
		return commandPool;
	}

	std::unique_ptr<CommandPool> createCommandPool()
	{
		return std::make_unique<CommandPool>();
	}
}