#ifndef BUFFERS_CLASS
#define BUFFERS_CLASS
#include <vulkan/vulkan.hpp>

#include "VertexBuffers/VertexBuffer.hpp"
#include "IndexBuffers/IndexBuffer.hpp"

namespace gibvk::renderer::buffers {
	class Buffers {
	public:
		Buffers() = default;

		static Buffers* _get();

		void initialize();
		uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
		void createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& bufferMemory);
		void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size);

		[[nodiscard]] const vertexbuffers::VertexBuffer& getVertexBuffer() const;
		[[nodiscard]] const indexbuffers::IndexBuffer& getIndexBuffer() const;

	private:
		static std::unique_ptr<Buffers> buffer;

		std::unique_ptr<vertexbuffers::VertexBuffer> vertexBuffer{};
		std::unique_ptr<indexbuffers::IndexBuffer> indexBuffer{};
	};

	Buffers* get();
}

#endif // !BUFFERS_CLASS
