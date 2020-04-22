#pragma once

#include <variant>
#include "Errors.h"
#include "Board.h"

class Game {
private:
    Board game_board_;
    std::vector<Player> players_;
    bool is_game_in_check_;
    bool is_game_ended_;
    Player *current_player_;

public:
    Game();

    Player *get_current_player();

    void add_player(int player_id, bool is_dark, bool is_top);

    void initialize_game();

    Player &get_player_by_number(size_t id);

    void make_move(const std::string &from, const std::string &to);

    void render();
};
