#ifndef IMAGE_VIEWS_CLASS
#define IMAGE_VIEWS_CLASS

#include <vulkan/vulkan.hpp>

namespace gibvk::vulkan::swapchains {
	class ImageViews {
	public:
		ImageViews();

		[[nodiscard]] const std::vector<vk::ImageView> getSwapchainImageViews() const;

	private:
		std::vector<vk::ImageView> swapchainImageViews;
	};

	std::unique_ptr<ImageViews> createImageViews();

}


#endif // !IMAGE_VIEWS_CLASS
