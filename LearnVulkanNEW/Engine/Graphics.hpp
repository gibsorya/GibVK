#ifndef GRAPHICS_CLASS
#define GRAPHICS_CLASS

#include "Vulkan/Window.hpp"
#include "Vulkan/Instance.hpp"
#include "Vulkan/Surface.hpp"
#include "Vulkan/ValidationLayers/DebugUtils.hpp"
#include "Vulkan/ValidationLayers/ValidLayers.hpp"
#include "Vulkan/Devices/PhysicalDevices.hpp"
#include "Vulkan/Devices/QueueFamilies.hpp"
#include "Vulkan/Devices/LogicalDevices.hpp"
#include "Vulkan/Queues/GraphicsQueue.hpp"
#include "Vulkan/Queues/PresentQueue.hpp"
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
		[[nodiscard]] const vulkan::Surface& getSurface() const;
		[[nodiscard]] const vulkan::debugutils::DebugUtils& getDebugMessenger() const;
		[[nodiscard]] const vulkan::devices::PhysicalDevices& getPhysicalDevice() const;
		[[nodiscard]] const vulkan::devices::LogicalDevices& getLogicalDevice() const;
		[[nodiscard]] const vulkan::queues::GraphicsQueue& getGraphicsQueue() const;
		[[nodiscard]] const vulkan::queues::PresentQueue& getPresentQueue() const;

	private:
		static std::unique_ptr<Graphics> graphics;

		std::unique_ptr<vulkan::VulkanWindow> window{};
		std::unique_ptr<vulkan::VulkanInstance> instance{};
		std::unique_ptr<vulkan::Surface> surface{};
		std::unique_ptr<vulkan::debugutils::DebugUtils> debugMessenger{};
		std::unique_ptr<vulkan::devices::PhysicalDevices> physicalDevice{};
		std::unique_ptr<vulkan::devices::LogicalDevices> logicalDevice{};
		std::unique_ptr<vulkan::queues::GraphicsQueue> graphicsQueue{};
		std::unique_ptr<vulkan::queues::PresentQueue> presentQueue{};
	};

	Graphics *get();

}

#endif