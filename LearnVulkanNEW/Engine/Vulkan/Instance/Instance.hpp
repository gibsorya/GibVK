#ifndef INSTANCE_CLASS
#define INSTANCE_CLASS

#include <vulkan/vulkan.hpp>

namespace gibvk::vulkan {
	class VulkanInstance {
	public:
		VulkanInstance();

		//void createInstance();

		[[nodiscard]] const vk::Instance& getInstance() const;
	private:
		vk::Instance instance;
	};

	std::unique_ptr<VulkanInstance> create();
}

#endif