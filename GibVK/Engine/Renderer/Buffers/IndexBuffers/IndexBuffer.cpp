#include "IndexBuffer.hpp"
#include "../../../Graphics.hpp"
#include "../Buffers.hpp"

namespace gibvk::renderer::buffers::indexbuffers {
	IndexBuffer::IndexBuffer()
	{
		vk::DeviceSize bufferSize = sizeof(indices[0]) * indices.size();

		vk::Buffer stagingBuffer;
		vk::DeviceMemory stagingBufferMemory;
		buffers::get()->createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);

		void* data;
		graphics::get()->getLogicalDevice().getLogicalDevice().mapMemory(stagingBufferMemory, 0, bufferSize, {}, &data);
			memcpy(data, indices.data(), (size_t)bufferSize);
		graphics::get()->getLogicalDevice().getLogicalDevice().unmapMemory(stagingBufferMemory);

		buffers::get()->createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, indexBuffer, indexBufferMemory);

		buffers::get()->copyBuffer(stagingBuffer, indexBuffer, bufferSize);

		graphics::get()->getLogicalDevice().getLogicalDevice().destroyBuffer(stagingBuffer);
		graphics::get()->getLogicalDevice().getLogicalDevice().freeMemory(stagingBufferMemory);
	}

	const vk::Buffer& IndexBuffer::getIndexBuffer() const
	{
		return indexBuffer;
	}

	const vk::DeviceMemory& IndexBuffer::getIndexBufferMemory() const
	{
		return indexBufferMemory;
	}

	std::unique_ptr<IndexBuffer> createIndexBuffer()
	{
		return std::make_unique<IndexBuffer>();
	}
}