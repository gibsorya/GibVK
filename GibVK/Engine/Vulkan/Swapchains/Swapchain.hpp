#ifndef SWAPCHAIN_CLASS
#define SWAPCHAIN_CLASS

#include <vulkan/vulkan.hpp>
#include <vector>

namespace gibvk::vulkan::swapchains {
	static const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	struct SwapchainSupportDetails {
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
	};

	class Swapchain {
	public:
		Swapchain();

		vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);

		vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);

		vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilites);

		static SwapchainSupportDetails querySwapchainSupport(vk::PhysicalDevice device);

		[[nodiscard]] const vk::SwapchainKHR& getSwapchain() const;
		[[nodiscard]] const std::vector<vk::Image> getSwapchainImages() const;
		[[nodiscard]] const vk::Format getSwapchainImageFormat() const;
		[[nodiscard]] const vk::Extent2D getSwapchainExtent() const;

	private:
		vk::SwapchainKHR swapchain;
		std::vector<vk::Image> swapchainImages;
		vk::Format swapchainImageFormat;
		vk::Extent2D swapchainExtent;
	};

	std::unique_ptr<Swapchain> createSwapchain();
}

#endif