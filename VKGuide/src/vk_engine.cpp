
#include "vk_engine.h"

#include <SDL.h>
#include <SDL_vulkan.h>

#include <vk_types.h>
#include <vk_initializers.h>

#include <iostream>
#include <cmath>
#include <fstream>
#include "VkBootstrap.h"
#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"
#include <glm/gtx/transform.hpp>

#ifdef NDEBUG
	const static bool enableValidationLayers = false;
#else
	const static bool enableValidationLayers = true;
#endif

#define VK_CHECK(x)                                                 \
	do                                                              \
	{                                                               \
		VkResult err = x;                                           \
		if (err)                                                    \
		{                                                           \
			std::cout <<"Detected Vulkan error: " << err << std::endl; \
			abort();                                                \
		}                                                           \
	} while (0)

void VulkanEngine::init()
{
	// We initialize SDL and create a window with it. 
	SDL_Init(SDL_INIT_VIDEO);

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN);
	
	_window = SDL_CreateWindow(
		"Vulkan Engine",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		_windowExtent.width,
		_windowExtent.height,
		window_flags
	);

	init_vulkan();
	init_swapchain();
	init_commands();
	init_default_renderpass();
	init_framebuffers();
	init_sync_objects();
	init_descriptors();
	init_pipelines();
	load_meshes();
	init_scene();
	//everything went fine
	_isInitialized = true;
}
void VulkanEngine::cleanup()
{	
	if (_isInitialized) {
		vkDeviceWaitIdle(_device);

		_mainDeletionQueue.flush();

		vkDestroyDevice(_device, nullptr);
		vkDestroySurfaceKHR(_instance, _surface, nullptr);
		vkb::destroy_debug_utils_messenger(_instance, _debugMessenger, nullptr);
		vkDestroyInstance(_instance, nullptr);

		SDL_DestroyWindow(_window);
	}
}

void VulkanEngine::draw()
{
	if (SDL_GetWindowFlags(_window) & SDL_WINDOW_MINIMIZED)
		return;

	vkWaitForFences(_device, 1, &get_current_frame()._renderFence, true, UINT64_MAX);
	vkResetFences(_device, 1, &get_current_frame()._renderFence);

	VK_CHECK(vkResetCommandBuffer(get_current_frame()._mainCommandBuffer, 0));

	uint32_t swapchainImageIndex;
	VK_CHECK(vkAcquireNextImageKHR(_device, _swapchain, UINT64_MAX, get_current_frame()._presentSemaphore, nullptr, &swapchainImageIndex));
	
	VkCommandBuffer cmd = get_current_frame()._mainCommandBuffer;

	VkCommandBufferBeginInfo beginInfo = vkinit::commandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	vkBeginCommandBuffer(cmd, &beginInfo);

	VkClearValue clearValue;
	float flash = abs(sin(_frameNumber / 120.f));
	clearValue.color = { { 0.0f, 0.0f, flash, 1.0f}};

	VkClearValue depthClear;
	depthClear.depthStencil.depth = 1.0f;

	VkClearValue clearValues[] = { clearValue, depthClear };

	VkRenderPassBeginInfo rpInfo = vkinit::renderPassBeginInfo(_renderPass, _windowExtent, _frameBuffers[swapchainImageIndex]);
	rpInfo.clearValueCount = 2;
	rpInfo.pClearValues = &clearValues[0];
	vkCmdBeginRenderPass(cmd, &rpInfo, VK_SUBPASS_CONTENTS_INLINE);

	draw_objects(cmd, _renderables.data(), _renderables.size());

	vkCmdEndRenderPass(cmd);
	VK_CHECK(vkEndCommandBuffer(cmd));

	VkSubmitInfo submit = vkinit::submitInfo(&cmd);
	VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	submit.pWaitDstStageMask = &waitStage;
	submit.waitSemaphoreCount = 1;
	submit.signalSemaphoreCount = 1;
	submit.pSignalSemaphores = &get_current_frame()._renderSemaphore;
	submit.pWaitSemaphores = &get_current_frame()._presentSemaphore;

	VK_CHECK(vkQueueSubmit(_graphicsQueue, 1, &submit, get_current_frame()._renderFence));

	VkPresentInfoKHR presentInfo = vkinit::presentInfo();
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &_swapchain;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &get_current_frame()._renderSemaphore;
	presentInfo.pImageIndices = &swapchainImageIndex;

	VK_CHECK(vkQueuePresentKHR(_graphicsQueue, &presentInfo));

	_frameNumber++;
}

