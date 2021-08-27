#include "VertexInput.hpp"
namespace gibvk::vulkan::pipelines::vertexinputs {
	VertexInput::VertexInput()
	{
		vertexInputInfo = vk::PipelineVertexInputStateCreateInfo({}, 0);
	}

	const vk::PipelineVertexInputStateCreateInfo& VertexInput::getVertexInputInfo() const
	{
		return vertexInputInfo;
	}
	std::unique_ptr<VertexInput> createVertexInput()
	{
		return std::make_unique<VertexInput>();
	}
}