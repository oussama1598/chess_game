#pragma once

#include <variant>
#include "Errors.h"
#include "Board.h"

class Game {
private:
    Board game_board_;
    std::vector<Player> players_;

public:
    Game();

    void add_player(int player_id, bool is_dark, bool is_top);

    void initialize_game();

    Player &get_player_by_number(size_t id);

    void make_move(const std::string &from, const std::string &to);

    void render();
};
