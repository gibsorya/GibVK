#include "TextureImage.hpp"
#include "../../../Renderer/Buffers/Buffers.hpp"
#include "../../../Graphics.hpp"
#include <stb_image.h>

namespace gibvk::vulkan::drawing::textureimages {
	TextureImage::TextureImage()
	{
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load(graphics::TEXTURE_PATH.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		vk::DeviceSize imageSize = texWidth * texHeight * 4;
		mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
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

		drawing::createImage(texWidth, texHeight, mipLevels, vk::Format::eR8G8B8A8Srgb, vk::ImageTiling::eOptimal, 
			vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
			vk::MemoryPropertyFlagBits::eDeviceLocal, textureImage, textureImageMemory);

		drawing::transitionImageLayout(textureImage, vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, mipLevels);
		copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
		//drawing::transitionImageLayout(textureImage, vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal, mipLevels);

		graphics::get()->getLogicalDevice().getLogicalDevice().destroyBuffer(stagingBuffer);
		graphics::get()->getLogicalDevice().getLogicalDevice().freeMemory(stagingBufferMemory);

		generateMipmaps(textureImage, vk::Format::eR8G8B8A8Srgb, texWidth, texHeight, mipLevels);
	}

	void TextureImage::copyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height)
	{
		vk::CommandBuffer commandBuffer = renderer::buffers::beginSingleTimeCommands();

		auto region = vk::BufferImageCopy(0, 0, 0, { vk::ImageAspectFlagBits::eColor, 0, 0, 1 }, {0, 0, 0}, {width, height, 1});

		commandBuffer.copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, 1, &region);

		renderer::buffers::endSingleTimeCommands(commandBuffer);
	}

	void TextureImage::generateMipmaps(vk::Image image, vk::Format imageFormat, int32_t texWidth, int32_t texHeight, int32_t mipLevels)
	{
		vk::FormatProperties formatProperties;
		graphics::get()->getPhysicalDevice().getPhysicalDevice().getFormatProperties(imageFormat, &formatProperties);

		if (!(formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImageFilterLinear)) {
			throw std::runtime_error("Texture image format does not support linear blitting!");
		}

		vk::CommandBuffer commandBuffer = renderer::buffers::beginSingleTimeCommands();

		vk::ImageMemoryBarrier barrier{};
		barrier.image = image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.subresourceRange.levelCount = 1;

		int32_t mipWidth = texWidth;
		int32_t mipHeight = texHeight;

		for (uint32_t i = 1; i < mipLevels; i++) {
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
			barrier.newLayout = vk::ImageLayout::eTransferSrcOptimal;
			barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
			barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;

			commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTransfer, {}, 0, nullptr, 0, nullptr, 1, &barrier);

			auto blit = vk::ImageBlit({ vk::ImageAspectFlagBits::eColor, i - 1, 0, 1 }, { vk::Offset3D{}, {mipWidth, mipHeight, 1} },
				{ vk::ImageAspectFlagBits::eColor, i, 0, 1 }, { vk::Offset3D{}, {mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1} });

			commandBuffer.blitImage(image, vk::ImageLayout::eTransferSrcOptimal, image, vk::ImageLayout::eTransferDstOptimal, 1, &blit, vk::Filter::eLinear);

			barrier.oldLayout = vk::ImageLayout::eTransferSrcOptimal;
			barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
			barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

			commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, {}, 0, nullptr, 0, nullptr, 1, &barrier);

			if (mipWidth > 1) mipWidth /= 2;
			if (mipHeight > 1) mipHeight /= 2;
		}

		barrier.subresourceRange.baseMipLevel = mipLevels - 1;
		barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
		barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

		commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, {}, 0, nullptr, 0, nullptr, 1, &barrier);

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

	const uint32_t& gibvk::vulkan::drawing::textureimages::TextureImage::getMipLevels() const
	{
		return mipLevels;
	}

	std::unique_ptr<TextureImage> createTextureImage()
	{
		return std::make_unique<TextureImage>();
	}


	TextureImageView::TextureImageView() 
	{
		textureImageView = graphics::get()->createImageView(drawing::get()->getTextureImage().getTextureImage(), vk::Format::eR8G8B8A8Srgb, 
			vk::ImageAspectFlagBits::eColor, drawing::get()->getTextureImage().getMipLevels());
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
			0.0f, static_cast<float>(drawing::get()->getTextureImage().getMipLevels()), vk::BorderColor::eIntOpaqueBlack, VK_FALSE);
		
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