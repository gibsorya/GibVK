#ifndef FRAMEBUFFER_CLASS
#define FRAMEBUFFER_CLASS

#include <vulkan/vulkan.hpp>

namespace gibvk::vulkan::drawing::framebuffers {
	class Framebuffer {
	public:
		Framebuffer();

		[[nodiscard]] const std::vector<vk::Framebuffer>& getSwapchainFramebuffers() const;

	private:
		std::vector<vk::Framebuffer> swapchainFramebuffer;
	};

	std::unique_ptr<Framebuffer> createFramebuffers();
}
#endif // !FRAMEBUFFER_CLASS
