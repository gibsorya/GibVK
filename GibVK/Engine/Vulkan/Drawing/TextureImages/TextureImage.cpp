#include "TextureImage.hpp"
#include "../../../Renderer/Buffers/Buffers.hpp"
#include "../../../Graphics.hpp"
#include <stb_image.h>

namespace gibvk::vulkan::drawing::textureimages {
	TextureImage::TextureImage()
	{
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load("textures/texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		vk::DeviceSize imageSize = texWidth * texHeight * 4;

		if (!pixels) {
			throw std::runtime_error("Failed to load texture image!");
		}

		vk::Buffer stagingBuffer;
		vk::DeviceMemory stagingBufferMemory;
		renderer::buffers::get()->createBuffer(imageSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);

		void* data;
		graphics::get()->getLogicalDevice().getLogicalDevice().mapMemory(stagingBufferMemory, 0, imageSize, {}, &data);
		memcpy(data, pixels, static_cast<size_t>(imageSize));
		graphics::get()->getLogicalDevice().getLogicalDevice().unmapMemory(stagingBufferMemory);

		stbi_image_free(pixels);

		drawing::createImage(texWidth, texHeight, vk::Format::eR8G8B8A8Srgb, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal, textureImage, textureImageMemory);

		transitionImageLayout(textureImage, vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
		copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
		transitionImageLayout(textureImage, vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

		graphics::get()->getLogicalDevice().getLogicalDevice().destroyBuffer(stagingBuffer);
		graphics::get()->getLogicalDevice().getLogicalDevice().freeMemory(stagingBufferMemory);
	}

	
	void TextureImage::transitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
	{
		vk::CommandBuffer commandBuffer = renderer::buffers::beginSingleTimeCommands();
		
		auto barrier = vk::ImageMemoryBarrier(vk::AccessFlagBits::eNoneKHR, vk::AccessFlagBits::eNoneKHR, oldLayout, newLayout, VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED, image, {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});

		vk::PipelineStageFlags sourceStage;
		vk::PipelineStageFlags destinationStage;

		if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal) {
			barrier.srcAccessMask = vk::AccessFlagBits::eNoneKHR;
			barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

			sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
			destinationStage = vk::PipelineStageFlagBits::eTransfer;
		}
		else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
			barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
			barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

			sourceStage = vk::PipelineStageFlagBits::eTransfer;
			destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
		}
		else {
			throw std::invalid_argument("Unsupported layout transition!");
		}

		commandBuffer.pipelineBarrier(sourceStage, destinationStage, {}, 0, nullptr, 0, nullptr, 1, &barrier);

		renderer::buffers::endSingleTimeCommands(commandBuffer);
	}

	void TextureImage::copyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height)
	{
		vk::CommandBuffer commandBuffer = renderer::buffers::beginSingleTimeCommands();

		auto region = vk::BufferImageCopy(0, 0, 0, { vk::ImageAspectFlagBits::eColor, 0, 0, 1 }, {0, 0, 0}, {width, height, 1});

		commandBuffer.copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, 1, &region);

		renderer::buffers::endSingleTimeCommands(commandBuffer);
	}

	const vk::Image& TextureImage::getTextureImage() const
	{
		return textureImage;
	}

	const vk::DeviceMemory& TextureImage::getTextureImageMemory() const
	{
		return textureImageMemory;
	}

	std::unique_ptr<TextureImage> createTextureImage()
	{
		return std::make_unique<TextureImage>();
	}


	TextureImageView::TextureImageView() 
	{
		textureImageView = graphics::get()->createImageView(drawing::get()->getTextureImage().getTextureImage(), vk::Format::eR8G8B8A8Srgb);
	}

	const vk::ImageView& gibvk::vulkan::drawing::textureimages::TextureImageView::getTextureImageView() const
	{
		return textureImageView;
	}

	std::unique_ptr<TextureImageView> createTextureImageView()
	{
		return std::make_unique<TextureImageView>();
	}

	TextureSampler::TextureSampler()
	{
		vk::PhysicalDeviceProperties properties{};
		graphics::get()->getPhysicalDevice().getPhysicalDevice().getProperties(&properties);

		auto samplerInfo = vk::SamplerCreateInfo({}, vk::Filter::eLinear, vk::Filter::eLinear, vk::SamplerMipmapMode::eLinear, vk::SamplerAddressMode::eRepeat, 
			vk::SamplerAddressMode::eRepeat, vk::SamplerAddressMode::eRepeat, 0.0f, VK_TRUE, properties.limits.maxSamplerAnisotropy, VK_FALSE, vk::CompareOp::eAlways,
			0.0f, 0.0f, vk::BorderColor::eIntOpaqueBlack, VK_FALSE);
		
		if (graphics::get()->getLogicalDevice().getLogicalDevice().createSampler(&samplerInfo, nullptr, &textureSampler) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create texture sampler!");
		}
	}

	const vk::Sampler& gibvk::vulkan::drawing::textureimages::TextureSampler::getTextureSampler() const
	{
		return textureSampler;
	}

	std::unique_ptr<TextureSampler> createTextureSampler()
	{
		return std::make_unique<TextureSampler>();
	}
}