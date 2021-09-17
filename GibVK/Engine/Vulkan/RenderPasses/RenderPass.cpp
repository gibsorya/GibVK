#include "RenderPass.hpp"
#include "../../Graphics.hpp"
namespace gibvk::vulkan::renderpasses {
	RenderPass::RenderPass()
	{
		auto colorAttachment = vk::AttachmentDescription({}, graphics::get()->getSwapchain().getSwapchainImageFormat(), graphics::msaaSamples,
			vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore, vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare,
			vk::ImageLayout::eUndefined, vk::ImageLayout::eColorAttachmentOptimal);
		auto colorAttachmentRef = vk::AttachmentReference(0, vk::ImageLayout::eColorAttachmentOptimal);

		auto colorAttachmentResolve = vk::AttachmentDescription({}, graphics::get()->getSwapchain().getSwapchainImageFormat(), vk::SampleCountFlagBits::e1,
			vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eStore, vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR);
		auto colorAttachmentResolveRef = vk::AttachmentReference(2, vk::ImageLayout::eColorAttachmentOptimal);

		auto depthAttachment = vk::AttachmentDescription({}, drawing::resources::findDepthFormat(), graphics::msaaSamples, vk::AttachmentLoadOp::eClear,
			vk::AttachmentStoreOp::eDontCare, vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal);
		auto depthAttachmentRef = vk::AttachmentReference(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);


		auto subpass = vk::SubpassDescription({}, vk::PipelineBindPoint::eGraphics, 0, nullptr, 1, &colorAttachmentRef, &colorAttachmentResolveRef, &depthAttachmentRef, 0, 0);
		auto dependency = vk::SubpassDependency(VK_SUBPASS_EXTERNAL, 0, vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
			vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests, {}, vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite, {});

		std::array<vk::AttachmentDescription, 3> attachments = { colorAttachment, depthAttachment, colorAttachmentResolve };
		auto renderPassInfo = vk::RenderPassCreateInfo({}, static_cast<uint32_t>(attachments.size()), attachments.data(), 1, &subpass, 1, &dependency);
		
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