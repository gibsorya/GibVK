#ifndef VALID_LAYERS_CLASS
#define VALID_LAYERS_CLASS

#include <vulkan/vulkan.hpp>
#include <vector>
#include <iostream>

namespace gibvk::vulkan {
	namespace validlayers {
		const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
		};

		class ValidLayers {
		public:
			static bool checkValidationLayerSupport();
		};
	}
#ifdef NDEBUG
	const static bool enableValidationLayers = false;
#else
	const static bool enableValidationLayers = true;
#endif

}

#endif
