#ifndef MAIN_GRAPHICS_PIPELINE_CLASS
#define MAIN_GRAPHICS_PIPELINE_CLASS
#include <vulkan/vulkan.hpp>
#include <vector>
#include <fstream>

#include "ShaderModules/ShaderModule.hpp"
#include "VertexInputs/VertexInput.hpp"
#include "InputAssemblies/InputAssembly.hpp"
#include "Viewports/Viewport.hpp"
#include "Scissors/Scissor.hpp"
#include "Rasterizers/Rasterizer.hpp"
#include "Multisamplers/Multisampling.hpp"
#include "ColorBlenders/ColorBlend.hpp"
#include "DepthStencilState/DepthStencilState.hpp"

namespace gibvk::vulkan::pipelines {
	class MainGraphicsPipeline {
	public:
		MainGraphicsPipeline() = default;

		static MainGraphicsPipeline* _get();

		void initialize();
		void destroy();

		static std::vector<char> readFile(const std::string& fileName);

		[[nodiscard]] const vertexinputs::VertexInput& getVertexInput() const;
		[[nodiscard]] const assemblies::InputAssembly& getInputAssembly() const;
		[[nodiscard]] const shadermodules::ShaderModule& getVertShaderModule() const;
		[[nodiscard]] const shadermodules::ShaderModule& getFragShaderModule() const;
		[[nodiscard]] const viewports::Viewport& getViewport() const;
		[[nodiscard]] const viewports::ViewportState& getViewportState() const;
		[[nodiscard]] const scissors::Scissor& getScissor() const;
		[[nodiscard]] const rasterizers::Rasterizer& getRasterizer() const;
		[[nodiscard]] const multisampling::Multisampling& getMultisampling() const;
		[[nodiscard]] const colorblends::ColorBlendAttachment& getColorBlendAttachment() const;
		[[nodiscard]] const colorblends::ColorBlending& getColorBlending() const;
		[[nodiscard]] const depth::DepthStencilState& getDepthStencil() const;

		[[nodiscard]] const vk::PipelineShaderStageCreateInfo& getVertShaderInfo() const;
		[[nodiscard]] const vk::PipelineShaderStageCreateInfo& getFragShaderInfo() const;
		//[[nodiscard]] const std::vector<vk::PipelineShaderStageCreateInfo>& getShaderStages() const;
		[[nodiscard]] const vk::PipelineLayout& getPipelineLayout() const;
		[[nodiscard]] const vk::PipelineLayoutCreateInfo& getPipelineLayoutInfo() const;
		[[nodiscard]] const vk::Pipeline& getGraphicsPipeline() const;

	private:
		vk::PipelineShaderStageCreateInfo vertShaderStageInfo{};
		vk::PipelineShaderStageCreateInfo fragShaderStageInfo{};
		//std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
		vk::PipelineLayout pipelineLayout;
		vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
		vk::Pipeline graphicsPipeline;

		static std::unique_ptr<MainGraphicsPipeline> mainGraphicsPipeline;

		std::unique_ptr<shadermodules::ShaderModule> vertShaderModule{};
		std::unique_ptr<shadermodules::ShaderModule> fragShaderModule{};
		std::unique_ptr<vertexinputs::VertexInput> vertexInput{};
		std::unique_ptr<assemblies::InputAssembly> inputAssembly{};
		std::unique_ptr<viewports::Viewport> viewport{};
		std::unique_ptr<viewports::ViewportState> viewportState{};
		std::unique_ptr<scissors::Scissor> scissor{};
		std::unique_ptr<rasterizers::Rasterizer> rasterizer{};
		std::unique_ptr<multisampling::Multisampling> multisampling{};
		std::unique_ptr<colorblends::ColorBlendAttachment> colorBlendAttachment{};
		std::unique_ptr<colorblends::ColorBlending> colorBlending{};
		std::unique_ptr<depth::DepthStencilState> depthStencil{};
	};

		MainGraphicsPipeline* get();
}

#endif