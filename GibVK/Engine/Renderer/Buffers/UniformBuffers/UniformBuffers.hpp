#ifndef UNIFORM_BUFFERS_CLASS
#define UNIFORM_BUFFERS_CLASS
#define GLM_FORCE_RADIANS

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

namespace gibvk::renderer::buffers::uniformbuffers {
	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

	class UniformBuffer {
	public:
		UniformBuffer();
		
		[[nodiscard]] const std::vector<vk::Buffer>& getUniformbuffers() const;
		[[nodiscard]] const std::vector<vk::DeviceMemory>& getUniformbuffersMemory() const;

	private:
		std::vector<vk::Buffer> uniformBuffers;
		std::vector<vk::DeviceMemory> uniformBuffersMemory;
	};


	std::unique_ptr<UniformBuffer> createUniformBuffers();
}

#endif // !UNIFORM_BUFFERS_CLASS
