#ifndef BUFFERS_CLASS
#define BUFFERS_CLASS
#include <vulkan/vulkan.hpp>

#include "VertexBuffers/VertexBuffer.hpp"

namespace gibvk::renderer::buffers {
	class Buffers {
	public:
		Buffers() = default;

		static Buffers* _get();

		void initialize();

		[[nodiscard]] const vertexbuffers::VertexBuffer& getVertexBuffer() const;

	private:
		static std::unique_ptr<Buffers> buffer;

		std::unique_ptr<vertexbuffers::VertexBuffer> vertexBuffer{};
	};

	Buffers* get();
}

#endif // !BUFFERS_CLASS
