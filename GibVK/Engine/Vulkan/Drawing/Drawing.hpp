#ifndef DRAWING_CLASS
#define DRAWING_CLASS

#include <vulkan/vulkan.hpp>

#include "Framebuffers/Framebuffer.hpp"
#include "CommandPools/CommandPool.hpp"
#include "CommandBuffers/CommandBuffer.hpp"
//#include "SyncObjects/SyncObjects.hpp"

namespace gibvk::vulkan::drawing {
	const int MAX_FRAMES_IN_FLIGHT = 2;

	/*std::vector<vk::Semaphore> imageAvailableSemaphore(MAX_FRAMES_IN_FLIGHT);
	std::vector<vk::Semaphore> renderFinishedSemaphore(MAX_FRAMES_IN_FLIGHT);
	std::vector<vk::Fence> inFlightFences(MAX_FRAMES_IN_FLIGHT);
	std::vector<VkFence> imagesInFlight;*/

	class Drawing {
	public:
		Drawing() = default;

		static Drawing* _get();

		void initialize();
		void draw();
		void createSyncObjects();

		[[nodiscard]] const framebuffers::Framebuffer& getFramebuffer() const;
		[[nodiscard]] const commandpools::CommandPool& getCommandPool() const;
		[[nodiscard]] const commandbuffers::CommandBuffer& getCommandBuffer() const;
		//[[nodiscard]] const syncobjects::SyncObjects& getSyncObjects() const;
		[[nodiscard]] const size_t& getCurrentFrame() const;
		[[nodiscard]] const std::vector<VkFence>& getImagesInFlight() const;
		[[nodiscard]] const std::vector<vk::Semaphore>& getImageAvailabeSemaphore() const;
		[[nodiscard]] const std::vector<vk::Semaphore>& getRenderFinishedSemaphore() const;
		[[nodiscard]] const std::vector<vk::Fence>& getInFlightFences() const;


	private:
		static std::unique_ptr<Drawing> drawing;

		std::vector<vk::Semaphore> imageAvailableSemaphore;
		std::vector<vk::Semaphore> renderFinishedSemaphore;
		std::vector<vk::Fence> inFlightFences;
		std::vector<VkFence> imagesInFlight;
		size_t currentFrame = 0;

		std::unique_ptr<framebuffers::Framebuffer> framebuffer{};
		std::unique_ptr<commandpools::CommandPool> commandPool{};
		std::unique_ptr<commandbuffers::CommandBuffer> commandBuffer{};
		//std::unique_ptr<syncobjects::SyncObjects> syncObjects{};
	};

	Drawing* get();
}
#endif // !DRAWING_CLASS
