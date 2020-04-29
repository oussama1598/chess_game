#pragma once

#include <random>
#include "game/Game.h"

class AI_Player {
private:
    static std::default_random_engine generator_;

    static int get_random_index_(int size);

public:

    static void make_a_move(Game *game, int level);

    static void random_move(Game *game);

    static void high_value_move(Game *game);

    static int evaluate_board(int player_id, Game &game);

    static int get_piece_value(int player_id, Piece *piece);
};
