#ifndef INDEX_BUFFER_CLASS
#define INDEX_BUFFER_CLASS

#include <vulkan/vulkan.hpp>
#include <vector>
#include <glm/glm.hpp>

namespace gibvk::renderer::buffers::indexbuffers {
	const std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4
	};

	class IndexBuffer {
	public:
		IndexBuffer();

		[[nodiscard]] const vk::Buffer& getIndexBuffer() const;
		[[nodiscard]] const vk::DeviceMemory& getIndexBufferMemory() const;

	private:
		vk::Buffer indexBuffer;
		vk::DeviceMemory indexBufferMemory;
	};

	std::unique_ptr<IndexBuffer> createIndexBuffer();

}


#endif // !INDEX_BUFFER_CLASS
