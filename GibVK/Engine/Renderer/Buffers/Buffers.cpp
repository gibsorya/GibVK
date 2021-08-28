#include "Buffers.hpp"
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
	}
	const vertexbuffers::VertexBuffer& Buffers::getVertexBuffer() const
	{
		if (vertexBuffer == nullptr) {
			throw std::runtime_error("Vertex buffer has not been initialized!");
		}

		return *vertexBuffer;
	}
	Buffers* get()
	{
		return Buffers::_get();
	}
}