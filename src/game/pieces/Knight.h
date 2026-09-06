#pragma once

#include "Piece.h"
#include <array>

class Knight : public Piece
{
public:
    explicit Knight(int player_id);

    [[nodiscard]] Piece *clone() const override;

    std::vector<std::string>
    get_possible_moves(bool is_top, const std::string &from) override;
};
