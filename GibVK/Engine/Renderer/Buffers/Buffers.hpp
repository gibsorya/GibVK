#ifndef BUFFERS_CLASS
#define BUFFERS_CLASS
#include <vulkan/vulkan.hpp>

#include "VertexBuffers/VertexBuffer.hpp"
#include "IndexBuffers/IndexBuffer.hpp"
#include "UniformBuffers/DescriptorSet.hpp"
#include "UniformBuffers/DescriptorPool.hpp"
#include "UniformBuffers/UniformBuffers.hpp"

namespace gibvk::renderer::buffers {
	class Buffers {
	public:
		Buffers() = default;

		static Buffers* _get();

		void initialize(bool recreatingSwapchain = false);
		void initDescriptor();
		uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
		void createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& bufferMemory);
		void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size);
		
		

		[[nodiscard]] const vertexbuffers::VertexBuffer& getVertexBuffer() const;
		[[nodiscard]] const indexbuffers::IndexBuffer& getIndexBuffer() const;
		[[nodiscard]] const descriptors::DescriptorSetLayout& getDescriptorSetLayout() const;
		[[nodiscard]] const descriptors::DescriptorSets& getDescriptorSets() const;
		[[nodiscard]] const descriptors::DescriptorPool& getDescriptorPool() const;
		[[nodiscard]] const uniformbuffers::UniformBuffer& getUniformBuffer() const;

	private:
		static std::unique_ptr<Buffers> buffer;

		std::unique_ptr<vertexbuffers::VertexBuffer> vertexBuffer{};
		std::unique_ptr<indexbuffers::IndexBuffer> indexBuffer{};
		std::unique_ptr<descriptors::DescriptorSetLayout> descriptorSetLayout{};
		std::unique_ptr<descriptors::DescriptorSets> descriptorSets{};
		std::unique_ptr<descriptors::DescriptorPool> descriptorPool{};
		std::unique_ptr<uniformbuffers::UniformBuffer> uniformBuffer{};
	};

	vk::CommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(vk::CommandBuffer commandBuffer);

	Buffers* get();
}

#endif // !BUFFERS_CLASS
