#ifndef DEPTH_RESOURCES_CLASS
#define DEPTH_RESOURCES_CLASS

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vulkan/vulkan.hpp>

namespace gibvk::vulkan::drawing::resources {
	class DepthResources {
	public:
		DepthResources();

		

		[[nodiscard]] const vk::Image& getDepthImage() const;
		[[nodiscard]] const vk::DeviceMemory& getDepthImageMemory() const;
		[[nodiscard]] const vk::ImageView& getDepthImageView() const;

	private:
		vk::Image depthImage;
		vk::DeviceMemory depthImageMemory;
		vk::ImageView depthImageView;
	};

	vk::Format findDepthFormat();

	std::unique_ptr<DepthResources> createDepthResources();
}
#endif // !DEPTH_RESOURCES_CLASS
