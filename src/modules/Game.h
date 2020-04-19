#pragma once

#include "Board.h"
#include "Player.h"

class Game
{
private:
    Board game_board_;
    std::vector<Player> players_;

public:
    Game();
    ~Game();

    void add_player(const int player_number, const bool is_dark, const bool is_top);
    void initialize_game();
    void render();
};
