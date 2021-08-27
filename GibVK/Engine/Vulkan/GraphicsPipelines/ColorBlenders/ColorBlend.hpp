#ifndef COLOR_BLEND_CLASS
#define COLOR_BLEND_CLASS

#include <vulkan/vulkan.hpp>

namespace gibvk::vulkan::pipelines::colorblends {
	class ColorBlendAttachment {
	public:
		ColorBlendAttachment();
		

		[[nodiscard]] const vk::PipelineColorBlendAttachmentState& getColorBlendAttachment() const;
		

	private:
		vk::PipelineColorBlendAttachmentState colorBlendAttachment{};
	};

	class ColorBlending {
	public:
		ColorBlending();

		[[nodiscard]] const vk::PipelineColorBlendStateCreateInfo& getColorBlending() const;

	private:
		vk::PipelineColorBlendStateCreateInfo colorBlending{};
	};

	std::unique_ptr<ColorBlendAttachment> createColorBlendAttachment();
	std::unique_ptr<ColorBlending> createColorBlending();
}

#endif // !COLOR_BLEND_CLASS
