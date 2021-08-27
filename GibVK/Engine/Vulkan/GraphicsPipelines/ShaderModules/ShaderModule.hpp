#ifndef SHADER_MODULE_CLASS
#define SHADER_MODULE_CLASS

#include <vulkan/vulkan.hpp>
#include <vector>

namespace gibvk::vulkan::pipelines::shadermodules {
	class ShaderModule {
	public:
		ShaderModule(const std::vector<char>& code);

		[[nodiscard]] const vk::ShaderModule& getShaderModule() const;

	private:
		vk::ShaderModule shaderModule;
	};

	std::unique_ptr<ShaderModule> createShaderModule(const std::vector<char>& code);
}

#endif // !SHADER_MODULE_CLASS
