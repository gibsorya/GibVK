#ifndef DESCRIPTOR_POOL_CLASS
#define DESCRIPTOR_POOL_CLASS

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

namespace gibvk::renderer::buffers::descriptors {
	class DescriptorPool {
	public:
		DescriptorPool();

		[[nodiscard]] const vk::DescriptorPool& getDescriptorPool() const;

	private:
		vk::DescriptorPool descriptorPool;
	};

	std::unique_ptr<DescriptorPool> createDescriptorPool();
}

#endif