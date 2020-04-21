#pragma once

#include "Piece.h"

class King : public Piece
{
public:
    explicit King(int player_id);

    std::vector<std::string>
    get_possible_moves(bool is_top, const std::string &from) override;
};
