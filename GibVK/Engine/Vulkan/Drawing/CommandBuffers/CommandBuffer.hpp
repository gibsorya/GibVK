#ifndef COMMAND_BUFFER_CLASS
#define COMMAND_BUFFER_CLASS

#include <vulkan/vulkan.hpp>

namespace gibvk::vulkan::drawing::commandbuffers {
	class CommandBuffer {
	public:
		CommandBuffer();

		[[nodiscard]] const std::vector<vk::CommandBuffer>& getCommandBuffers() const;

	private:
		std::vector<vk::CommandBuffer> commandBuffers;
	};

	std::unique_ptr<CommandBuffer> createCommandBuffer();
}

#endif // !COMMAND_BUFFER_CLASS
