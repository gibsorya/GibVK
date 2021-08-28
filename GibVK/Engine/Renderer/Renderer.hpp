#ifndef RENDERER_CLASS
#define RENDERER_CLASS

#include <memory>
#include <vulkan/vulkan.hpp>

#include "Buffers/Buffers.hpp"

namespace gibvk::renderer {
	class Renderer {
	public:
		Renderer() = default;

		static Renderer* _get();

		void initialize();

	private:
		static std::unique_ptr<Renderer> renderer;
	};

	Renderer* get();
}

#endif // !RENDERER_CLASS
