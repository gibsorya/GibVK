#include "DescriptorSet.hpp"
#include "../../../Graphics.hpp"
#include "../Buffers.hpp"

namespace gibvk::renderer::buffers::descriptors {
	DescriptorSetLayout::DescriptorSetLayout()
	{
		auto uboLayoutBinding = vk::DescriptorSetLayoutBinding(0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex, nullptr);

		auto samplerLayoutBinding = vk::DescriptorSetLayoutBinding(1, vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment, nullptr);

		std::array<vk::DescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

		auto layoutInfo = vk::DescriptorSetLayoutCreateInfo({}, static_cast<uint32_t>(bindings.size()), bindings.data());

		if (graphics::get()->getLogicalDevice().getLogicalDevice().createDescriptorSetLayout(&layoutInfo, nullptr, &descriptorSetLayout) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create descriptor set layout!");
		}
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

			auto imageInfo = vk::DescriptorImageInfo(vulkan::drawing::get()->getTextureSampler().getTextureSampler(), vulkan::drawing::get()->getTextureImageView().getTextureImageView(), vk::ImageLayout::eShaderReadOnlyOptimal);

			std::array<vk::WriteDescriptorSet, 2> descriptorWrites{};

			descriptorWrites.at(0) = vk::WriteDescriptorSet(descriptorSets.at(i), 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &bufferInfo, nullptr);
			descriptorWrites.at(1) = vk::WriteDescriptorSet(descriptorSets.at(i), 1, 0, 1, vk::DescriptorType::eCombinedImageSampler, &imageInfo, nullptr, nullptr);

			graphics::get()->getLogicalDevice().getLogicalDevice().updateDescriptorSets(static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
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