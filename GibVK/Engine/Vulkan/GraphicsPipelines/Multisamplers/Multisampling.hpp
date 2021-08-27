#ifndef MULTISAMPLING_CLASS
#define MULTISAMPLING_CLASS

#include <vulkan/vulkan.hpp>

namespace gibvk::vulkan::pipelines::multisampling {
	class Multisampling {
	public:
		Multisampling();

		[[nodiscard]] const vk::PipelineMultisampleStateCreateInfo& getMultisampling() const;

	private:
		vk::PipelineMultisampleStateCreateInfo multisampling{};
	};

	std::unique_ptr<Multisampling> createMultisampling();
}

#endif // !MULTISAMPLING_CLASS
