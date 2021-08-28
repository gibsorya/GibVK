#include "VertexBuffer.hpp"


namespace gibvk::renderer::buffers::vertexbuffers {
	struct Vertex vertex;

	VertexBuffer::VertexBuffer()
	{
	}

	const vk::Buffer& VertexBuffer::getVertexBuffer() const
	{
		return vertexBuffer;
	}

	std::unique_ptr<VertexBuffer> createVertexBuffer()
	{
		return std::make_unique<VertexBuffer>();
	}
}