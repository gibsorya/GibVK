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
		instance = vulkan::create();

	}

	void Graphics::render()
	{
		while (!glfwWindowShouldClose(window->getWindow())) {
			glfwPollEvents();
		}
	}

	void Graphics::cleanup()
	{
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

	Graphics* get()
	{
		return Graphics::_get();
	}
}