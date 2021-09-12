#include "DescriptorSet.hpp"
#include "../../../Graphics.hpp"
#include "../Buffers.hpp"

namespace gibvk::renderer::buffers::descriptors {
	DescriptorSetLayout::DescriptorSetLayout()
	{
		auto uboLayoutBinding = vk::DescriptorSetLayoutBinding(0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex, nullptr);
		//VkDescriptorSetLayoutBinding uboLayoutBinding{};
		//uboLayoutBinding.binding = 0;
		//uboLayoutBinding.descriptorCount = 1;
		//uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		//uboLayoutBinding.pImmutableSamplers = nullptr;
		//uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		auto layoutInfo = vk::DescriptorSetLayoutCreateInfo({}, 1, &uboLayoutBinding);
		//VkDescriptorSetLayoutCreateInfo layoutInfo{};
		//layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		//layoutInfo.bindingCount = 1;
		//layoutInfo.pBindings = &uboLayoutBinding;

		if (graphics::get()->getLogicalDevice().getLogicalDevice().createDescriptorSetLayout(&layoutInfo, nullptr, &descriptorSetLayout) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create descriptor set layout!");
		}

		//if (vkCreateDescriptorSetLayout(graphics::get()->getLogicalDevice().getLogicalDevice(), &layoutInfo, nullptr, &reinterpret_cast<VkDescriptorSetLayout&>(descriptorSetLayout)) != VK_SUCCESS) {
		//	throw std::runtime_error("failed to create descriptor set layout!");
		//}
	}

	const vk::DescriptorSetLayout& DescriptorSetLayout::getDescriptorSetLayout() const
	{
		return descriptorSetLayout;
	}


	std::unique_ptr<DescriptorSetLayout> createDescriptorSetLayout()
	{
		return std::make_unique<DescriptorSetLayout>();
	}


	DescriptorSets::DescriptorSets()
	{
		std::vector<vk::DescriptorSetLayout> layouts(graphics::get()->getSwapchain().getSwapchainImages().size(), buffers::get()->getDescriptorSetLayout().getDescriptorSetLayout());
		auto allocInfo = vk::DescriptorSetAllocateInfo(buffers::get()->getDescriptorPool().getDescriptorPool(), static_cast<uint32_t>(graphics::get()->getSwapchain().getSwapchainImages().size()), layouts.data());
		
		descriptorSets.resize(graphics::get()->getSwapchain().getSwapchainImages().size());
		if (graphics::get()->getLogicalDevice().getLogicalDevice().allocateDescriptorSets(&allocInfo, descriptorSets.data()) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to allocate descriptor sets!");
		}

		for (size_t i = 0; i < graphics::get()->getSwapchain().getSwapchainImages().size(); i++) {
			auto bufferInfo = vk::DescriptorBufferInfo(buffers::get()->getUniformBuffer().getUniformbuffers().at(i), 0, sizeof(uniformbuffers::UniformBufferObject));

			auto descriptorWrite = vk::WriteDescriptorSet(descriptorSets.at(i), 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &bufferInfo, nullptr);
			
			graphics::get()->getLogicalDevice().getLogicalDevice().updateDescriptorSets(1, &descriptorWrite, 0, nullptr);
		}
	}

	const std::vector<vk::DescriptorSet>& DescriptorSets::getDescriptorSets() const
	{
		return descriptorSets;
	}

	std::unique_ptr<DescriptorSets> createDescriptorSets()
	{
		return std::make_unique<DescriptorSets>();
	}
}