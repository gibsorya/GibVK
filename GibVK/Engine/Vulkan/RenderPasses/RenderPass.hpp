#ifndef RENDER_PASS_CLASS
#define RENDER_PASS_CLASS

#include <vulkan/vulkan.hpp>

namespace gibvk::vulkan::renderpasses {
	class RenderPass {
	public:
		RenderPass();

		[[nodiscard]] const vk::RenderPass& getRenderPass() const;

	private:
		vk::RenderPass renderPass;
	};

	std::unique_ptr<RenderPass> createRenderPass();
}
#endif // !RENDER_PASS_CLASS
