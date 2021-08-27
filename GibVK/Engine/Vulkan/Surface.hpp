#ifndef SURFACE_CLASS
#define SURFACE_CLASS

#include <vulkan/vulkan.hpp>

namespace gibvk::vulkan {
	class Surface {
	public:
		Surface();

		[[nodiscard]] const vk::SurfaceKHR& getSurface() const;
	private:
		vk::SurfaceKHR surface;
	};

	std::unique_ptr<Surface> createSurface();
}

#endif // !SURFACE_CLASS
