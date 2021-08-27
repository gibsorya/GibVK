#include "ShaderModule.hpp"
#include "../../../Graphics.hpp"

namespace gibvk::vulkan::pipelines::shadermodules {
	ShaderModule::ShaderModule(const std::vector<char>& code)
	{
		vk::ShaderModuleCreateInfo createInfo({}, code.size(), reinterpret_cast<const uint32_t*>(code.data()));

		if (gibvk::graphics::get()->getLogicalDevice().getLogicalDevice().createShaderModule(&createInfo, nullptr, &shaderModule) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create shader module!");
		}

	}

	const vk::ShaderModule& ShaderModule::getShaderModule() const
	{
		return shaderModule;
	}

	std::unique_ptr<ShaderModule> createShaderModule(const std::vector<char>& code)
	{
		return std::make_unique<ShaderModule>(code);
	}
}