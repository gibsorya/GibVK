#ifndef COMMAND_POOL_CLASS
#define COMMAND_POOL_CLASS

#include <vulkan/vulkan.hpp>
namespace gibvk::vulkan::drawing::commandpools {
	class CommandPool {
	public:
		CommandPool();

		[[nodiscard]] const vk::CommandPool& getCommandPool() const;

	private:
		vk::CommandPool commandPool;
	};

	std::unique_ptr<CommandPool> createCommandPool();
}

#endif // !COMMAND_POOL_CLASS
