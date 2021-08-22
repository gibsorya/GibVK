#ifndef VULKAN_INSTANCE_CLASS
#define VULKAN_INSTANCE_CLASS

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <vector>
#include "../Validation Layers/ValidationLayers.h"

class VulkanInstance {
public:
	void createInstance();

	//vk::Instance getInstance();

	std::vector<const char*> getRequiredExtensions();

	vk::Instance instance;

private:
	
};

#endif