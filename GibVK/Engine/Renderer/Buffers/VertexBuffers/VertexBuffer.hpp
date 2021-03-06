#ifndef VERTEX_BUFFERS_CLASS
#define VERTEX_BUFFERS_CLASS
#define GLM_ENABLE_EXPERIMENTAL
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	//Describes at which rate to load data from memory throughout the vertices
	static vk::VertexInputBindingDescription getBindingDescription() {
		auto bindingDescription = vk::VertexInputBindingDescription(0, sizeof(Vertex), vk::VertexInputRate::eVertex);

		return bindingDescription;
	}

	static std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions() {
		auto attributeDescriptions = std::array<vk::VertexInputAttributeDescription, 3>{ {
			{ 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, pos)},
			{ 1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color) },
			{ 2, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, texCoord) },
			} };

		return attributeDescriptions;
	}

	bool operator==(const Vertex& other) const {
		return pos == other.pos && color == other.color && texCoord == other.texCoord;
	}

};

namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}

namespace gibvk::renderer::buffers::vertexbuffers {
	
	//struct Vertex {
	//	glm::vec3 pos;
	//	glm::vec3 color;
	//	glm::vec2 texCoord;

	//	//Describes at which rate to load data from memory throughout the vertices
	//	static vk::VertexInputBindingDescription getBindingDescription() {
	//		auto bindingDescription = vk::VertexInputBindingDescription(0, sizeof(Vertex), vk::VertexInputRate::eVertex);

	//		return bindingDescription;
	//	}

	//	static std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions() {
	//		auto attributeDescriptions = std::array<vk::VertexInputAttributeDescription, 3>{ {
	//			{0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, pos)},
	//			{ 1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color) },
	//			{ 2, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, texCoord) },
	//			} };

	//		return attributeDescriptions;
	//	}

	//	bool operator==(const Vertex& other) const {
	//		return pos == other.pos && color == other.color && texCoord == other.texCoord;
	//	}

	//};

	
	extern std::vector<Vertex> vertices;
	

	/*const std::vector<Vertex> vertices = {
		{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

		{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
		{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
	};*/

	


	class VertexBuffer {
	public:
		VertexBuffer();

		//[[nodiscard]] const std::vector<Vertex>& getVertices() const;

		[[nodiscard]] const vk::Buffer& getVertexBuffer() const;
		[[nodiscard]] const vk::DeviceMemory& getDeviceMemory() const;

	private:
		/*std::vector<Vertex> vertices;*/

		vk::Buffer vertexBuffer;
		vk::DeviceMemory vertexBufferMemory;
	};

	std::unique_ptr<VertexBuffer> createVertexBuffer();
}

#endif // !VERTEX_BUFFERS_CLASS
