#include "ColorResources.hpp"
#include "../../../Graphics.hpp"


namespace gibvk::vulkan::drawing::resources {
	ColorResources::ColorResources()
	{
		vk::Format colorFormat = graphics::get()->getSwapchain().getSwapchainImageFormat();

		createImage(graphics::get()->getSwapchain().getSwapchainExtent().width, graphics::get()->getSwapchain().getSwapchainExtent().height, 1, graphics::msaaSamples, colorFormat,
			vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment, 
			vk::MemoryPropertyFlagBits::eDeviceLocal, colorImage, colorImageMemory);

		colorImageView = graphics::get()->createImageView(colorImage, colorFormat, vk::ImageAspectFlagBits::eColor, 1);

	}
	const vk::Image& ColorResources::getColorImage() const
	{
		return colorImage;
	}
	const vk::DeviceMemory& ColorResources::getColorImageMemory() const
	{
		return colorImageMemory;
	}
	const vk::ImageView& ColorResources::getColorImageView() const
	{
		return colorImageView;
	}
	std::unique_ptr<ColorResources> createColorResources()
	{
		return std::make_unique<ColorResources>();
	}
}