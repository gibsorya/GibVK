//#include "SyncObjects.hpp"
//#include "../../../Graphics.hpp"
//
//namespace gibvk::vulkan::drawing::syncobjects {
//	SyncObjects::SyncObjects()
//	{
//		imageAvailableSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
//		renderFinishedSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
//		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
//		imagesInFlight.resize(graphics::get()->getSwapchain().getSwapchainImages().size(), VK_NULL_HANDLE);
//
//		auto semaphoreInfo = vk::SemaphoreCreateInfo({});
//
//		auto fenceInfo = vk::FenceCreateInfo({vk::FenceCreateFlagBits::eSignaled});
//		
//		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
//			if (graphics::get()->getLogicalDevice().getLogicalDevice().createSemaphore(&semaphoreInfo, nullptr, &imageAvailableSemaphore[i]) != vk::Result::eSuccess
//				|| graphics::get()->getLogicalDevice().getLogicalDevice().createSemaphore(&semaphoreInfo, nullptr, &renderFinishedSemaphore[i]) != vk::Result::eSuccess
//				|| graphics::get()->getLogicalDevice().getLogicalDevice().createFence(&fenceInfo, nullptr, &inFlightFences[i]) != vk::Result::eSuccess) {
//
//				throw std::runtime_error("Failed to create sync objects!");
//			}
//		}
//		
//	}
//
//	const std::vector<vk::Semaphore>& SyncObjects::getImageAvailabeSemaphore() const
//	{
//		return imageAvailableSemaphore;
//	}
//
//	const std::vector<vk::Semaphore>& SyncObjects::getRenderFinishedSemaphore() const
//	{
//		return renderFinishedSemaphore;
//	}
//
//	const std::vector<vk::Fence>& SyncObjects::getInFlightFences() const
//	{
//		return inFlightFences;
//	}
//
//	std::vector<vk::Fence>& SyncObjects::getImagesInFlight()
//	{
//		return imagesInFlight;
//	}
//	
//	std::unique_ptr<SyncObjects> createSyncObjects()
//	{
//		return std::make_unique<SyncObjects>();
//	}
//}