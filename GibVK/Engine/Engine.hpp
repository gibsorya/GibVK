#ifndef ENGINE_CLASS
#define ENGINE_CLASS

#include "Application.hpp"

#include "Graphics.hpp"

namespace gibvk::vulkanengine {
	class VulkanEngine {
	public:
		static void run(const Application &application);
		static void initVulkan();
		static void mainLoop();
		static void cleanup();
	};
}

#endif