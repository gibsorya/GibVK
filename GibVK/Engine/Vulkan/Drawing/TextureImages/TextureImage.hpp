#ifndef TEXTURE_IMAGE_CLASS
#define TEXTURE_IMAGE_CLASS
#define STB_IMAGE_IMPLEMENTATION

#include <vulkan/vulkan.hpp>


namespace gibvk::vulkan::drawing::textureimages {
	class TextureImage {
	public:
		TextureImage();


		//void transitionImageLayout(vk::Image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);
		void copyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height);
		void generateMipmaps(vk::Image image, vk::Format imageFormat, int32_t texWidth, int32_t texHeight, int32_t mipLevels);

		[[nodiscard]] const vk::Image& getTextureImage() const;
		[[nodiscard]] const vk::DeviceMemory& getTextureImageMemory() const;
		[[nodiscard]] const uint32_t& getMipLevels() const;

	private:
		vk::Image textureImage;
		vk::DeviceMemory textureImageMemory;
		uint32_t mipLevels;
	};

	std::unique_ptr<TextureImage> createTextureImage();

	class TextureImageView {
	public:
		TextureImageView();

		[[nodiscard]] const vk::ImageView& getTextureImageView() const;

	private:
		vk::ImageView textureImageView;
	};

	std::unique_ptr<TextureImageView> createTextureImageView();

	class TextureSampler {
	public:
		TextureSampler();

		[[nodiscard]] const vk::Sampler& getTextureSampler() const;

	private:
		vk::Sampler textureSampler;
	};

	std::unique_ptr<TextureSampler> createTextureSampler();
}

#endif // !TEXTURE_IMAGE_CLASS
