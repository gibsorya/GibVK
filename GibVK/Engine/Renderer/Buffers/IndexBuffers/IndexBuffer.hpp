#ifndef INDEX_BUFFER_CLASS
#define INDEX_BUFFER_CLASS

#include <vulkan/vulkan.hpp>
#include <vector>
#include <glm/glm.hpp>

namespace gibvk::renderer::buffers::indexbuffers {
	//const std::vector<uint32_t> indices = {
	//	0, 1, 2, 2, 3, 0,
	//	4, 5, 6, 6, 7, 4
	//};
	/*struct Indices {
		static std::vector<uint32_t> indices;
	};*/

	extern std::vector<uint32_t> indices;

	class IndexBuffer {
	public:
		IndexBuffer();

		//[[nodiscard]] const std::vector<uint32_t>& getIndices() const;
		[[nodiscard]] const vk::Buffer& getIndexBuffer() const;
		[[nodiscard]] const vk::DeviceMemory& getIndexBufferMemory() const;

	private:
		//std::vector<uint32_t> indices;
		vk::Buffer indexBuffer;
		vk::DeviceMemory indexBufferMemory;
	};

	std::unique_ptr<IndexBuffer> createIndexBuffer();

}


#endif // !INDEX_BUFFER_CLASS
