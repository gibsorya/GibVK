#include "CommandBuffer.hpp"
#include "../Drawing.hpp"
#include "../../../Graphics.hpp"
#include "../../../Renderer/Renderer.hpp"

namespace gibvk::vulkan::drawing::commandbuffers {
	CommandBuffer::CommandBuffer()
	{
		commandBuffers.resize(drawing::get()->getFramebuffer().getSwapchainFramebuffers().size());

		auto allocInfo = vk::CommandBufferAllocateInfo(drawing::get()->getCommandPool().getCommandPool(),vk::CommandBufferLevel::ePrimary, (uint32_t)commandBuffers.size());

		if (graphics::get()->getLogicalDevice().getLogicalDevice().allocateCommandBuffers(&allocInfo, commandBuffers.data()) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to allocate command buffers!");
		}

		for (size_t i = 0; i < commandBuffers.size(); i++) {
			auto beginInfo = vk::CommandBufferBeginInfo({}, nullptr);

			if (commandBuffers[i].begin(&beginInfo) != vk::Result::eSuccess) {
				throw std::runtime_error("Failed to begin recording command buffer!");
			}

			vk::ClearValue clearColor = { vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}) };

			auto renderPassInfo = vk::RenderPassBeginInfo(graphics::get()->getRenderPass().getRenderPass(), drawing::get()->getFramebuffer().getSwapchainFramebuffers().at(i),
				{ {0, 0}, graphics::get()->getSwapchain().getSwapchainExtent() }, 1, &clearColor);

			commandBuffers[i].beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);

			commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, pipelines::get()->getGraphicsPipeline());

			vk::Buffer vertexBuffers[] = {renderer::buffers::get()->getVertexBuffer().getVertexBuffer()};
			vk::DeviceSize offsets[] = {0};
			commandBuffers[i].bindVertexBuffers(0, 1, vertexBuffers, offsets);

			commandBuffers[i].bindIndexBuffer(renderer::buffers::get()->getIndexBuffer().getIndexBuffer(), 0, vk::IndexType::eUint16);

			commandBuffers[i].drawIndexed(static_cast<uint32_t>(renderer::buffers::indexbuffers::indices.size()), 1, 0, 0, 0);

			commandBuffers[i].endRenderPass();
			
			if  (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("Failed to record command buffer!");
			}
		}

	}

	const std::vector<vk::CommandBuffer>& CommandBuffer::getCommandBuffers() const
	{
		return commandBuffers;
	}

	std::unique_ptr<CommandBuffer> createCommandBuffer()
	{
		return std::make_unique<CommandBuffer>();
	}
}