void VulkanEngine::run()
{
	SDL_Event e;
	bool bQuit = false;

	//main loop
	while (!bQuit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			// close the window when user alt-f4s or clicks the X button
			switch (e.type)
			{
			case SDL_QUIT:
				bQuit = true;
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
				case SDLK_SPACE:
					_zed = 1.0f;
					break;
				case SDLK_LCTRL:
					_zed = -1.0f;
					break;
				case SDLK_w:
					_vertical = 1.0f;
					break;
				case SDLK_s:
					_vertical = -1.0f;
					break;
				case SDLK_a:
					_horizontal = -1.0f;
					break;
				case SDLK_d:
					_horizontal = 1.0f;
					break;
				default:
					break;
				}
				break;
			case SDL_KEYUP:
				switch (e.key.keysym.sym)
				{
				case SDLK_SPACE:
					if (_zed > 0)
						_zed = 0.0f;
					break;
				case SDLK_LCTRL:
					if (_zed < 0)
						_zed = 0.0f;
					break;
				case SDLK_w:
					if (_vertical > 0)
						_vertical = 0.0f;
					break;
				case SDLK_s:
					if (_vertical < 0)
						_vertical = 0.0f;
					break;
				case SDLK_a:
					if (_horizontal < 0)
						_horizontal = 0.0f;
					break;
				case SDLK_d:
					if (_horizontal > 0)
						_horizontal = 0.0f;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}

		draw();
	}
}

void VulkanEngine::init_scene()
{
	RenderObject monkey;
	monkey.mesh = get_mesh("monkey");
	monkey.material = get_material("defaultmesh");
	monkey.transformMatrix = glm::mat4{ 1.0f };

	_renderables.push_back(monkey);

	for (int x = -20; x <= 20; x++) {
		for (int y = -20; y <= 20; y++) {

			RenderObject tri;
			tri.mesh = get_mesh("triangle");
			tri.material = get_material("defaultmesh");
			glm::mat4 translation = glm::translate(glm::mat4{ 1.0 }, glm::vec3(x, 0, y));
			glm::mat4 scale = glm::scale(glm::mat4{ 1.0 }, glm::vec3(0.2, 0.2, 0.2));
			tri.transformMatrix = translation * scale;

			_renderables.push_back(tri);
		}
	}
}

