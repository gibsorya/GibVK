#include "Application.hpp"
#include "Engine.hpp"

namespace gibvk {
	Application::Application()
	{
		//Here to maybe do something in the future
	}

	void Application::run()
	{
		gibvk::vulkanengine::VulkanEngine::run(*this);
	}
}
