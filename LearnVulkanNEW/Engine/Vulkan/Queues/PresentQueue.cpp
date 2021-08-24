#include "PresentQueue.hpp"

namespace gibvk::vulkan::queues {
	PresentQueue::PresentQueue()
	{
	}
	const vk::Queue& PresentQueue::getPresentQueue() const
	{
		return presentQueue;
	}
	std::unique_ptr<PresentQueue> createPresentQueue()
	{
		return std::make_unique<PresentQueue>();
	}
}