#include "VertexInput.hpp"
#include "../../../Renderer/Buffers/Buffers.hpp"

namespace gibvk::vulkan::pipelines::vertexinputs {
	VertexInput::VertexInput()
	{
		auto bindingDescription = renderer::buffers::vertexbuffers::Vertex::getBindingDescription();
		auto attributeDescriptions = renderer::buffers::vertexbuffers::Vertex::getAttributeDescriptions();

		vertexInputInfo = vk::PipelineVertexInputStateCreateInfo({}, 1, &bindingDescription, static_cast<uint32_t>(attributeDescriptions.size()), attributeDescriptions.data());
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