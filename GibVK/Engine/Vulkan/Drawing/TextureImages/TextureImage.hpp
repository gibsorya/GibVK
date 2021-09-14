#ifndef TEXTURE_IMAGE_CLASS
#define TEXTURE_IMAGE_CLASS
#define STB_IMAGE_IMPLEMENTATION

#include <vulkan/vulkan.hpp>


namespace gibvk::vulkan::drawing::textureimages {
	class TextureImage {
	public:
		TextureImage();


		void transitionImageLayout(vk::Image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);
		void copyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height);

		[[nodiscard]] const vk::Image& getTextureImage() const;
		[[nodiscard]] const vk::DeviceMemory& getTextureImageMemory() const;

	private:
		vk::Image textureImage;
		vk::DeviceMemory textureImageMemory;
	};

	std::unique_ptr<TextureImage> createTextureImage();
}

#endif // !TEXTURE_IMAGE_CLASS
