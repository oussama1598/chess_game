#include <iostream>

#include "modules/Game.h"

int main() {
    Game game;

//    try {
//        game.make_move("A2", "B2");
//    } catch (std::exception &error) {
//        std::cout << error.what() << std::endl;
//    }

    game.render();

    return 0;
}
