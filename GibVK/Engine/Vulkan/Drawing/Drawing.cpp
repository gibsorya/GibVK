#include "Drawing.hpp"
#include "../../Graphics.hpp"
#include "../../Renderer/Renderer.hpp"
#include "../../Renderer/Buffers/Buffers.hpp"
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

	void Drawing::initialize(bool isSwapchainCleaning)
	{
		framebuffer = framebuffers::createFramebuffers();
		if (!isSwapchainCleaning) {
			commandPool = commandpools::createCommandPool();
		}
		renderer::get()->initialize(isSwapchainCleaning);
		commandBuffer = commandbuffers::createCommandBuffer();

		if (!isSwapchainCleaning) {
			createSyncObjects();
		}
	}

	void Drawing::draw()
	{
		graphics::get()->getLogicalDevice().getLogicalDevice().waitForFences(1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
		
		uint32_t imageIndex;

		vk::Result result = graphics::get()->getLogicalDevice().getLogicalDevice().acquireNextImageKHR(graphics::get()->getSwapchain().getSwapchain(), UINT64_MAX, imageAvailableSemaphore[currentFrame], VK_NULL_HANDLE, &imageIndex);

		if (result == vk::Result::eErrorOutOfDateKHR) {
			graphics::get()->recreateSwapchain();
			return;
		} else if(result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
			throw std::runtime_error("Failed to acquire swapchain image!");
		}

		updateUniformBuffer(imageIndex);

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

		result = graphics::get()->getPresentQueue().getPresentQueue().presentKHR(&presentInfo);

		if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || &graphics::get()->getFramebufferResized()) {
			graphics::get()->getFramebufferResized() = false;
			graphics::get()->recreateSwapchain();
			return;
		}
		else if (result != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to present swapchain image!");
		}
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

	void Drawing::updateUniformBuffer(uint32_t currentImage)
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		renderer::buffers::uniformbuffers::UniformBufferObject ubo{};
		ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = glm::perspective(glm::radians(45.0f), graphics::get()->getSwapchain().getSwapchainExtent().width / (float)graphics::get()->getSwapchain().getSwapchainExtent().height, 0.1f, 10.0f);

		ubo.proj[1][1] *= -1;


		void* data;
		graphics::get()->getLogicalDevice().getLogicalDevice().mapMemory(renderer::buffers::get()->getUniformBuffer().getUniformbuffersMemory().at(currentImage), 0, sizeof(ubo), {}, &data);
			memcpy(data, &ubo, sizeof(ubo));
		graphics::get()->getLogicalDevice().getLogicalDevice().unmapMemory(renderer::buffers::get()->getUniformBuffer().getUniformbuffersMemory().at(currentImage));
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

	Drawing* get()
	{
		return Drawing::_get();
	}
}