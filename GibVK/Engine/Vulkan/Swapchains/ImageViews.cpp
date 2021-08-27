#include "ImageViews.hpp"
#include "../../Graphics.hpp"

namespace gibvk::vulkan::swapchains {
	ImageViews::ImageViews()
	{
		swapchainImageViews.resize(graphics::get()->getSwapchain().getSwapchainImages().size());

		for (size_t i = 0; i < graphics::get()->getSwapchain().getSwapchainImages().size(); i++) {
			vk::ImageViewCreateInfo createInfo({}, graphics::get()->getSwapchain().getSwapchainImages()[i], vk::ImageViewType::e2D, graphics::get()->getSwapchain().getSwapchainImageFormat(),
				{ vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity },
				{ vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 });

			if (graphics::get()->getLogicalDevice().getLogicalDevice().createImageView(&createInfo, nullptr, &swapchainImageViews[i]) != vk::Result::eSuccess) {
				throw std::runtime_error("Failed to create image views!");
			}
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