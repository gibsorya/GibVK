#ifndef DESCRIPTOR_SET_CLASS
#define DESCRIPTOR_SET_CLASS

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

namespace gibvk::renderer::buffers::descriptors {

	class DescriptorSetLayout {
	public:
		DescriptorSetLayout();

		[[nodiscard]] const vk::DescriptorSetLayout& getDescriptorSetLayout() const;

	private:
		vk::DescriptorSetLayout descriptorSetLayout;
	};

	std::unique_ptr<DescriptorSetLayout> createDescriptorSetLayout();

	class DescriptorSets {
	public:
		DescriptorSets();

		[[nodiscard]] const std::vector<vk::DescriptorSet>& getDescriptorSets() const;
	private:
		std::vector<vk::DescriptorSet> descriptorSets;
	};

	std::unique_ptr<DescriptorSets> createDescriptorSets();
}

#endif // !DESCRIPTOR_SET_CLASS
