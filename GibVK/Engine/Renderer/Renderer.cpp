#include "Renderer.hpp"
#include "Buffers/Buffers.hpp"

namespace gibvk::renderer {
	std::unique_ptr<Renderer> Renderer::renderer = nullptr;

	Renderer* Renderer::_get()
	{
		if (renderer == nullptr) {
			renderer = std::make_unique<Renderer>();

			return renderer.get();
		}

		return renderer.get();
	}

	void Renderer::initialize()
	{
		buffers::get()->initialize();
	}

	void Renderer::recreateSwapchain()
	{
		buffers::get()->recreateSwapchain();
	}

	Renderer* get()
	{
		return Renderer::_get();
	}
}