void VulkanEngine::init_vulkan()
{
	vkb::InstanceBuilder builder;
	auto inst_build = builder.set_app_name("Vulkan Guide").set_engine_name("Awesome Game Engine").request_validation_layers(enableValidationLayers).require_api_version(1, 3, 0).use_default_debug_messenger().build();

	if (!inst_build){
		std::cerr << "Failed to create Vulkan instance. Error: " << inst_build.error().message() << std::endl;
	}
	auto vkb_instance = inst_build.value();
	_instance = vkb_instance.instance;
	_debugMessenger = vkb_instance.debug_messenger;

	SDL_Vulkan_CreateSurface(_window, _instance, &_surface);

	vkb::PhysicalDeviceSelector phys_device_selector(vkb_instance);

	auto phys_device_selector_return = phys_device_selector.set_surface(_surface).set_minimum_version(1, 1).add_required_extension({VK_KHR_SWAPCHAIN_EXTENSION_NAME}).prefer_gpu_device_type(vkb::PreferredDeviceType::discrete).select();
	if (!phys_device_selector_return){
		std::cerr << "Failed to find Physical Device. Error: " << phys_device_selector_return.error().message() << std::endl;
	}
	auto phys_device = phys_device_selector_return.value();

	_gpuProperties = phys_device.properties;

	std::cout << "The GPU has a minimum buffer alignment of " << _gpuProperties.limits.minUniformBufferOffsetAlignment << std::endl;

	_physicalDevice = phys_device.physical_device;

	vkb::DeviceBuilder deviceBuilder {phys_device};
	auto device_return = deviceBuilder.build();
	if (!device_return){
		std::cerr << "Failed to create Device. Error: " << device_return.error().message() << std::endl;
	}
	vkb::Device vkb_device = device_return.value();
	_device = vkb_device.device;

	VmaAllocatorCreateInfo allocatorInfo = {};
	allocatorInfo.device = _device;
	allocatorInfo.physicalDevice = _physicalDevice;
	allocatorInfo.instance = _instance;
	vmaCreateAllocator(&allocatorInfo, &_allocator);

	_mainDeletionQueue.push_function([&, *this]() {
		vmaDestroyAllocator(_allocator);
	});
	
	_graphicsQueue = vkb_device.get_queue(vkb::QueueType::graphics).value();
	_graphicsFamily = vkb_device.get_queue_index(vkb::QueueType::graphics).value();
	_presentFamily = vkb_device.get_queue_index(vkb::QueueType::present).value();
}

void VulkanEngine::init_swapchain()
{
	vkb::SwapchainBuilder swapchain_builder{_physicalDevice, _device, _surface};
	auto vkb_swapchain = swapchain_builder
		.use_default_format_selection()
		.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
		.set_desired_extent(_windowExtent.width, _windowExtent.height)
		.build()
		.value();

	_swapchain = vkb_swapchain.swapchain;
	_swapchainImageFormat = vkb_swapchain.image_format;
	_swapchainImages = vkb_swapchain.get_images().value();
	_swapchainImageViews = vkb_swapchain.get_image_views().value();

	_mainDeletionQueue.push_function([=, *this]() {
		vkDestroySwapchainKHR(_device, _swapchain, nullptr);
	});

	VkExtent3D depthImageExtent = {
		_windowExtent.width,
		_windowExtent.height,
		1};

	_depthFormat = VK_FORMAT_D32_SFLOAT;

	VkImageCreateInfo dimg_info = vkinit::imageCreateInfo(_depthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, depthImageExtent);

	VmaAllocationCreateInfo dimg_allocinfo = {};
	dimg_allocinfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
	dimg_allocinfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	vmaCreateImage(_allocator, &dimg_info, &dimg_allocinfo, &_depthImage._image, &_depthImage._allocation, nullptr);

	VkImageViewCreateInfo dview_info = vkinit::imageViewCreateInfo(_depthFormat, _depthImage._image, VK_IMAGE_ASPECT_DEPTH_BIT);

	VK_CHECK(vkCreateImageView(_device, &dview_info, nullptr, &_depthImageView));

	_mainDeletionQueue.push_function([=, *this]() {
		vkDestroyImageView(_device, _depthImageView, nullptr);
		vmaDestroyImage(_allocator, _depthImage._image, _depthImage._allocation);
	});
}

