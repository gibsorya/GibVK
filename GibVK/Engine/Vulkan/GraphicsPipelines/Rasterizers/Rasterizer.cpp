#include "Rasterizer.hpp"
namespace gibvk::vulkan::pipelines::rasterizers {
	Rasterizer::Rasterizer()
	{
		rasterizer = vk::PipelineRasterizationStateCreateInfo({}, VK_FALSE, VK_FALSE, vk::PolygonMode::eFill, vk::CullModeFlagBits::eBack,
			vk::FrontFace::eClockwise, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f);
	}

	const vk::PipelineRasterizationStateCreateInfo& Rasterizer::getRasterizer() const
	{
		return rasterizer;
	}

	std::unique_ptr<Rasterizer> createRasterizer()
	{
		return std::make_unique<Rasterizer>();
	}
}