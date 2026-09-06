#pragma once

#include "Piece.h"

class Bishop : public Piece
{
public:
    explicit Bishop(int player_id);

    [[nodiscard]] Piece *clone() const override;

    std::vector<std::string>
    get_possible_moves(bool is_top, const std::string &from) override;
};
