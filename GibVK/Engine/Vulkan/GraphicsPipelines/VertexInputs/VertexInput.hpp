#ifndef VERTEX_INPUT_CLASS
#define VERTEX_INPUT_CLASS

#include <vulkan/vulkan.hpp>

namespace gibvk::vulkan::pipelines::vertexinputs {
	class VertexInput 
	{
	public:
		VertexInput();
		
		[[nodiscard]] const vk::PipelineVertexInputStateCreateInfo& getVertexInputInfo() const;

	private:
		vk::PipelineVertexInputStateCreateInfo vertexInputInfo{};
	};

	std::unique_ptr<VertexInput> createVertexInput();

}


#endif // !VERTEX_INPUT_CLASS
