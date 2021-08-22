#include "Application.hpp"
#include "Engine.hpp"

namespace gibvk {
	Application::Application()
	{
	}

	void Application::run()
	{
		gibvk::vulkanengine::VulkanEngine::run(*this);
	}
}
