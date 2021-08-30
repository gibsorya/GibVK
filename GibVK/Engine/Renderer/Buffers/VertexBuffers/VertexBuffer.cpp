#include "VertexBuffer.hpp"
#include "../../../Graphics.hpp"
#include "../Buffers.hpp"

namespace gibvk::renderer::buffers::vertexbuffers {
	struct Vertex vertex;

	VertexBuffer::VertexBuffer()
	{
		vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

		vk::Buffer stagingBuffer;
		vk::DeviceMemory stagingBufferMemory;
		buffers::get()->createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);

		void* data;
		graphics::get()->getLogicalDevice().getLogicalDevice().mapMemory(stagingBufferMemory, 0, bufferSize, {}, &data);
			memcpy(data, vertices.data(), (size_t)bufferSize);
		graphics::get()->getLogicalDevice().getLogicalDevice().unmapMemory(stagingBufferMemory);
		buffers::get()->createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, vertexBuffer, vertexBufferMemory);

		buffers::get()->copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
		graphics::get()->getLogicalDevice().getLogicalDevice().destroyBuffer(stagingBuffer);
		graphics::get()->getLogicalDevice().getLogicalDevice().freeMemory(stagingBufferMemory, nullptr);
	}

	const vk::Buffer& VertexBuffer::getVertexBuffer() const
	{
		return vertexBuffer;
	}

	const vk::DeviceMemory& VertexBuffer::getDeviceMemory() const
	{
		return vertexBufferMemory;
	}

	std::unique_ptr<VertexBuffer> createVertexBuffer()
	{
		return std::make_unique<VertexBuffer>();
	}
}