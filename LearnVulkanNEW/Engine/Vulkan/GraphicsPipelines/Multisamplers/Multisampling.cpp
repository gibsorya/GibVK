#include "Multisampling.hpp"

namespace gibvk::vulkan::pipelines::multisampling {
	Multisampling::Multisampling()
	{
		multisampling = vk::PipelineMultisampleStateCreateInfo({},vk::SampleCountFlagBits::e1, VK_FALSE, 1.0f, nullptr, VK_FALSE, VK_FALSE);
	}

	const vk::PipelineMultisampleStateCreateInfo& Multisampling::getMultisampling() const
	{
		return multisampling;
	}

	std::unique_ptr<Multisampling> createMultisampling()
	{
		return std::make_unique<Multisampling>();
	}
}