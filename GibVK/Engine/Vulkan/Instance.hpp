#ifndef INSTANCE_CLASS
#define INSTANCE_CLASS

#include <vulkan/vulkan.hpp>
#include "ValidationLayers/ValidLayers.hpp"
#include "ValidationLayers/DebugUtils.hpp"

namespace gibvk::vulkan {
	class VulkanInstance {
	public:
		VulkanInstance();

		std::vector<const char*> getRequiredExtensions();

		[[nodiscard]] const vk::Instance& getInstance() const;
	private:
		vk::Instance instance;
	};

	std::unique_ptr<VulkanInstance> createInstance();
}

#endif