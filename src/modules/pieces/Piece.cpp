#include "Piece.h"

Piece::Piece() : graphic_{" "} {}
Piece::Piece(std::string graphic) : graphic_{graphic} {}

std::string Piece::get_graphic()
{
    return graphic_;
}
