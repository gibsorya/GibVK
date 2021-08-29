#ifndef VERTEX_BUFFERS_CLASS
#define VERTEX_BUFFERS_CLASS

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

namespace gibvk::renderer::buffers::vertexbuffers {
	struct Vertex {
		glm::vec2 pos;
		glm::vec3 color;

		//Describes at which rate to load data from memory throughout the vertices
		static vk::VertexInputBindingDescription getBindingDescription() {
			auto bindingDescription = vk::VertexInputBindingDescription(0, sizeof(Vertex), vk::VertexInputRate::eVertex);

			return bindingDescription;
		}

		static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions() {
			auto attributeDescriptions = std::array<vk::VertexInputAttributeDescription, 2>{ {
				{0, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, pos)},
				{ 1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color) },
				} };

			return attributeDescriptions;
		}


	};

	const std::vector<Vertex> vertices = {
		{{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};

	class VertexBuffer {
	public:
		VertexBuffer();

		void createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& bufferMemory);
		void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size);

		[[nodiscard]] const vk::Buffer& getVertexBuffer() const;
		[[nodiscard]] const vk::DeviceMemory& getDeviceMemory() const;

		uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);

	private:
		vk::Buffer vertexBuffer;
		vk::DeviceMemory vertexBufferMemory;
	};

	std::unique_ptr<VertexBuffer> createVertexBuffer();
}

#endif // !VERTEX_BUFFERS_CLASS
