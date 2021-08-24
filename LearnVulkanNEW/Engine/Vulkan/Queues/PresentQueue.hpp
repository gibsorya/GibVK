#ifndef PRESENT_QUEUE_CLASS
#define PRESENT_QUEUE_CLASS
#include <vulkan/vulkan.hpp>

namespace gibvk::vulkan::queues {
	class PresentQueue {
	public:
		PresentQueue();

		[[nodiscard]] const vk::Queue& getPresentQueue() const;
		
	private:
		vk::Queue presentQueue;
	};

	std::unique_ptr<PresentQueue> createPresentQueue();
}

#endif // !PRESENT_QUEUE_CLASS
