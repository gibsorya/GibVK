#ifndef DEPTH_STENCIL_STATE_CLASS
#define DEPTH_STENCIL_STATE_CLASS

#include <vulkan/vulkan.hpp>

namespace gibvk::vulkan::pipelines::depth {
	class DepthStencilState {
	public:
		DepthStencilState();

		[[nodiscard]] const vk::PipelineDepthStencilStateCreateInfo& getDepthStencil() const;

	private:
		vk::PipelineDepthStencilStateCreateInfo depthStencil{};
	};

	std::unique_ptr<DepthStencilState> createDepthStencil();
}

#endif // !DEPTH_STENCIL_STATE_CLASS
