#include "game.h"
#include <iostream>

int main() {
    std::cout << "Starting Application..." << std::endl;

    try {
        Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}