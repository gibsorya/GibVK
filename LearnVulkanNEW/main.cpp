#include <iostream>
//#include "engine.h"
#include "Engine/Application.hpp"

class GibVKApp : public gibvk::Application {

};

int main() {
    //VulkanEngine engine;

    auto application = std::make_unique<GibVKApp>();

    try {
        //engine.run();
        application->run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}