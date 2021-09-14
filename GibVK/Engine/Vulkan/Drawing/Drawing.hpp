#ifndef DRAWING_CLASS
#define DRAWING_CLASS

#include <vulkan/vulkan.hpp>

#include "Framebuffers/Framebuffer.hpp"
#include "CommandPools/CommandPool.hpp"
#include "CommandBuffers/CommandBuffer.hpp"
#include "TextureImages/TextureImage.hpp"
//#include "SyncObjects/SyncObjects.hpp"

namespace gibvk::vulkan::drawing {
	const int MAX_FRAMES_IN_FLIGHT = 2;

	class Drawing {
	public:
		Drawing() = default;

		static Drawing* _get();

		void initialize(bool isSwapchainCleaning = false);
		void draw();
		void createSyncObjects();
		void updateUniformBuffer(uint32_t currentImage);

		[[nodiscard]] const framebuffers::Framebuffer& getFramebuffer() const;
		[[nodiscard]] const commandpools::CommandPool& getCommandPool() const;
		[[nodiscard]] const commandbuffers::CommandBuffer& getCommandBuffer() const;
		[[nodiscard]] const textureimages::TextureImage& getTextureImage() const;
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
		std::unique_ptr<textureimages::TextureImage> textureImage{};
		//std::unique_ptr<syncobjects::SyncObjects> syncObjects{};
	};

	void createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image& image, vk::DeviceMemory& imageMemory);

	Drawing* get();
}
#endif // !DRAWING_CLASS