void VulkanEngine::init_commands()
{
	VkCommandPoolCreateInfo cmdCreateInfo = vkinit::commandPoolCreateInfo(_graphicsFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	
	for(int i = 0; i < FRAME_OVERLAP; i++)
	{
		VK_CHECK(vkCreateCommandPool(_device, &cmdCreateInfo, nullptr, &_frames[i]._commandPool));

		VkCommandBufferAllocateInfo allocInfo = vkinit::commandBufferAllocateInfo(_frames[i]._commandPool, 1);

		VK_CHECK(vkAllocateCommandBuffers(_device, &allocInfo, &_frames[i]._mainCommandBuffer));

		_mainDeletionQueue.push_function([=, *this]() {
			vkDestroyCommandPool(_device,  _frames[i]._commandPool, nullptr);
		});
	}
}

void VulkanEngine::init_renderpass()
{

}

void VulkanEngine::init_default_renderpass()
{

	VkAttachmentDescription colorAttachment = {};
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.format = _swapchainImageFormat;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription depthAttachment = {};
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.format = _depthFormat;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkSubpassDependency depth_dependency = {};
	depth_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	depth_dependency.dstSubpass = 0;
	depth_dependency.srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	depth_dependency.srcAccessMask = 0;
	depth_dependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	depth_dependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	VkSubpassDependency dependencies[2] = {dependency, depth_dependency};

	VkAttachmentDescription attachments[2] = {colorAttachment, depthAttachment};

	VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.pNext = nullptr;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.attachmentCount = 2;
    renderPassInfo.pAttachments = &attachments[0];
	renderPassInfo.dependencyCount = 2;
	renderPassInfo.pDependencies = &dependencies[0];

	VK_CHECK(vkCreateRenderPass(_device, &renderPassInfo, nullptr, &_renderPass));

	_mainDeletionQueue.push_function([=, *this]() {
		vkDestroyRenderPass(_device, _renderPass, nullptr);
    });
}

void VulkanEngine::init_framebuffers()
{
	VkFramebufferCreateInfo createInfo = vkinit::framebufferCreateInfo(_renderPass, _windowExtent);

	_frameBuffers = std::vector<VkFramebuffer>(_swapchainImages.size());

	for(int i = 0; i < _swapchainImages.size(); i++)
	{
		VkImageView attachments[2];
		attachments[0] = _swapchainImageViews[i];
		attachments[1] = _depthImageView;

		createInfo.pAttachments = attachments;
		createInfo.attachmentCount = 2;
		VK_CHECK(vkCreateFramebuffer(_device, &createInfo, nullptr, &_frameBuffers[i]));

		_mainDeletionQueue.push_function([=, *this]() {
			vkDestroyFramebuffer(_device, _frameBuffers[i], nullptr);
			vkDestroyImageView(_device, _swapchainImageViews[i], nullptr);
    	});
	}
}

void VulkanEngine::init_sync_objects()
{
	VkSemaphoreCreateInfo semCreateInfo = vkinit::semaphoreCreateInfo();
	VkFenceCreateInfo fenceCreateInfo = vkinit::fenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);

	for(int i = 0; i < FRAME_OVERLAP; i++)
	{
		VK_CHECK(vkCreateSemaphore(_device, &semCreateInfo, nullptr, &_frames[i]._presentSemaphore));
		VK_CHECK(vkCreateSemaphore(_device, &semCreateInfo, nullptr,  &_frames[i]._renderSemaphore));

		VK_CHECK(vkCreateFence(_device, &fenceCreateInfo, nullptr, &_frames[i]._renderFence));

		_mainDeletionQueue.push_function([=, *this]() {
			vkDestroyFence(_device, _frames[i]._renderFence, nullptr);
			vkDestroySemaphore(_device, _frames[i]._presentSemaphore, nullptr);
			vkDestroySemaphore(_device, _frames[i]._renderSemaphore, nullptr);
    	});
	}
}

