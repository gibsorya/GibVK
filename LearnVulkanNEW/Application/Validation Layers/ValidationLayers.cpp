#include "ValidationLayers.h"

bool ValidLayers::checkValidationLayerSupport()
{
	uint32_t layerCount;
	vk::enumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<vk::LayerProperties> availableLayers(layerCount);
	vk::enumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : ValidLayers::validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}


	}

	return true;
}