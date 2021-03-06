#include "Graphics.hpp"
#include "Renderer/Buffers/Buffers.hpp"

namespace gibvk::graphics {
	std::unique_ptr<Graphics> Graphics::graphics = nullptr;

	vk::SampleCountFlagBits msaaSamples = vk::SampleCountFlagBits::e1;

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
		imageViews = vulkan::swapchains::createImageViews();
		renderPass = vulkan::renderpasses::createRenderPass();
		renderer::buffers::get()->initDescriptor();
		vulkan::pipelines::get()->initialize();
		vulkan::drawing::get()->initialize();
	}

	void Graphics::render()
	{
		while (!glfwWindowShouldClose(window->getWindow())) {
			glfwPollEvents();
			drawFrame();
		}

		logicalDevice->getLogicalDevice().waitIdle();
	}

	void Graphics::drawFrame()
	{
		vulkan::drawing::get()->draw();
	}

	void Graphics::recreateSwapchain()
	{
		int width = 0, height = 0;
		glfwGetFramebufferSize(window->getWindow(), &width, &height);

		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(window->getWindow(), &width, &height);
			glfwWaitEvents();
		}

		logicalDevice->getLogicalDevice().waitIdle();

		cleanupSwapchain();

		swapchain = vulkan::swapchains::createSwapchain();
		imageViews = vulkan::swapchains::createImageViews();
		renderPass = vulkan::renderpasses::createRenderPass();
		vulkan::pipelines::get()->initialize();
		vulkan::drawing::get()->recreateSwapchain();
	}

	void Graphics::cleanupSwapchain()
	{
		logicalDevice->getLogicalDevice().destroyImageView(vulkan::drawing::get()->getDepthResources().getDepthImageView());
		logicalDevice->getLogicalDevice().destroyImage(vulkan::drawing::get()->getDepthResources().getDepthImage());
		logicalDevice->getLogicalDevice().freeMemory(vulkan::drawing::get()->getDepthResources().getDepthImageMemory());

		logicalDevice->getLogicalDevice().destroyImageView(vulkan::drawing::get()->getColorResources().getColorImageView());
		logicalDevice->getLogicalDevice().destroyImage(vulkan::drawing::get()->getColorResources().getColorImage());
		logicalDevice->getLogicalDevice().freeMemory(vulkan::drawing::get()->getColorResources().getColorImageMemory());

		for (auto framebuffer : vulkan::drawing::get()->getFramebuffer().getSwapchainFramebuffers()) {
			logicalDevice->getLogicalDevice().destroyFramebuffer(framebuffer);
		}

		logicalDevice->getLogicalDevice().freeCommandBuffers(vulkan::drawing::get()->getCommandPool().getCommandPool(), static_cast<uint32_t>(vulkan::drawing::get()->getCommandBuffer().getCommandBuffers().size()),
			vulkan::drawing::get()->getCommandBuffer().getCommandBuffers().data());

		logicalDevice->getLogicalDevice().destroyPipeline(vulkan::pipelines::get()->getGraphicsPipeline());
		logicalDevice->getLogicalDevice().destroyPipelineLayout(vulkan::pipelines::get()->getPipelineLayout());
		logicalDevice->getLogicalDevice().destroyRenderPass(renderPass->getRenderPass());

		for (auto imageView : imageViews->getSwapchainImageViews()) {
			logicalDevice->getLogicalDevice().destroyImageView(imageView);
		}

		logicalDevice->getLogicalDevice().destroySwapchainKHR(swapchain->getSwapchain());

		for (size_t i = 0; i < swapchain->getSwapchainImages().size(); i++) {
			logicalDevice->getLogicalDevice().destroyBuffer(renderer::buffers::get()->getUniformBuffer().getUniformbuffers().at(i));
			logicalDevice->getLogicalDevice().freeMemory(renderer::buffers::get()->getUniformBuffer().getUniformbuffersMemory().at(i));
		}

		logicalDevice->getLogicalDevice().destroyDescriptorPool(renderer::buffers::get()->getDescriptorPool().getDescriptorPool());
	}

	void Graphics::cleanup()
	{
		cleanupSwapchain();

		logicalDevice->getLogicalDevice().destroySampler(vulkan::drawing::get()->getTextureSampler().getTextureSampler());
		logicalDevice->getLogicalDevice().destroyImageView(vulkan::drawing::get()->getTextureImageView().getTextureImageView());

		logicalDevice->getLogicalDevice().destroyImage(vulkan::drawing::get()->getTextureImage().getTextureImage());
		logicalDevice->getLogicalDevice().freeMemory(vulkan::drawing::get()->getTextureImage().getTextureImageMemory());

		logicalDevice->getLogicalDevice().destroyDescriptorSetLayout(renderer::buffers::get()->getDescriptorSetLayout().getDescriptorSetLayout());

		logicalDevice->getLogicalDevice().destroyBuffer(renderer::buffers::get()->getIndexBuffer().getIndexBuffer());
		logicalDevice->getLogicalDevice().freeMemory(renderer::buffers::get()->getIndexBuffer().getIndexBufferMemory());

		logicalDevice->getLogicalDevice().destroyBuffer(renderer::buffers::get()->getVertexBuffer().getVertexBuffer());
		logicalDevice->getLogicalDevice().freeMemory(renderer::buffers::get()->getVertexBuffer().getDeviceMemory());

		for (size_t i = 0; i < vulkan::drawing::MAX_FRAMES_IN_FLIGHT; i++) {
			logicalDevice->getLogicalDevice().destroySemaphore(vulkan::drawing::get()->getRenderFinishedSemaphore().at(i));
			logicalDevice->getLogicalDevice().destroySemaphore(vulkan::drawing::get()->getImageAvailabeSemaphore().at(i));
			logicalDevice->getLogicalDevice().destroyFence(vulkan::drawing::get()->getInFlightFences().at(i));
		}

		logicalDevice->getLogicalDevice().destroyCommandPool(vulkan::drawing::get()->getCommandPool().getCommandPool());

		logicalDevice->getLogicalDevice().destroy();

		if (vulkan::enableValidationLayers) {
			vulkan::debugutils::DestroyDebugUtilsMessengerEXT(instance->getInstance(), debugMessenger->getDebugMessenger(), nullptr);
		}
		
		instance->getInstance().destroySurfaceKHR(surface->getSurface());
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

	const vulkan::swapchains::ImageViews& Graphics::getImageViews() const
	{
		if (imageViews == nullptr) {
			throw std::runtime_error("Image Views have not been initalized");
		}

		return *imageViews;
	}

	const vulkan::renderpasses::RenderPass& Graphics::getRenderPass() const
	{
		if (renderPass == nullptr) {
			throw std::runtime_error("Render Pass has not been initialized");
		}

		return *renderPass;
	}

	bool& Graphics::getFramebufferResized()
	{
		return framebufferResized;
	}

	void Graphics::framebufferResizedCallback(GLFWwindow* window, int width, int height)
	{
		auto app = reinterpret_cast<Graphics*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}

	vk::ImageView Graphics::createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags, uint32_t mipLevels)
	{
		auto viewInfo = vk::ImageViewCreateInfo({}, image, vk::ImageViewType::e2D, format, {}, { aspectFlags, 0, mipLevels, 0, 1 });

		vk::ImageView imageView;

		if (graphics::get()->getLogicalDevice().getLogicalDevice().createImageView(&viewInfo, nullptr, &imageView) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create texture image view!");
		}

		return imageView;
	}

	Graphics* get()
	{
		return Graphics::_get();
	}
}