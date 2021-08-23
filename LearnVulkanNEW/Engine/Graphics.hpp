#ifndef GRAPHICS_CLASS
#define GRAPHICS_CLASS

#include "Vulkan/Window/Window.hpp"
#include "Vulkan/Instance/Instance.hpp"
#include "Application.hpp"

#include <memory>
#include <vulkan/vulkan.hpp>
namespace gibvk::graphics {
	class Graphics {
	public:
		Graphics() = default;

		static Graphics* _get();

		void initialize();
		void render();
		void cleanup();

		[[nodiscard]] const vulkan::VulkanWindow &getWindow() const;
		[[nodiscard]] const vulkan::VulkanInstance &getInstance() const;

	private:
		static std::unique_ptr<Graphics> graphics;

		std::unique_ptr<vulkan::VulkanWindow> window{};
		std::unique_ptr<vulkan::VulkanInstance> instance{};
	};

	Graphics *get();

}

#endif