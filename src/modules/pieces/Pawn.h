#pragma once

#include <iostream>
#include "Piece.h"

class Pawn : public Piece {
public:
    explicit Pawn(int player_id);

    bool is_valid_move(Player &player, const std::string &from, const std::string &to) override;
};
