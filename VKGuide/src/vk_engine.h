// vulkan_guide.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <vk_types.h>

class VulkanEngine {
public:

	VkInstance _instance;
	VkDebugUtilsMessengerEXT _debugMessenger;
	VkPhysicalDevice _physicalDevice;
	VkDevice _device;

	uint32_t _graphicsFamily;
	uint32_t _presentFamily;
	VkQueue _graphicsQueue;

	VkCommandPool _commandPool;
	VkCommandBuffer _commandBuffer;

	std::vector<VkFramebuffer> _frameBuffers;
	VkRenderPass _renderPass;

	VkSurfaceKHR _surface;
	VkSwapchainKHR _swapchain;
	VkFormat _swapchainImageFormat;
	std::vector<VkImage> _swapchainImages;
	std::vector<VkImageView> _swapchainImageViews;

	VkFence _renderFence;
	VkSemaphore _presentSemaphore;
	VkSemaphore _renderSemaphore;

	bool _isInitialized{ false };
	int _frameNumber {0};

	VkExtent2D _windowExtent{ 1700 , 900 };

	struct SDL_Window* _window{ nullptr };

	//initializes everything in the engine
	void init();

	//shuts down the engine
	void cleanup();

	//draw loop
	void draw();

	//run main loop
	void run();

private:
	//Vulkan initializers
	void init_vulkan();

	void init_swapchain();

	void init_commands();

	void init_renderpass();

	void init_default_renderpass();

	void init_framebuffers();

	void init_sync_objects();
};
