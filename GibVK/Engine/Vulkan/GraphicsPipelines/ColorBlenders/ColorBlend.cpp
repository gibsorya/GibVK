#include "ColorBlend.hpp"
#include "../MainGraphicsPipeline.hpp"

namespace gibvk::vulkan::pipelines::colorblends {
	ColorBlendAttachment::ColorBlendAttachment()
	{
		colorBlendAttachment = vk::PipelineColorBlendAttachmentState(VK_FALSE, vk::BlendFactor::eOne, vk::BlendFactor::eZero, vk::BlendOp::eAdd,
			vk::BlendFactor::eOne, vk::BlendFactor::eZero, 
			vk::BlendOp::eAdd, vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
	}

	const vk::PipelineColorBlendAttachmentState& ColorBlendAttachment::getColorBlendAttachment() const
	{
		return colorBlendAttachment;
	}

	std::unique_ptr<ColorBlendAttachment> createColorBlendAttachment()
	{
		return std::make_unique<ColorBlendAttachment>();
	}

	
	ColorBlending::ColorBlending()
	{
		colorBlending = vk::PipelineColorBlendStateCreateInfo({}, VK_FALSE, vk::LogicOp::eCopy, 1, &get()->getColorBlendAttachment().getColorBlendAttachment(), { 0.0f, 0.0f, 0.0f, 0.0f });
	}

	const vk::PipelineColorBlendStateCreateInfo& ColorBlending::getColorBlending() const
	{
		return colorBlending;
	}

	std::unique_ptr<ColorBlending> createColorBlending()
	{
		return std::make_unique<ColorBlending>();
	}
}
