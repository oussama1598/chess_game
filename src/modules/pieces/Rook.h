#pragma once

#include <iostream>
#include "Piece.h"

class Rook : public Piece
{
public:
    explicit Rook(int player_id);

    std::vector<std::string>
    get_possible_moves(bool is_top, const std::string &from);
};
