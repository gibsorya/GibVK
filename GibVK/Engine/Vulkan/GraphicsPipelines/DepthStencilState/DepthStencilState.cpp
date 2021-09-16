#include "DepthStencilState.hpp"

namespace gibvk::vulkan::pipelines::depth {
	DepthStencilState::DepthStencilState()
	{
		depthStencil = vk::PipelineDepthStencilStateCreateInfo({}, VK_TRUE, VK_TRUE, vk::CompareOp::eLess, VK_FALSE, VK_FALSE, {}, {}, 0.0f, 1.0f);
	}
	const vk::PipelineDepthStencilStateCreateInfo& DepthStencilState::getDepthStencil() const
	{
		return depthStencil;
	}
	std::unique_ptr<DepthStencilState> createDepthStencil()
	{
		return std::make_unique<DepthStencilState>();
	}
}