#include "Piece.h"

Piece::Piece() : graphics_{{" ", " "}}, is_dark_{false} {}
Piece::Piece(std::array<std::string, 2> graphics, bool is_dark) : graphics_{graphics}, is_dark_{is_dark} {}

std::string Piece::get_graphic()
{
    if (is_dark_)
        return graphics_[1];

    return graphics_[0];
}
