//#ifndef SYNC_OBJECTS_CLASS
//#define SYNC_OBJECTS_CLASS
//
//#include <vulkan/vulkan.hpp>
//
//namespace gibvk::vulkan::drawing::syncobjects {
//	class SyncObjects {
//	public:
//		SyncObjects();
//
//		[[nodiscard]] const std::vector<vk::Semaphore>& getImageAvailabeSemaphore() const;
//		[[nodiscard]] const std::vector<vk::Semaphore>& getRenderFinishedSemaphore() const;
//		[[nodiscard]] const std::vector<vk::Fence>& getInFlightFences() const;
//		[[nodiscard]] std::vector<vk::Fence>& getImagesInFlight();
//	private:
//		std::vector<vk::Semaphore> imageAvailableSemaphore;
//		std::vector<vk::Semaphore> renderFinishedSemaphore;
//		std::vector<vk::Fence> inFlightFences;
//		std::vector<vk::Fence> imagesInFlight;
//	};
//
//	std::unique_ptr<SyncObjects> createSyncObjects();
//
//}
//
//#endif // !SYNC_OBJECTS_CLASS
