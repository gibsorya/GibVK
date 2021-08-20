#ifndef VULKAN_INSTANCE_CLASS
#define VULKAN_INSTANCE_CLASS

#include <vulkan/vulkan.hpp>
#include <iostream>

class VulkanInstance {
public:
	void createInstance();

	vk::Instance getInstance();

private:
	vk::Instance instance;
};

#endif