#include "GraphicsQueue.hpp"

namespace gibvk::vulkan::queues {
	GraphicsQueue::GraphicsQueue()
	{
	}

	const vk::Queue& GraphicsQueue::getGraphicsQueue() const
	{
		return graphicsQueue;
	}

	std::unique_ptr<GraphicsQueue> createGraphicsQueue()
	{
		return std::make_unique<GraphicsQueue>();
	}
}