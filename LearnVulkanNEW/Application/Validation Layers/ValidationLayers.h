#ifndef VALIDATION_LAYERS_CLASS
#define VALIDATION_LAYERS_CLASS

#include <vector>
#include <vulkan/vulkan.hpp>
#include "DebugUtils/DebugUtils.h"

class ValidLayers {
public:
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	bool checkValidationLayerSupport();

#ifdef NDEBUG
	const static bool enableValidationLayers = false;
#else
	const static bool enableValidationLayers = true;
#endif

	

	
};

#endif