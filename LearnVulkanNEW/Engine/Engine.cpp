#include "Engine.hpp"
#include <iostream>

namespace gibvk::vulkanengine {
	void VulkanEngine::run(const Application& application) {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}
	void VulkanEngine::initWindow()
	{
		std::cout << "Initialized Window!" << std::endl;
	}
	void VulkanEngine::initVulkan()
	{
		std::cout << "Initialized Vulkan!" << std::endl;
	}
	void VulkanEngine::mainLoop()
	{
		std::cout << "Started main loop!" << std::endl;
	}
	void VulkanEngine::cleanup()
	{
		std::cout << "Cleaning up..." << std::endl;
	}
}