#include "Graphics.hpp"

namespace gibvk::graphics {
	std::unique_ptr<Graphics> Graphics::graphics = nullptr;

	Graphics* Graphics::_get()
	{
		if (graphics == nullptr) {
			graphics = std::make_unique<Graphics>();

			return graphics.get();
		}

		return graphics.get();
	}
	void Graphics::initialize()
	{
		constexpr uint32_t WIDTH = 800;
		constexpr uint32_t HEIGHT = 600;
		constexpr auto windowTitle = "Vulkan Window";

		window = vulkan::create(WIDTH, HEIGHT, windowTitle);
		instance = vulkan::createInstance();
		debugMessenger = vulkan::debugutils::setupDebugMessenger();
		surface = vulkan::createSurface();
		physicalDevice = vulkan::devices::pickPhysicalDevice();
		graphicsQueue = vulkan::queues::createGraphicsQueue();
		presentQueue = vulkan::queues::createPresentQueue();
		logicalDevice = vulkan::devices::createLogicalDevice();
		swapchain = vulkan::swapchains::createSwapchain();
	}

	void Graphics::render()
	{
		while (!glfwWindowShouldClose(window->getWindow())) {
			glfwPollEvents();
		}
	}

	void Graphics::cleanup()
	{
		vkDestroySwapchainKHR(logicalDevice->getLogicalDevice(), swapchain->getSwapchain(), nullptr);
		logicalDevice->getLogicalDevice().destroy();

		if (vulkan::enableValidationLayers) {
			vulkan::debugutils::DestroyDebugUtilsMessengerEXT(instance->getInstance(), debugMessenger->getDebugMessenger(), nullptr);
		}
		
		vkDestroySurfaceKHR(instance->getInstance(), surface->getSurface(), nullptr);
		instance->getInstance().destroy();

		glfwDestroyWindow(window->getWindow());
		glfwTerminate();
	}

	const vulkan::VulkanWindow& Graphics::getWindow() const
	{
		if (window == nullptr) {
			throw std::runtime_error("Window has not been initialized");
		}

		return *window;
	}

	const vulkan::VulkanInstance& Graphics::getInstance() const
	{
		if (instance == nullptr) {
			throw std::runtime_error("Instance has not been initialized");
		}

		return *instance;
	}

	const vulkan::Surface& Graphics::getSurface() const
	{
		if (surface == nullptr) {
			throw std::runtime_error("Surface has not been initalized");
		}

		return *surface;
	}

	const vulkan::debugutils::DebugUtils& Graphics::getDebugMessenger() const
	{
		if (debugMessenger == nullptr) {
			throw std::runtime_error("Debug Messenger has not been initalized");
		}

		return *debugMessenger;
	}

	const vulkan::devices::PhysicalDevices& Graphics::getPhysicalDevice() const
	{
		if (physicalDevice == nullptr) {
			throw std::runtime_error("Physical Device has not been initalized");
		}

		return *physicalDevice;
	}

	const vulkan::devices::LogicalDevices& Graphics::getLogicalDevice() const
	{
		if (logicalDevice == nullptr) {
			throw std::runtime_error("Logical Device has not been initalized");
		}

		return *logicalDevice;
	}

	const vulkan::queues::GraphicsQueue& Graphics::getGraphicsQueue() const
	{
		if (graphicsQueue == nullptr) {
			throw std::runtime_error("Graphics queue has not been initalized");
		}

		return *graphicsQueue;
	}

	const vulkan::queues::PresentQueue& Graphics::getPresentQueue() const
	{
		if (presentQueue == nullptr) {
			throw std::runtime_error("Present queue has not been initialized");
		}

		return *presentQueue;
	}

	const vulkan::swapchains::Swapchain& Graphics::getSwapchain() const
	{
		if (swapchain == nullptr) {
			throw std::runtime_error("Swapchain has not been initalized");
		}

		return *swapchain;
	}

	Graphics* get()
	{
		return Graphics::_get();
	}
}