void VulkanEngine::init_pipelines()
{
	VkShaderModule colorMeshShader;
	if (!load_shader_module("../shaders/default_lit.frag.spv", &colorMeshShader))
	{
		std::cout << "Error when building the triangle fragment shader module" << std::endl;
	}
	else {
		std::cout << "Triangle fragment shader successfully loaded" << std::endl;
	}

	VkShaderModule meshVertShader;
	if (!load_shader_module("../shaders/tri_mesh_descriptors.vert.spv", &meshVertShader))
	{
		std::cout << "Error when building the triangle vertex shader module" << std::endl;

	}
	else {
		std::cout << "Triangle vertex shader successfully loaded" << std::endl;
	}

	PipelineBuilder pipelineBuilder;
	pipelineBuilder._shaderStages.push_back(vkinit::pipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, meshVertShader));
	pipelineBuilder._shaderStages.push_back(vkinit::pipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, colorMeshShader));

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = vkinit::pipelineLayoutCreateInfo();

	VkPushConstantRange pushConstant;
	pushConstant.offset = 0;
	pushConstant.size = sizeof(MeshPushConstants);
	pushConstant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	pipelineLayoutInfo.pPushConstantRanges = &pushConstant;
	pipelineLayoutInfo.pushConstantRangeCount = 1;

	VkDescriptorSetLayout setLayouts[] = { _globalSetLayout };
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = setLayouts;

	VkPipelineLayout pipeLayout;
	VK_CHECK(vkCreatePipelineLayout(_device, &pipelineLayoutInfo, nullptr, &pipeLayout));

	pipelineBuilder._pipelineLayout = pipeLayout;

	pipelineBuilder._vertexInputInfo = vkinit::vertexInputStateCreateInfo();

	pipelineBuilder._inputAssembly = vkinit::inputAssemblyCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
	
	pipelineBuilder._viewport.x = 0.0f;
	pipelineBuilder._viewport.y = 0.0f;
	pipelineBuilder._viewport.width = (float)_windowExtent.width;
	pipelineBuilder._viewport.height = (float)_windowExtent.height;
	pipelineBuilder._viewport.minDepth = 0.0f;
	pipelineBuilder._viewport.maxDepth = 1.0f;

	pipelineBuilder._scissor.extent = _windowExtent;
	pipelineBuilder._scissor.offset = { 0, 0 };

	pipelineBuilder._rasterizer = vkinit::rasterizationStateCreateInfo(VK_POLYGON_MODE_FILL);

	pipelineBuilder._colorBlendAttachment = vkinit::colorBlendAttachmentState();

	pipelineBuilder._multisampling = vkinit::multisampleCreateInfo();

	pipelineBuilder._depthStencil = vkinit::depthStencilCreateInfo(true, true, VK_COMPARE_OP_LESS_OR_EQUAL);

	VertexInputDescription vertexDescription = Vertex::get_vertex_description();
	pipelineBuilder._vertexInputInfo.pVertexAttributeDescriptions = vertexDescription.attributes.data();
	pipelineBuilder._vertexInputInfo.vertexAttributeDescriptionCount = vertexDescription.attributes.size();

	pipelineBuilder._vertexInputInfo.pVertexBindingDescriptions = vertexDescription.bindings.data();
	pipelineBuilder._vertexInputInfo.vertexBindingDescriptionCount = vertexDescription.bindings.size();

	VkPipeline pipeline = pipelineBuilder.build_pipeline(_device, _renderPass);

	//clear the shader stages for the builder
	create_material(pipeline, pipeLayout, "defaultmesh");

	vkDestroyShaderModule(_device, meshVertShader, nullptr);
	vkDestroyShaderModule(_device, colorMeshShader, nullptr);

	_mainDeletionQueue.push_function([=, *this]() {
		vkDestroyPipeline(_device, pipeline, nullptr);

		vkDestroyPipelineLayout(_device, pipeLayout, nullptr); });
}

