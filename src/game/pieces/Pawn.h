#pragma once

#include "Piece.h"

class Pawn : public Piece {
public:
    explicit Pawn(int player_id);

    [[nodiscard]] Piece *clone() const override;

    std::vector<std::string>
    get_possible_moves(bool is_top, const std::string &from) override;
};
