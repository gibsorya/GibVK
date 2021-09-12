#include "UniformBuffers.hpp"
#include "../../../Graphics.hpp"
#include "../Buffers.hpp"

namespace gibvk::renderer::buffers::uniformbuffers {
	UniformBuffer::UniformBuffer()
	{
		vk::DeviceSize bufferSize = sizeof(UniformBufferObject);

		uniformBuffers.resize(graphics::get()->getSwapchain().getSwapchainImages().size());
		uniformBuffersMemory.resize(graphics::get()->getSwapchain().getSwapchainImages().size());

		for (size_t i = 0; i < graphics::get()->getSwapchain().getSwapchainImages().size(); i++) {
			buffers::get()->createBuffer(bufferSize, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, uniformBuffers[i], uniformBuffersMemory[i]);
		}
	}

	const std::vector<vk::Buffer>& UniformBuffer::getUniformbuffers() const
	{
		return uniformBuffers;
	}

	const std::vector<vk::DeviceMemory>& UniformBuffer::getUniformbuffersMemory() const
	{
		return uniformBuffersMemory;
	}

	std::unique_ptr<UniformBuffer> createUniformBuffers()
	{
		return std::make_unique<UniformBuffer>();
	}

}
