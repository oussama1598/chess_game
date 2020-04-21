#pragma once

#include <iostream>
#include "Piece.h"

class Pawn : public Piece {
public:
    explicit Pawn(int player_id);

    std::vector<std::string>
    get_possible_moves(bool is_top, const std::string &from) override;
};
