#include "VertexBuffer.hpp"
#include "../../../Graphics.hpp"

namespace gibvk::renderer::buffers::vertexbuffers {
	struct Vertex vertex;

	VertexBuffer::VertexBuffer()
	{
		vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

		vk::Buffer stagingBuffer;
		vk::DeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);

		void* data;
		graphics::get()->getLogicalDevice().getLogicalDevice().mapMemory(stagingBufferMemory, 0, bufferSize, {}, &data);
			memcpy(data, vertices.data(), (size_t)bufferSize);
		graphics::get()->getLogicalDevice().getLogicalDevice().unmapMemory(stagingBufferMemory);
		createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, vertexBuffer, vertexBufferMemory);

		copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
		graphics::get()->getLogicalDevice().getLogicalDevice().destroyBuffer(stagingBuffer);
		graphics::get()->getLogicalDevice().getLogicalDevice().freeMemory(stagingBufferMemory, nullptr);
	}

	void VertexBuffer::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& bufferMemory)
	{
		auto bufferInfo = vk::BufferCreateInfo({}, size, usage, vk::SharingMode::eExclusive);

		if (graphics::get()->getLogicalDevice().getLogicalDevice().createBuffer(&bufferInfo, nullptr, &buffer) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create vertex buffer!");
		}

		vk::MemoryRequirements memRequirements;
		graphics::get()->getLogicalDevice().getLogicalDevice().getBufferMemoryRequirements(buffer, &memRequirements);

		auto allocInfo = vk::MemoryAllocateInfo(memRequirements.size, findMemoryType(memRequirements.memoryTypeBits, properties));

		if (graphics::get()->getLogicalDevice().getLogicalDevice().allocateMemory(&allocInfo, nullptr, &bufferMemory) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to allocate vertex buffer memory");
		}

		graphics::get()->getLogicalDevice().getLogicalDevice().bindBufferMemory(buffer, bufferMemory, 0);
	}

	void VertexBuffer::copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size)
	{
		auto allocInfo = vk::CommandBufferAllocateInfo(vulkan::drawing::get()->getCommandPool().getCommandPool(), vk::CommandBufferLevel::ePrimary, 1);

		vk::CommandBuffer commandBuffer;
		graphics::get()->getLogicalDevice().getLogicalDevice().allocateCommandBuffers(&allocInfo, &commandBuffer);

		auto beginInfo = vk::CommandBufferBeginInfo({vk::CommandBufferUsageFlagBits::eOneTimeSubmit});

		commandBuffer.begin(&beginInfo);

		auto copyRegion = vk::BufferCopy(0, 0, size);
		commandBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copyRegion);
		commandBuffer.end();

		vk::SubmitInfo submitInfo{};
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;


		graphics::get()->getGraphicsQueue().getGraphicsQueue().submit(1, &submitInfo, VK_NULL_HANDLE);
		graphics::get()->getGraphicsQueue().getGraphicsQueue().waitIdle();

		graphics::get()->getLogicalDevice().getLogicalDevice().freeCommandBuffers(vulkan::drawing::get()->getCommandPool().getCommandPool(), 1, &commandBuffer);
		

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