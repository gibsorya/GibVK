#include "Swapchain.hpp"
#include "../Devices/QueueFamilies.hpp"
#include "../../Graphics.hpp"

namespace gibvk::vulkan::swapchains {
	Swapchain::Swapchain()
	{
		SwapchainSupportDetails swapchainSupport = querySwapchainSupport(graphics::get()->getPhysicalDevice().getPhysicalDevice());

		vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapchainSupport.formats);
		vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapchainSupport.presentModes);
		vk::Extent2D swapExtent = chooseSwapExtent(swapchainSupport.capabilities);

		uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;
		
		if (swapchainSupport.capabilities.maxImageCount > 0 && imageCount > swapchainSupport.capabilities.maxImageCount) {
			imageCount = swapchainSupport.capabilities.maxImageCount;
		}

		vk::SwapchainCreateInfoKHR createInfo({}, graphics::get()->getSurface().getSurface(), imageCount,
			surfaceFormat.format, surfaceFormat.colorSpace, swapExtent, 1, vk::ImageUsageFlagBits::eColorAttachment, 
			vk::SharingMode::eExclusive, 0, nullptr, swapchainSupport.capabilities.currentTransform, vk::CompositeAlphaFlagBitsKHR::eOpaque,
			presentMode, VK_TRUE, VK_NULL_HANDLE);

		devices::QueueFamilyIndices indices = devices::QueueFamilies::findQueueFamilies(graphics::get()->getPhysicalDevice().getPhysicalDevice());
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}

		if (graphics::get()->getLogicalDevice().getLogicalDevice().createSwapchainKHR(&createInfo, nullptr, &swapchain) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create swapchain!");
		}

		graphics::get()->getLogicalDevice().getLogicalDevice().getSwapchainImagesKHR(swapchain, &imageCount, nullptr);
		//graphics::get()->getSwapchain().getSwapchain()
		swapchainImages.resize(imageCount);
		graphics::get()->getLogicalDevice().getLogicalDevice().getSwapchainImagesKHR(swapchain, &imageCount, swapchainImages.data());

		swapchainImageFormat = surfaceFormat.format;
		swapchainExtent = swapExtent;

	}

	vk::SurfaceFormatKHR Swapchain::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
	{
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	vk::PresentModeKHR Swapchain::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
				return availablePresentMode;
			}
		}

		return vk::PresentModeKHR::eFifo;
	}

	vk::Extent2D Swapchain::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilites)
	{
		if (capabilites.currentExtent.width != UINT32_MAX) {
			return capabilites.currentExtent;
		}
		else {
			int width, height;
			glfwGetFramebufferSize(graphics::get()->getWindow().getWindow(), &width, &height);

			vk::Extent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = std::clamp(actualExtent.width, capabilites.minImageExtent.width, capabilites.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilites.minImageExtent.height, capabilites.maxImageExtent.height);

			return actualExtent;
		}
	}

	SwapchainSupportDetails Swapchain::querySwapchainSupport(vk::PhysicalDevice device)
	{
		SwapchainSupportDetails details;

		device.getSurfaceCapabilitiesKHR(graphics::get()->getSurface().getSurface(), &details.capabilities);

		uint32_t formatCount;

		device.getSurfaceFormatsKHR(graphics::get()->getSurface().getSurface(), &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			device.getSurfaceFormatsKHR(graphics::get()->getSurface().getSurface(), &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		device.getSurfacePresentModesKHR(graphics::get()->getSurface().getSurface(), &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			device.getSurfacePresentModesKHR(graphics::get()->getSurface().getSurface(), &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	const vk::SwapchainKHR& Swapchain::getSwapchain() const
	{
		return swapchain;
	}

	const std::vector<vk::Image> Swapchain::getSwapchainImages() const
	{
		return swapchainImages;
	}

	const vk::Format Swapchain::getSwapchainImageFormat() const
	{
		return swapchainImageFormat;
	}

	const vk::Extent2D Swapchain::getSwapchainExtent() const
	{
		return swapchainExtent;
	}

	std::unique_ptr<Swapchain> createSwapchain()
	{
		return std::make_unique<Swapchain>();
	}
}