bool VulkanEngine::load_shader_module(const char* filePath, VkShaderModule* outShaderModule)
{
	std::ifstream file(filePath, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		return false;
	}

	//find what the size of the file is by looking up the location of the cursor
	//because the cursor is at the end, it gives the size directly in bytes
	size_t fileSize = (size_t)file.tellg();

	//spirv expects the buffer to be on uint32, so make sure to reserve an int vector big enough for the entire file
	std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

	//put file cursor at beginning
	file.seekg(0);

	//load the entire file into the buffer
	file.read((char*)buffer.data(), fileSize);

	//now that the file is loaded into the buffer, we can close it
	file.close();

	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.codeSize = buffer.size() * sizeof(uint32_t);
	createInfo.pCode = buffer.data();

	VkShaderModule shaderModule;
	if(vkCreateShaderModule(_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
	{
		return false;
	}

	*outShaderModule = shaderModule;
	return true;
}

void VulkanEngine::init_vertex_buffer()
{
	_mainDeletionQueue.push_function([=, *this]() {
		vkDestroyBuffer(_device, _vertexBuffer, nullptr); });
}

void VulkanEngine::load_meshes()
{
	Mesh triMesh{};
	triMesh._vertices.resize(3);

	triMesh._vertices[0].position = { 1.f, 1.f, 0.0f };
	triMesh._vertices[1].position = {-1.f, 1.f, 0.0f };
	triMesh._vertices[2].position = { 0.f,-1.f, 0.0f };

	triMesh._vertices[0].color = { 1.f, 0.f, 0.0f };
	triMesh._vertices[1].color = { 0.f, 1.f, 0.0f };
	triMesh._vertices[2].color = { 0.f, 0.f, 1.0f };

	Mesh monkeyMesh{};
	monkeyMesh.load_from_obj("../assets/monkey_smooth.obj");

	upload_mesh(triMesh);
	upload_mesh(monkeyMesh);

	_meshes["monkey"] = monkeyMesh;
	_meshes["triangle"] = triMesh;
}

void VulkanEngine::upload_mesh(Mesh& mesh)
{
	//allocate vertex buffer
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	//this is the total size, in bytes, of the buffer we are allocating
	bufferInfo.size = mesh._vertices.size() * sizeof(Vertex);
	//this buffer is going to be used as a Vertex Buffer
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;


	//let the VMA library know that this data should be writeable by CPU, but also readable by GPU
	VmaAllocationCreateInfo vmaallocInfo = {};
	vmaallocInfo.usage = VMA_MEMORY_USAGE_AUTO;
	vmaallocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
	vmaallocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

	//allocate the buffer
	VK_CHECK(vmaCreateBuffer(_allocator, &bufferInfo, &vmaallocInfo,
		&mesh._vertexBuffer._buffer,
		&mesh._vertexBuffer._allocation,
		nullptr));

	//add the destruction of triangle mesh buffer to the deletion queue
	_mainDeletionQueue.push_function([=, *this]() {
        vmaDestroyBuffer(_allocator, mesh._vertexBuffer._buffer, mesh._vertexBuffer._allocation);
    });

	void* data;
	vmaMapMemory(_allocator, mesh._vertexBuffer._allocation, &data);

	memcpy(data, mesh._vertices.data(), mesh._vertices.size() * sizeof(Vertex));

	vmaUnmapMemory(_allocator, mesh._vertexBuffer._allocation);
}

Material* VulkanEngine::create_material(VkPipeline pipeline, VkPipelineLayout layout, const std::string& name)
{
	Material mat;
	mat.pipeline = pipeline;
	mat.pipelineLayout = layout;
	_materials[name] = mat;
	return &_materials[name];
}

Material* VulkanEngine::get_material(const std::string& name)
{
	//search for the object, and return nullptr if not found
	auto it = _materials.find(name);
	if (it == _materials.end()) {
		return nullptr;
	}
	else {
		return &(*it).second;
	}
}


Mesh* VulkanEngine::get_mesh(const std::string& name)
{
	auto it = _meshes.find(name);
	if (it == _meshes.end()) {
		return nullptr;
	}
	else {
		return &(*it).second;
	}
}

FrameData& VulkanEngine::get_current_frame()
{
	return _frames[_frameNumber % FRAME_OVERLAP];
}

AllocatedBuffer VulkanEngine::create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage)
{
	//allocate vertex buffer
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.pNext = nullptr;

	bufferInfo.size = allocSize;
	bufferInfo.usage = usage;

	VmaAllocationCreateInfo vmaallocInfo = {};
	vmaallocInfo.usage = memoryUsage;
	vmaallocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
	vmaallocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

	AllocatedBuffer newBuffer;

	//allocate the buffer
	VK_CHECK(vmaCreateBuffer(_allocator, &bufferInfo, &vmaallocInfo,
		&newBuffer._buffer,
		&newBuffer._allocation,
		nullptr));

	return newBuffer;
}

void VulkanEngine::init_descriptors()
{
	std::vector<VkDescriptorPoolSize> sizes =
	{
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 10 }
	};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = 0;
	pool_info.maxSets = 10;
	pool_info.poolSizeCount = (uint32_t)sizes.size();
	pool_info.pPoolSizes = sizes.data();

	vkCreateDescriptorPool(_device, &pool_info, nullptr, &_descriptorPool);

	VkDescriptorSetLayoutBinding cameraBind = vkinit::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);

	VkDescriptorSetLayoutBinding sceneBind = vkinit::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 1);

	VkDescriptorSetLayoutBinding bindings[] = { cameraBind,sceneBind };

	VkDescriptorSetLayoutCreateInfo setinfo = {};
	setinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	setinfo.pNext = nullptr;
	setinfo.bindingCount = 2;
	setinfo.flags = 0;
	setinfo.pBindings = bindings;

	vkCreateDescriptorSetLayout(_device, &setinfo, nullptr, &_globalSetLayout);

	const size_t sceneParamBufferSize = FRAME_OVERLAP * pad_uniform_buffer_size(sizeof(GPUSceneData));
	_sceneParameterBuffer = create_buffer(sceneParamBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_AUTO);

	for (int i = 0; i < FRAME_OVERLAP; i++)
	{
		_frames[i].cameraBuffer = create_buffer(sizeof(GPUCameraData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_AUTO);
		
		VkDescriptorSetAllocateInfo allocInfo ={};
		allocInfo.pNext = nullptr;
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		//using the pool we just set
		allocInfo.descriptorPool = _descriptorPool;
		//only 1 descriptor
		allocInfo.descriptorSetCount = 1;
		//using the global data layout
		allocInfo.pSetLayouts = &_globalSetLayout;

		vkAllocateDescriptorSets(_device, &allocInfo, &_frames[i].globalDescriptor);

		VkDescriptorBufferInfo cameraInfo;
		//it will be the camera buffer
		cameraInfo.buffer = _frames[i].cameraBuffer._buffer;
		//at 0 offset
		cameraInfo.offset = 0;
		//of the size of a camera data struct
		cameraInfo.range = sizeof(GPUCameraData);

		VkDescriptorBufferInfo sceneInfo;
		sceneInfo.buffer = _sceneParameterBuffer._buffer;
		sceneInfo.offset = 0;
		sceneInfo.range = sizeof(GPUSceneData);

		VkWriteDescriptorSet cameraWrite = vkinit::writeDescriptorBuffer(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,  _frames[i].globalDescriptor, &cameraInfo, 0);
		VkWriteDescriptorSet sceneWrite = vkinit::writeDescriptorBuffer(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, _frames[i].globalDescriptor, &sceneInfo, 1);
		VkWriteDescriptorSet setWrites[] = { cameraWrite,sceneWrite };

		vkUpdateDescriptorSets(_device, 2, setWrites, 0, nullptr);
	}

	_mainDeletionQueue.push_function([&]() {
		vkDestroyDescriptorSetLayout(_device, _globalSetLayout, nullptr);
		vkDestroyDescriptorPool(_device, _descriptorPool, nullptr);
		vmaDestroyBuffer(_allocator, _sceneParameterBuffer._buffer, _sceneParameterBuffer._allocation);

		for (int i = 0; i < FRAME_OVERLAP; i++)
		{
			vmaDestroyBuffer(_allocator,_frames[i].cameraBuffer._buffer, _frames[i].cameraBuffer._allocation);

			// vmaDestroyBuffer(_allocator, _frames[i].objectBuffer._buffer, _frames[i].objectBuffer._allocation);
		}
	});
}


