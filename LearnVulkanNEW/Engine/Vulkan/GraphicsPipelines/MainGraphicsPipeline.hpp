#ifndef MAIN_GRAPHICS_PIPELINE_CLASS
#define MAIN_GRAPHICS_PIPELINE_CLASS
#include <vulkan/vulkan.hpp>
#include <vector>
#include <fstream>

#include "ShaderModules/ShaderModule.hpp"

namespace gibvk::vulkan::pipelines {
	class MainGraphicsPipeline {
	public:
		MainGraphicsPipeline() = default;

		static MainGraphicsPipeline* _get();

		void initialize();

		void destroy();

		static std::vector<char> readFile(const std::string& fileName);

		[[nodiscard]] const shadermodules::ShaderModule getVertShaderModule() const;
		[[nodiscard]] const shadermodules::ShaderModule getFragShaderModule() const;
		//[[nodiscard]] const shadermodules::ShaderModule& getShaderModule() const;

	private:
		static std::unique_ptr<MainGraphicsPipeline> graphicsPipeline;

		std::unique_ptr<shadermodules::ShaderModule> vertShaderModule{};
		std::unique_ptr<shadermodules::ShaderModule> fragShaderModule{};
	};

		MainGraphicsPipeline* get();
}

#endif