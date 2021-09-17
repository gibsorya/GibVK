#include "PhysicalDevices.hpp"
#include "../../Graphics.hpp"
#include "QueueFamilies.hpp"

namespace gibvk::vulkan::devices {
	PhysicalDevices::PhysicalDevices()
	{
		uint32_t deviceCount = 0;
		graphics::get()->getInstance().getInstance().enumeratePhysicalDevices(&deviceCount, nullptr);

		if (deviceCount == 0) {
			throw std::runtime_error("Failed to find GPU's with Vulkan support!");
		}

		std::vector<vk::PhysicalDevice> physicalDevices(deviceCount);
		graphics::get()->getInstance().getInstance().enumeratePhysicalDevices(&deviceCount, physicalDevices.data());

		for (const auto& device : physicalDevices) {
			if (isDeviceSuitable(device)) {
				physicalDevice = device;
				graphics::msaaSamples = getMaxUsableSampleCount();
				break;
			}
		}

		if (&physicalDevice == VK_NULL_HANDLE) {
			throw std::runtime_error("Failed to find a suitable GPU!");
		}
	}

	bool PhysicalDevices::isDeviceSuitable(vk::PhysicalDevice device)
	{
		QueueFamilyIndices indices = devices::QueueFamilies::findQueueFamilies(device);

		bool extensionsSupported = checkDeviceExtensionSupport(device);

		bool swapchainAdequate = false;
		if (extensionsSupported) {
			swapchains::SwapchainSupportDetails swapchainSupport = swapchains::Swapchain::querySwapchainSupport(device);
			swapchainAdequate = !swapchainSupport.formats.empty() && !swapchainSupport.presentModes.empty();
		}

		vk::PhysicalDeviceFeatures supportedFeatures;
		device.getFeatures(&supportedFeatures);

		return indices.isComplete() && extensionsSupported && swapchainAdequate && supportedFeatures.samplerAnisotropy;
	}

	bool PhysicalDevices::checkDeviceExtensionSupport(vk::PhysicalDevice device)
	{
		uint32_t extensionCount;
		device.enumerateDeviceExtensionProperties(nullptr, &extensionCount, nullptr);

		std::vector<vk::ExtensionProperties> availableExtensions(extensionCount);
		device.enumerateDeviceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(swapchains::deviceExtensions.begin(), swapchains::deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	vk::SampleCountFlagBits PhysicalDevices::getMaxUsableSampleCount()
	{
		vk::PhysicalDeviceProperties physicalDeviceProperties;
		physicalDevice.getProperties(&physicalDeviceProperties);

		vk::SampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

		if (counts & vk::SampleCountFlagBits::e64) { return vk::SampleCountFlagBits::e64; }
		if (counts & vk::SampleCountFlagBits::e32) { return vk::SampleCountFlagBits::e32; }
		if (counts & vk::SampleCountFlagBits::e16) { return vk::SampleCountFlagBits::e16; }
		if (counts & vk::SampleCountFlagBits::e8) { return vk::SampleCountFlagBits::e8; }
		if (counts & vk::SampleCountFlagBits::e4) { return vk::SampleCountFlagBits::e4; }
		if (counts & vk::SampleCountFlagBits::e2) { return vk::SampleCountFlagBits::e2; }

		return vk::SampleCountFlagBits::e1;
	}

	const vk::PhysicalDevice& PhysicalDevices::getPhysicalDevice() const
	{
		return physicalDevice;
	}

	std::unique_ptr<PhysicalDevices> pickPhysicalDevice()
	{
		return std::make_unique<PhysicalDevices>();
	}
}