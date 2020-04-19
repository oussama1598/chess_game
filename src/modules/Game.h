#pragma once

#include "Board.h"

class Game
{
private:
    Board game_board_;

public:
    Game();
    ~Game();

    void render();
};
