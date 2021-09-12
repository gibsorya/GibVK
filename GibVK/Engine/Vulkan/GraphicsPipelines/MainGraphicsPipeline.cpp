#include "MainGraphicsPipeline.hpp"
#include "../../Graphics.hpp"
#include "../../Renderer/Buffers/Buffers.hpp"

namespace gibvk::vulkan::pipelines {
	std::unique_ptr<MainGraphicsPipeline> MainGraphicsPipeline::mainGraphicsPipeline = nullptr;

	

	MainGraphicsPipeline* MainGraphicsPipeline::_get()
	{
		if (mainGraphicsPipeline == nullptr) {
			mainGraphicsPipeline = std::make_unique<MainGraphicsPipeline>();

			return mainGraphicsPipeline.get();
		}

		return mainGraphicsPipeline.get();
	}

	void MainGraphicsPipeline::initialize()
	{
		auto vertShaderCode = readFile("shaders/vert.spv");
		auto fragShaderCode = readFile("shaders/frag.spv");

		vertShaderModule = shadermodules::createShaderModule(vertShaderCode);
		fragShaderModule = shadermodules::createShaderModule(fragShaderCode);

		vertShaderStageInfo = vk::PipelineShaderStageCreateInfo({}, vk::ShaderStageFlagBits::eVertex, vertShaderModule->getShaderModule(), "main");
		fragShaderStageInfo = vk::PipelineShaderStageCreateInfo({}, vk::ShaderStageFlagBits::eFragment, fragShaderModule->getShaderModule(), "main");
		vk::PipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		//vertexInput = vertexinputs::createVertexInput();
		auto bindingDescription = renderer::buffers::vertexbuffers::Vertex::getBindingDescription();
		auto attributeDescriptions = renderer::buffers::vertexbuffers::Vertex::getAttributeDescriptions();

		auto vertexInputInfo = vk::PipelineVertexInputStateCreateInfo({}, 1, &bindingDescription, static_cast<uint32_t>(attributeDescriptions.size()), attributeDescriptions.data());

		inputAssembly = assemblies::createInputAssembly();
		viewport = viewports::createViewport();
		scissor = scissors::createScissor();
		viewportState = viewports::createViewportState();
		rasterizer = rasterizers::createRasterizer();
		multisampling = multisampling::createMultisampling();
		colorBlendAttachment = colorblends::createColorBlendAttachment();
		colorBlending = colorblends::createColorBlending();

		pipelineLayoutInfo = vk::PipelineLayoutCreateInfo({}, 1, &renderer::buffers::get()->getDescriptorSetLayout().getDescriptorSetLayout(), 0, nullptr);

		if (gibvk::graphics::get()->getLogicalDevice().getLogicalDevice().createPipelineLayout(&pipelineLayoutInfo, nullptr, &pipelineLayout) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create pipeline layout!");
		}

		auto pipelineInfo = vk::GraphicsPipelineCreateInfo({}, 2, shaderStages, &vertexInputInfo, &inputAssembly->getInputAssemblyInfo(),
			nullptr, &viewportState->getViewportState(), &rasterizer->getRasterizer(), &multisampling->getMultisampling(), nullptr, &colorBlending->getColorBlending(), 
			nullptr, pipelineLayout, graphics::get()->getRenderPass().getRenderPass(), 0, VK_NULL_HANDLE, -1);

		if (graphics::get()->getLogicalDevice().getLogicalDevice().createGraphicsPipelines(VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create graphics pipeline!");
		}

		destroy();
	}

	void MainGraphicsPipeline::destroy()
	{
		gibvk::graphics::get()->getLogicalDevice().getLogicalDevice().destroyShaderModule(vertShaderModule->getShaderModule());
		gibvk::graphics::get()->getLogicalDevice().getLogicalDevice().destroyShaderModule(fragShaderModule->getShaderModule());
	}

	std::vector<char> MainGraphicsPipeline::readFile(const std::string& fileName)
	{
		std::ifstream file(fileName, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("Failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	const vertexinputs::VertexInput& MainGraphicsPipeline::getVertexInput() const
	{
		if (vertexInput == nullptr) {
			throw std::runtime_error("Vertex Input has not been initialized");
		}

		return *vertexInput;
	}

	const assemblies::InputAssembly& MainGraphicsPipeline::getInputAssembly() const
	{
		if (inputAssembly == nullptr) {
			throw std::runtime_error("Input assembly has not been initalized");
		}

		return *inputAssembly;
	}

	const shadermodules::ShaderModule& MainGraphicsPipeline::getVertShaderModule() const
	{
		if (vertShaderModule == nullptr) {
			throw std::runtime_error("Failed to initialize vert shader module!");
		}

		return *vertShaderModule;
	}

	const shadermodules::ShaderModule& MainGraphicsPipeline::getFragShaderModule() const
	{
		if (fragShaderModule == nullptr) {
			throw std::runtime_error("Failed to initialize vert shader module!");
		}

		return *fragShaderModule;
	}

	const viewports::Viewport& MainGraphicsPipeline::getViewport() const
	{
		if (viewport == nullptr) {
			throw std::runtime_error("Viewport has not been initialized");
		}

		return *viewport;
	}

	const viewports::ViewportState& MainGraphicsPipeline::getViewportState() const
	{
		if (viewportState == nullptr) {
			throw std::runtime_error("Viewport state has not been initialized");
		}

		return *viewportState;
	}

	const scissors::Scissor& MainGraphicsPipeline::getScissor() const
	{
		if (scissor == nullptr) {
			throw std::runtime_error("Scissor has not been initialized");
		}

		return *scissor;
	}

	const rasterizers::Rasterizer& MainGraphicsPipeline::getRasterizer() const
	{
		if (rasterizer == nullptr) {
			throw std::runtime_error("Rasterizer has not been initialized");
		}

		return *rasterizer;
	}

	const multisampling::Multisampling& MainGraphicsPipeline::getMultisampling() const
	{
		if (multisampling == nullptr) {
			throw std::runtime_error("Multisampler has not been initialized");
		}

		return *multisampling;
	}

	const colorblends::ColorBlendAttachment& MainGraphicsPipeline::getColorBlendAttachment() const
	{
		if (colorBlendAttachment == nullptr) {
			throw std::runtime_error("Color blend attachment has not been initialized");
		}

		return *colorBlendAttachment;
	}

	const colorblends::ColorBlending& MainGraphicsPipeline::getColorBlending() const
	{
		if (colorBlending == nullptr) {
			throw std::runtime_error("Color blending has not been initialized");
		}

		return *colorBlending;
	}

	const vk::PipelineShaderStageCreateInfo& MainGraphicsPipeline::getVertShaderInfo() const
	{
		return vertShaderStageInfo;
		//return vk::PipelineShaderStageCreateInfo({}, vk::ShaderStageFlagBits::eVertex, vertShaderModule->getShaderModule(), "main");
	}

	const vk::PipelineShaderStageCreateInfo& MainGraphicsPipeline::getFragShaderInfo() const
	{
		return fragShaderStageInfo;
		//return vk::PipelineShaderStageCreateInfo({}, vk::ShaderStageFlagBits::eFragment, fragShaderModule->getShaderModule(), "main");
	}

	//const std::vector<vk::PipelineShaderStageCreateInfo>& MainGraphicsPipeline::getShaderStages() const
	//{
	//	return shaderStages;
	//}

	const vk::PipelineLayout& MainGraphicsPipeline::getPipelineLayout() const
	{
		return pipelineLayout;
	}

	const vk::PipelineLayoutCreateInfo& MainGraphicsPipeline::getPipelineLayoutInfo() const
	{
		return pipelineLayoutInfo;
	}

	const vk::Pipeline& MainGraphicsPipeline::getGraphicsPipeline() const
	{
		return graphicsPipeline;
	}

	//const std::vector<vk::PipelineShaderStageCreateInfo>& MainGraphicsPipeline::getShaderStages() const
	//{
	//	return shaderStages;
	//	//auto shaderStages = { getVertShaderInfo(), getFragShaderInfo() };
	//}

	MainGraphicsPipeline* get()
	{
		return MainGraphicsPipeline::_get();
	}
}