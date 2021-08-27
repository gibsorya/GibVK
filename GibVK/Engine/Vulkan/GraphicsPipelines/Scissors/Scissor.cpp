#include "Scissor.hpp"
#include "../../../Graphics.hpp"

namespace gibvk::vulkan::pipelines::scissors {
	Scissor::Scissor()
	{
		scissor = vk::Rect2D({ 0, 0 }, gibvk::graphics::get()->getSwapchain().getSwapchainExtent());
	}

	const vk::Rect2D& Scissor::getScissor() const
	{
		return scissor;
	}

	std::unique_ptr<Scissor> createScissor()
	{
		return std::make_unique<Scissor>();
	}
}