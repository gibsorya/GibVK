#include "Viewport.hpp"
#include "../../../Graphics.hpp"

namespace gibvk::vulkan::pipelines::viewports {
	Viewport::Viewport()
	{
		viewport = vk::Viewport(0.0f, 0.0f, (float)gibvk::graphics::get()->getSwapchain().getSwapchainExtent().width, 
			(float)gibvk::graphics::get()->getSwapchain().getSwapchainExtent().height, 0.0f, 1.0f);
	}

	const vk::Viewport& Viewport::getViewport() const
	{
		return viewport;
	}

	std::unique_ptr<Viewport> createViewport()
	{
		return std::make_unique<Viewport>();
	}

	ViewportState::ViewportState()
	{
		viewportState = vk::PipelineViewportStateCreateInfo({}, 1, &pipelines::get()->getViewport().getViewport(), 1, &pipelines::get()->getScissor().getScissor());
	}

	const vk::PipelineViewportStateCreateInfo& ViewportState::getViewportState() const
	{
		return viewportState;
	}
	
	std::unique_ptr<ViewportState> createViewportState()
	{
		return std::make_unique<ViewportState>();
	}
}