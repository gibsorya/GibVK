﻿// vulkan_guide.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <vk_types.h>
#include <vector>
#include <functional>
#include <deque>
#include <vk_mesh.h>
#include <glm/glm.hpp>

struct MeshPushConstants {
	glm::vec4 data;
	glm::mat4 render_matrix;
};

struct DeletionQueue
{
	std::deque<std::function<void()>> deletors;

	void push_function(std::function<void()>&& function) {
		deletors.push_back(function);
	}

	void flush() {
		// reverse iterate the deletion queue to execute all the functions
		for (auto it = deletors.rbegin(); it != deletors.rend(); it++) {
			(*it)(); //call the function
		}

		deletors.clear();
	}
};

class VulkanEngine {
public:
	DeletionQueue _mainDeletionQueue;
	VmaAllocator _allocator;

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

	VkPipelineLayout _trianglePipelineLayout;
	VkPipelineLayout _meshPipelineLayout;
	VkPipeline _trianglePipeline;
	VkPipeline _redTrianglePipeline;
	VkPipeline _meshPipeline;
	Mesh _triangleMesh;
	Mesh _monkeyMesh;
	VkBuffer _vertexBuffer;

	VkImageView _depthImageView;
	AllocatedImage _depthImage;
	VkFormat _depthFormat;

	bool _isInitialized{ false };
	int _frameNumber {0};
	int _selectedShader{ 0 };
	int _horizontal{0};
	int _vertical{0};
	int _zed{0};
	glm::vec3 camPos = { 0.f,0.f,-2.f };

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

	bool load_shader_module(const char* filePath, VkShaderModule* outShaderModule);

	void init_pipelines();

	void init_vertex_buffer();

	void load_meshes();
	void upload_mesh(Mesh& mesh);
};

class PipelineBuilder {
public:

	std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
	VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
	VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
	VkViewport _viewport;
	VkRect2D _scissor;
	VkPipelineRasterizationStateCreateInfo _rasterizer;
	VkPipelineColorBlendAttachmentState _colorBlendAttachment;
	VkPipelineMultisampleStateCreateInfo _multisampling;
	VkPipelineLayout _pipelineLayout;
	VkPipelineDepthStencilStateCreateInfo _depthStencil;

	VkPipeline build_pipeline(VkDevice device, VkRenderPass pass);
};