void VulkanEngine::draw_objects(VkCommandBuffer cmd,RenderObject* first, int count)
{
	camPos.x -= _horizontal * 0.15f;
	camPos.z += _vertical * 0.15f;
	camPos.y -= _zed * 0.15f;

	glm::mat4 view = glm::translate(glm::mat4(1.f), camPos);
	//camera projection
	glm::mat4 projection = glm::perspective(glm::radians(70.f), 1700.f / 900.f, 0.1f, 200.0f);
	projection[1][1] *= -1;

	GPUCameraData camData;
	camData.proj = projection;
	camData.view = view;
	camData.viewproj = projection * view;

	void* data;
	vmaMapMemory(_allocator, get_current_frame().cameraBuffer._allocation, &data);
	memcpy(data, &camData, sizeof(GPUCameraData));
	vmaUnmapMemory(_allocator, get_current_frame().cameraBuffer._allocation);

	float framed = (_frameNumber / 120.f);
	_sceneParameters.ambientColor = { sin(framed), 0, cos(framed), 1};

	char* sceneData;
	vmaMapMemory(_allocator, _sceneParameterBuffer._allocation, (void**)&sceneData);
	int frameIndex = _frameNumber % FRAME_OVERLAP;

	sceneData += pad_uniform_buffer_size(sizeof(GPUSceneData)) * frameIndex;
	memcpy(sceneData, &_sceneParameters, sizeof(GPUSceneData));
	vmaUnmapMemory(_allocator, _sceneParameterBuffer._allocation);

	Mesh* lastMesh = nullptr;
	Material* lastMaterial = nullptr;
	for (int i = 0; i < count; i++)
	{
		RenderObject& object = first[i];

		//only bind the pipeline if it doesn't match with the already bound one
		if (object.material != lastMaterial) {

			vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, object.material->pipeline);
			lastMaterial = object.material;

			uint32_t uniform_offset = pad_uniform_buffer_size(sizeof(GPUSceneData)) * frameIndex;

			vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, object.material->pipelineLayout, 0, 1, &get_current_frame().globalDescriptor, 1, &uniform_offset);
		}


		glm::mat4 model = object.transformMatrix;
		//final render matrix, that we are calculating on the cpu
		glm::mat4 mesh_matrix = projection * view * model;

		MeshPushConstants constants;
		constants.render_matrix = object.transformMatrix;

		//upload the mesh to the GPU via push constants
		vkCmdPushConstants(cmd, object.material->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(MeshPushConstants), &constants);

		//only bind the mesh if it's a different one from last bind
		if (object.mesh != lastMesh) {
			//bind the mesh vertex buffer with offset 0
			VkDeviceSize offset = 0;
			vkCmdBindVertexBuffers(cmd, 0, 1, &object.mesh->_vertexBuffer._buffer, &offset);
			lastMesh = object.mesh;
		}
		//we can now draw
		vkCmdDraw(cmd, object.mesh->_vertices.size(), 1, 0, 0);
	}
}

