#include <iostream>

#include "modules/Game.h"
#include "Errors.h"

int main() {
    Game game;

    try {
        game.make_move("A2", "A4");
    } catch (std::exception &error) {
        std::cout << error.what() << std::endl;
    }

    game.render();

    return 0;
}
