#ifndef RASTERIZER_CLASS
#define RASTERIZER_CLASS

#include <vulkan/vulkan.hpp>

namespace gibvk::vulkan::pipelines::rasterizers {
	class Rasterizer {
	public:
		Rasterizer();

		[[nodiscard]] const vk::PipelineRasterizationStateCreateInfo& getRasterizer() const;

	private:
		vk::PipelineRasterizationStateCreateInfo rasterizer{};
	};

	std::unique_ptr<Rasterizer> createRasterizer();
}

#endif // !RASTERIZER_CLASS
