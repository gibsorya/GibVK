#include "Surface.hpp"
#include "../Graphics.hpp"

namespace gibvk::vulkan {

	Surface::Surface()
	{
		if (glfwCreateWindowSurface(graphics::get()->getInstance().getInstance(), graphics::get()->getWindow().getWindow(), nullptr, reinterpret_cast<VkSurfaceKHR*>(&surface)) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create window surface!");
		}
	}

	const vk::SurfaceKHR& Surface::getSurface() const
	{
		return surface;
	}

	std::unique_ptr<Surface> createSurface()
	{
		return std::make_unique<Surface>();
	}
}
