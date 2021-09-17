#include "Multisampling.hpp"
#include "../../../Graphics.hpp"

namespace gibvk::vulkan::pipelines::multisampling {
	Multisampling::Multisampling()
	{
		multisampling = vk::PipelineMultisampleStateCreateInfo({}, graphics::msaaSamples, VK_FALSE, 0.2f, nullptr, VK_FALSE, VK_FALSE);
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