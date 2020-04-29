#pragma once

#include <random>
#include "game/Game.h"

class AI_Player {
private:
    static std::default_random_engine generator_;
    static int get_random_index_(int size);

public:

    static void make_a_move(Game *game, int level);

    static void make_a_move_level1(Game *game);
};
