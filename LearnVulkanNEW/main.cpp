#include <iostream>
#include "Engine/Application.hpp"

class GibVKApp : public gibvk::Application {
    //Here to maybe do something in the future
};

int main() {

    auto application = std::make_unique<GibVKApp>();

    try {
        application->run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}