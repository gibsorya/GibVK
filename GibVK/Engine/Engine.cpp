#include "Engine.hpp"
#include <iostream>

namespace gibvk::vulkanengine {
	void VulkanEngine::run(const Application& application) {
		initVulkan();
		mainLoop();
		cleanup();
	}

	void VulkanEngine::initVulkan()
	{
		graphics::get()->initialize();
		std::cout << "Initialized Vulkan!" << std::endl;
	}
	void VulkanEngine::mainLoop()
	{
		std::cout << "Started main loop!" << std::endl;
		graphics::get()->render();
		std::cout << "Ended main loop!" << std::endl;
	}
	void VulkanEngine::cleanup()
	{
		std::cout << "Cleaning up..." << std::endl;
		graphics::get()->cleanup();
	}
}