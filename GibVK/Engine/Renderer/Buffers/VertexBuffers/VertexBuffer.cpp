#include "VertexBuffer.hpp"
#include "../../../Graphics.hpp"

namespace gibvk::renderer::buffers::vertexbuffers {
	struct Vertex vertex;

	VertexBuffer::VertexBuffer()
	{
		auto bufferInfo = vk::BufferCreateInfo({}, (sizeof(vertices[0]) * vertices.size()), vk::BufferUsageFlagBits::eVertexBuffer, vk::SharingMode::eExclusive);

		if (graphics::get()->getLogicalDevice().getLogicalDevice().createBuffer(&bufferInfo, nullptr, &vertexBuffer) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create vertex buffer!");
		}

		vk::MemoryRequirements memRequirements;
		graphics::get()->getLogicalDevice().getLogicalDevice().getBufferMemoryRequirements(vertexBuffer, &memRequirements);

		auto allocInfo = vk::MemoryAllocateInfo(memRequirements.size, findMemoryType(memRequirements.memoryTypeBits, 
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent));

		if (graphics::get()->getLogicalDevice().getLogicalDevice().allocateMemory(&allocInfo, nullptr, &vertexBufferMemory) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to allocate vertex buffer memory");
		}

		graphics::get()->getLogicalDevice().getLogicalDevice().bindBufferMemory(vertexBuffer, vertexBufferMemory, 0);

		void* data;
		graphics::get()->getLogicalDevice().getLogicalDevice().mapMemory(vertexBufferMemory, 0, bufferInfo.size, {}, &data);
			memcpy(data, vertices.data(), (size_t)bufferInfo.size);
		graphics::get()->getLogicalDevice().getLogicalDevice().unmapMemory(vertexBufferMemory);
	}

	uint32_t VertexBuffer::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
	{
		vk::PhysicalDeviceMemoryProperties memProperties;
		graphics::get()->getPhysicalDevice().getPhysicalDevice().getMemoryProperties(&memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("Failed to find memory type!");
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