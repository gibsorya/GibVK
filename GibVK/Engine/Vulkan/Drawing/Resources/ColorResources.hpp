#ifndef COLOR_RESOURCES_CLASS
#define COLOR_RESOURCES_CLASS

#include <vulkan/vulkan.hpp>

namespace gibvk::vulkan::drawing::resources {
	class ColorResources {
	public:
		ColorResources();

		[[nodiscard]] const vk::Image& getColorImage() const;
		[[nodiscard]] const vk::DeviceMemory& getColorImageMemory() const;
		[[nodiscard]] const vk::ImageView& getColorImageView() const;

	private:
		vk::Image colorImage;
		vk::DeviceMemory colorImageMemory;
		vk::ImageView colorImageView;
	};

	std::unique_ptr<ColorResources> createColorResources();
}

#endif // !COLOR_RESOURCES_CLASS
