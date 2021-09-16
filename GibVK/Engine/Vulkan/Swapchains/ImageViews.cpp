#include "ImageViews.hpp"
#include "../../Graphics.hpp"

namespace gibvk::vulkan::swapchains {
	ImageViews::ImageViews()
	{
		swapchainImageViews.resize(graphics::get()->getSwapchain().getSwapchainImages().size());

		for (size_t i = 0; i < graphics::get()->getSwapchain().getSwapchainImages().size(); i++) {
			swapchainImageViews.at(i) = graphics::get()->createImageView(graphics::get()->getSwapchain().getSwapchainImages().at(i), graphics::get()->getSwapchain().getSwapchainImageFormat(), vk::ImageAspectFlagBits::eColor);
		}
	}

	const std::vector<vk::ImageView> ImageViews::getSwapchainImageViews() const
	{
		return swapchainImageViews;
	}

	std::unique_ptr<ImageViews> createImageViews()
	{
		return std::make_unique<ImageViews>();
	}
}