#include "RenderPass.hpp"
#include "../../Graphics.hpp"
namespace gibvk::vulkan::renderpasses {
	RenderPass::RenderPass()
	{
		auto colorAttachment = vk::AttachmentDescription({}, graphics::get()->getSwapchain().getSwapchainImageFormat(), vk::SampleCountFlagBits::e1,
			vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore, vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare,
			vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR);

		auto colorAttachmentRef = vk::AttachmentReference(0, vk::ImageLayout::eColorAttachmentOptimal);
		auto subpass = vk::SubpassDescription({}, vk::PipelineBindPoint::eGraphics, 0, nullptr, 1, &colorAttachmentRef, nullptr, nullptr, 0, 0);
		auto dependency = vk::SubpassDependency(VK_SUBPASS_EXTERNAL, 0, vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eColorAttachmentOutput, {}, vk::AccessFlagBits::eColorAttachmentWrite, {});
		auto renderPassInfo = vk::RenderPassCreateInfo({}, 1, &colorAttachment, 1, &subpass, 1, &dependency);
		
		if (graphics::get()->getLogicalDevice().getLogicalDevice().createRenderPass(&renderPassInfo, nullptr, &renderPass) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create render pass!");
		}
	}

	const vk::RenderPass& RenderPass::getRenderPass() const
	{
		return renderPass;
	}

	std::unique_ptr<RenderPass> createRenderPass()
	{
		return std::make_unique<RenderPass>();
	}
}