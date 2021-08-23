#ifndef GRAPHICS_CLASS
#define GRAPHICS_CLASS

#include "Vulkan/Window.hpp"
#include "Vulkan/Instance.hpp"
#include "Vulkan/ValidationLayers/DebugUtils.hpp"
#include "Vulkan/ValidationLayers/ValidLayers.hpp"
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
		[[nodiscard]] const vulkan::debugutils::DebugUtils& getDebugMessenger() const;

	private:
		static std::unique_ptr<Graphics> graphics;

		std::unique_ptr<vulkan::VulkanWindow> window{};
		std::unique_ptr<vulkan::VulkanInstance> instance{};
		std::unique_ptr<vulkan::debugutils::DebugUtils> debugMessenger{};
	};

	Graphics *get();

}

#endif