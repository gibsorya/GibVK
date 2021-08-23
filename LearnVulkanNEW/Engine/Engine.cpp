#include "Engine.hpp"
#include <iostream>

namespace gibvk::vulkanengine {
	void VulkanEngine::run(const Application& application) {
		//initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}
	/*void VulkanEngine::initWindow()
	{
		std::cout << "Initialized Window!" << std::endl;
	}*/
	void VulkanEngine::initVulkan()
	{
		std::cout << "Initialized Vulkan!" << std::endl;
		graphics::get()->initialize();
	}
	void VulkanEngine::mainLoop()
	{
		std::cout << "Started main loop!" << std::endl;
		graphics::get()->render();
	}
	void VulkanEngine::cleanup()
	{
		std::cout << "Cleaning up..." << std::endl;
		graphics::get()->cleanup();
	}
}