#ifndef SCISSOR_CLASS
#define SCISSOR_CLASS

#include <vulkan/vulkan.hpp>

namespace gibvk::vulkan::pipelines::scissors {
	class Scissor {
	public:
		Scissor();

		[[nodiscard]] const vk::Rect2D& getScissor() const;

	private:
		vk::Rect2D scissor{};
	};

	std::unique_ptr<Scissor> createScissor();
}

#endif