VkPipeline PipelineBuilder::build_pipeline(VkDevice device, VkRenderPass pass)
{
	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.pNext = nullptr;

	viewportState.viewportCount = 1;
	viewportState.pViewports = &_viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &_scissor;

	//setup dummy color blending. We aren't using transparent objects yet
	//the blending is just "no blend", but we do write to the color attachment
	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.pNext = nullptr;

	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &_colorBlendAttachment;

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.pNext = nullptr;

	pipelineInfo.pDepthStencilState = &_depthStencil;
	pipelineInfo.stageCount = _shaderStages.size();
	pipelineInfo.pStages = _shaderStages.data();
	pipelineInfo.pVertexInputState = &_vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &_inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &_rasterizer;
	pipelineInfo.pMultisampleState = &_multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = _pipelineLayout;
	pipelineInfo.renderPass = pass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	VkPipeline newPipeline;
	if (vkCreateGraphicsPipelines(
		device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &newPipeline) != VK_SUCCESS) {
		std::cout << "failed to create pipeline\n";
		return VK_NULL_HANDLE; // failed to create graphics pipeline
	}
	else
	{
		return newPipeline;
	}
}

size_t VulkanEngine::pad_uniform_buffer_size(size_t originalSize)
{
	// Calculate required alignment based on minimum device offset alignment
	size_t minUboAlignment = _gpuProperties.limits.minUniformBufferOffsetAlignment;
	size_t alignedSize = originalSize;
	if (minUboAlignment > 0) {
		alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);
	}
	return alignedSize;
}

