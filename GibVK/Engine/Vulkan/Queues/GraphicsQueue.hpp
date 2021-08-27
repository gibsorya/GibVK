#ifndef GRAPHICS_QUEUE_CLASS
#define GRAPHICS_QUEUE_CLASS
#include <vulkan/vulkan.hpp>

namespace gibvk::vulkan::queues {
	class GraphicsQueue {
	public:
		GraphicsQueue();

		[[nodiscard]] const vk::Queue& getGraphicsQueue() const;

	private:
		vk::Queue graphicsQueue;
	};

	std::unique_ptr<GraphicsQueue> createGraphicsQueue();
}
#endif // !GRAPHICS_QUEUE_CLASS
