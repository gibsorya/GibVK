#ifndef VIEWPORT_CLASS
#define VIEWPORT_CLASS

#include <vulkan/vulkan.hpp>

namespace gibvk::vulkan::pipelines::viewports {
	class Viewport {
	public:
		Viewport();
		//Viewport(vk::Rect2D scissor = pipelines::get()->getScissor().getScissor());

		[[nodiscard]] const vk::Viewport& getViewport() const;

	private:
		vk::Viewport viewport{};
	};

	class ViewportState {
	public:
		ViewportState();

		[[nodiscard]] const vk::PipelineViewportStateCreateInfo& getViewportState() const;

	private:
		vk::PipelineViewportStateCreateInfo viewportState{};
	};

	std::unique_ptr<Viewport> createViewport();
	std::unique_ptr<ViewportState> createViewportState();
}

#endif