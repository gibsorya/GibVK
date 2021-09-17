#include "DepthResources.hpp"
#include "../../../Graphics.hpp"

namespace gibvk::vulkan::drawing::depthresources {
	DepthResources::DepthResources()
	{
		vk::Format depthFormat = findDepthFormat();

		createImage(graphics::get()->getSwapchain().getSwapchainExtent().width, graphics::get()->getSwapchain().getSwapchainExtent().height, 1, depthFormat, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal, depthImage, depthImageMemory);
		depthImageView = graphics::get()->createImageView(depthImage, depthFormat, vk::ImageAspectFlagBits::eDepth, 1);

		drawing::transitionImageLayout(depthImage, depthFormat, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal, 1);

	}

	vk::Format findDepthFormat()
	{
		return drawing::findSupportedFormat({ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint }, vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment);
	}

	const vk::Image& DepthResources::getDepthImage() const
	{
		return depthImage;
	}

	const vk::DeviceMemory& DepthResources::getDepthImageMemory() const
	{
		return depthImageMemory;
	}

	const vk::ImageView& DepthResources::getDepthImageView() const
	{
		return depthImageView;
	}

	std::unique_ptr<DepthResources> createDepthResources()
	{
		return std::make_unique<DepthResources>();
	}
}