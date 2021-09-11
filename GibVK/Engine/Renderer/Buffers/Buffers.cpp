#include "Buffers.hpp"
#include "../../Graphics.hpp"

namespace gibvk::renderer::buffers {
	std::unique_ptr<Buffers> Buffers::buffer = nullptr;

	Buffers* Buffers::_get()
	{
		if (buffer == nullptr) {
			buffer = std::make_unique<Buffers>();
		}
		return buffer.get();
	}

	void Buffers::initialize()
	{
		vertexBuffer = vertexbuffers::createVertexBuffer();
		indexBuffer = indexbuffers::createIndexBuffer();
	}

	uint32_t Buffers::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
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

	void Buffers::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& bufferMemory)
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

	void Buffers::copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size)
	{
		auto allocInfo = vk::CommandBufferAllocateInfo(vulkan::drawing::get()->getCommandPool().getCommandPool(), vk::CommandBufferLevel::ePrimary, 1);

		vk::CommandBuffer commandBuffer;
		graphics::get()->getLogicalDevice().getLogicalDevice().allocateCommandBuffers(&allocInfo, &commandBuffer);

		auto beginInfo = vk::CommandBufferBeginInfo({ vk::CommandBufferUsageFlagBits::eOneTimeSubmit });

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

	const vertexbuffers::VertexBuffer& Buffers::getVertexBuffer() const
	{
		if (vertexBuffer == nullptr) {
			throw std::runtime_error("Vertex buffer has not been initialized!");
		}

		return *vertexBuffer;
	}

	const indexbuffers::IndexBuffer& Buffers::getIndexBuffer() const
	{
		if (indexBuffer == nullptr) {
			throw std::runtime_error("Index buffer has not been initialized!");
		}

		return *indexBuffer;
	}

	Buffers* get()
	{
		return Buffers::_get();
	}
}