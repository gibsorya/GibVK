#include "InputAssembly.hpp"
namespace gibvk::vulkan::pipelines::assemblies {
	InputAssembly::InputAssembly()
	{
		inputAssemblyInfo = vk::PipelineInputAssemblyStateCreateInfo({},vk::PrimitiveTopology::eTriangleList, VK_FALSE);
	}
	const vk::PipelineInputAssemblyStateCreateInfo& InputAssembly::getInputAssemblyInfo() const
	{
		return inputAssemblyInfo;
	}
	std::unique_ptr<InputAssembly> createInputAssembly()
	{
		return std::make_unique<InputAssembly>();
	}
}