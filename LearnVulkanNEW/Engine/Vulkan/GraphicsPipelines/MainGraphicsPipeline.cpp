#include "MainGraphicsPipeline.hpp";
#include "../../Graphics.hpp"

namespace gibvk::vulkan::pipelines {
	std::unique_ptr<MainGraphicsPipeline> MainGraphicsPipeline::graphicsPipeline = nullptr;

	MainGraphicsPipeline* MainGraphicsPipeline::_get()
	{
		if (graphicsPipeline == nullptr) {
			graphicsPipeline = std::make_unique<MainGraphicsPipeline>();

			return graphicsPipeline.get();
		}

		return graphicsPipeline.get();
	}

	void MainGraphicsPipeline::initialize()
	{
		auto vertShaderCode = readFile("shaders/vert.spv");
		auto fragShaderCode = readFile("shaders/frag.spv");

		vertShaderModule = shadermodules::createShaderModule(vertShaderCode);
		fragShaderModule = shadermodules::createShaderModule(fragShaderCode);

		vk::PipelineShaderStageCreateInfo vertShaderStageInfo({}, vk::ShaderStageFlagBits::eVertex, vertShaderModule->getShaderModule(), "main");
		vk::PipelineShaderStageCreateInfo fragShaderStageInfo({}, vk::ShaderStageFlagBits::eFragment, fragShaderModule->getShaderModule(), "main");

		vk::PipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		destroy();
	}

	void MainGraphicsPipeline::destroy()
	{
		gibvk::graphics::get()->getLogicalDevice().getLogicalDevice().destroy(vertShaderModule->getShaderModule());
		gibvk::graphics::get()->getLogicalDevice().getLogicalDevice().destroy(fragShaderModule->getShaderModule());
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

	const shadermodules::ShaderModule MainGraphicsPipeline::getVertShaderModule() const
	{
		if (vertShaderModule == nullptr) {
			throw std::runtime_error("Failed to initialize vert shader module!");
		}

		return *vertShaderModule;
	}

	const shadermodules::ShaderModule MainGraphicsPipeline::getFragShaderModule() const
	{
		if (fragShaderModule == nullptr) {
			throw std::runtime_error("Failed to initialize vert shader module!");
		}

		return *fragShaderModule;
	}

	MainGraphicsPipeline* get()
	{
		return MainGraphicsPipeline::_get();
	}
}