#ifndef INPUT_ASSEMBLY_CLASS
#define INPUT_ASSEMBLY_CLASS

#include <vulkan/vulkan.hpp>

namespace gibvk::vulkan::pipelines::assemblies {
	class InputAssembly {
	public:
		InputAssembly();

		[[nodiscard]] const vk::PipelineInputAssemblyStateCreateInfo& getInputAssemblyInfo() const;

	private:
		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
	};

	std::unique_ptr<InputAssembly> createInputAssembly();

}

#endif // !INPUT_ASSEMBLY_CLASS


