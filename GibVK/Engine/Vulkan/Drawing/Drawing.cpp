#include "Drawing.hpp"
#include "../../Graphics.hpp"

namespace gibvk::vulkan::drawing {
	std::unique_ptr<Drawing> Drawing::drawing = nullptr;


	Drawing* Drawing::_get()
	{
		if (drawing == nullptr) {
			drawing = std::make_unique<Drawing>();

			return drawing.get();
		}

		return drawing.get();
	}

	void Drawing::initialize()
	{
		framebuffer = framebuffers::createFramebuffers();
		commandPool = commandpools::createCommandPool();
		commandBuffer = commandbuffers::createCommandBuffer();
		/*syncObjects = syncobjects::createSyncObjects();*/
		createSyncObjects();
		//imagesInFlight.resize(graphics::get()->getSwapchain().getSwapchainImages().size(), VK_NULL_HANDLE);
	}

	void Drawing::draw()
	{
		graphics::get()->getLogicalDevice().getLogicalDevice().waitForFences(1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
		
		uint32_t imageIndex;

		graphics::get()->getLogicalDevice().getLogicalDevice().acquireNextImageKHR(graphics::get()->getSwapchain().getSwapchain(), UINT64_MAX, imageAvailableSemaphore[currentFrame],
			VK_NULL_HANDLE, &imageIndex);

		if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
			graphics::get()->getLogicalDevice().getLogicalDevice().waitForFences(1, &reinterpret_cast<vk::Fence&>(imagesInFlight[imageIndex]), VK_TRUE, UINT64_MAX);
		}

		imagesInFlight[imageIndex] = inFlightFences[currentFrame];

		vk::Semaphore waitSemaphores[] = { imageAvailableSemaphore[currentFrame] };
		vk::Semaphore signalSemaphores[] = { renderFinishedSemaphore[currentFrame] };
		vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };

		auto submitInfo = vk::SubmitInfo(1, waitSemaphores, waitStages, 1, &commandBuffer->getCommandBuffers().at(imageIndex), 1, signalSemaphores);

		graphics::get()->getLogicalDevice().getLogicalDevice().resetFences(1, &inFlightFences[currentFrame]);

		if (graphics::get()->getGraphicsQueue().getGraphicsQueue().submit(1, &submitInfo, inFlightFences[currentFrame]) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to submit draw command buffer!");
		}

		vk::SwapchainKHR swapchains[] = { graphics::get()->getSwapchain().getSwapchain() };

		auto presentInfo = vk::PresentInfoKHR(1, signalSemaphores, 1, swapchains, &imageIndex, nullptr);

		graphics::get()->getPresentQueue().getPresentQueue().presentKHR(&presentInfo);

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void Drawing::createSyncObjects()
	{
		imageAvailableSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
		imagesInFlight.resize(graphics::get()->getSwapchain().getSwapchainImages().size(), VK_NULL_HANDLE);

		auto semaphoreInfo = vk::SemaphoreCreateInfo({});

		auto fenceInfo = vk::FenceCreateInfo({ vk::FenceCreateFlagBits::eSignaled });

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			if (graphics::get()->getLogicalDevice().getLogicalDevice().createSemaphore(&semaphoreInfo, nullptr, &imageAvailableSemaphore[i]) != vk::Result::eSuccess
				|| graphics::get()->getLogicalDevice().getLogicalDevice().createSemaphore(&semaphoreInfo, nullptr, &renderFinishedSemaphore[i]) != vk::Result::eSuccess
				|| graphics::get()->getLogicalDevice().getLogicalDevice().createFence(&fenceInfo, nullptr, &inFlightFences[i]) != vk::Result::eSuccess) {

				throw std::runtime_error("Failed to create sync objects!");
			}
		}
	}

	const framebuffers::Framebuffer& Drawing::getFramebuffer() const
	{
		if (framebuffer == nullptr) {
			throw std::runtime_error("Framebuffer has not been initialized");
		}

		return *framebuffer;
	}

	const commandpools::CommandPool& Drawing::getCommandPool() const
	{
		if (commandPool == nullptr) {
			throw std::runtime_error("Command pool has not been initialized");
		}

		return *commandPool;
	}

	const commandbuffers::CommandBuffer& Drawing::getCommandBuffer() const
	{
		if (commandBuffer == nullptr) {
			throw std::runtime_error("Command buffer has not been initialized");
		}

		return *commandBuffer;
	}

	/*const syncobjects::SyncObjects& Drawing::getSyncObjects() const
	{
		if (syncObjects == nullptr) {
			throw std::runtime_error("Command pool has not been initialized");
		}

		return *syncObjects;
	}*/

	const size_t& Drawing::getCurrentFrame() const
	{
		return currentFrame;
	}

	const std::vector<VkFence>& Drawing::getImagesInFlight() const
	{
		return imagesInFlight;
	}

	const std::vector<vk::Semaphore>& Drawing::getImageAvailabeSemaphore() const
	{
		return imageAvailableSemaphore;
	}

	const std::vector<vk::Semaphore>& Drawing::getRenderFinishedSemaphore() const
	{
		return renderFinishedSemaphore;
	}

	const std::vector<vk::Fence>& Drawing::getInFlightFences() const
	{
		return inFlightFences;
	}

	//const std::vector<vk::Fence>& Drawing::getImagesInFlight() const
	//{
	//	return imagesInFlight;
	//}

	Drawing* get()
	{
		return Drawing::_get();
	}
}