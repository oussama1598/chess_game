#pragma once

#include "Piece.h"

class Queen : public Piece
{
public:
    explicit Queen(int player_id);

    std::vector<std::string>
    get_possible_moves(bool is_top, const std::string &from) override;
};
