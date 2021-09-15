#include "DescriptorPool.hpp"
#include "../../../Graphics.hpp"

namespace gibvk::renderer::buffers::descriptors {
	DescriptorPool::DescriptorPool()
	{
		std::array<vk::DescriptorPoolSize, 2> poolSizes{};
		poolSizes.at(0)	= vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, static_cast<uint32_t>(graphics::get()->getSwapchain().getSwapchainImages().size()));
		poolSizes.at(1) = vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, static_cast<uint32_t>(graphics::get()->getSwapchain().getSwapchainImages().size()));

		auto poolInfo = vk::DescriptorPoolCreateInfo({}, static_cast<uint32_t>(graphics::get()->getSwapchain().getSwapchainImages().size()), static_cast<uint32_t>(poolSizes.size()), poolSizes.data());

		if (graphics::get()->getLogicalDevice().getLogicalDevice().createDescriptorPool(&poolInfo, nullptr, &descriptorPool) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create descriptor pool!");
		}
	}
	const vk::DescriptorPool& DescriptorPool::getDescriptorPool() const
	{
		return descriptorPool;
	}
	std::unique_ptr<DescriptorPool> createDescriptorPool()
	{
		return std::make_unique<DescriptorPool>();
	}
}
