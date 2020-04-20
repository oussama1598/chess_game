#include <iostream>
#include "modules/Game.h"

int main()
{
    Game game;

    game.make_move("A1", "B5");
    game.make_move("A2", "C5");

    game.render();

    return 0;
}
