#pragma once

#include <iostream>
#include "Piece.h"

class Pawn : public Piece {
public:
    explicit Pawn(int player_id);

    bool is_valid_move(Player &player, piece_coordinates source, piece_coordinates destination) override;
};
