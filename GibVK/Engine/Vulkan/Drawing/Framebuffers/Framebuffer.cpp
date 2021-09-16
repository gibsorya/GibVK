#include "Framebuffer.hpp"
#include "../../../Graphics.hpp"
namespace gibvk::vulkan::drawing::framebuffers {
	Framebuffer::Framebuffer()
	{
		swapchainFramebuffer.resize(graphics::get()->getImageViews().getSwapchainImageViews().size());

		for (size_t i = 0; i < graphics::get()->getImageViews().getSwapchainImageViews().size(); i++) {
			/*vk::ImageView attachments[] = {
				graphics::get()->getImageViews().getSwapchainImageViews().at(i)
			};*/
			std::array<vk::ImageView, 2> attachments = { graphics::get()->getImageViews().getSwapchainImageViews().at(i), drawing::get()->getDepthResources().getDepthImageView() };

			auto framebufferInfo = vk::FramebufferCreateInfo({}, graphics::get()->getRenderPass().getRenderPass(), static_cast<uint32_t>(attachments.size()), attachments.data(),
				graphics::get()->getSwapchain().getSwapchainExtent().width, graphics::get()->getSwapchain().getSwapchainExtent().height,
				1);

			if (graphics::get()->getLogicalDevice().getLogicalDevice().createFramebuffer(&framebufferInfo, nullptr, &swapchainFramebuffer[i]) != vk::Result::eSuccess) {
				throw std::runtime_error("Failed to create framebuffers!");
			}
		}
	}
	const std::vector<vk::Framebuffer>& Framebuffer::getSwapchainFramebuffers() const
	{
		return swapchainFramebuffer;
	}
	std::unique_ptr<Framebuffer> createFramebuffers()
	{
		return std::make_unique<Framebuffer>();
	}
}