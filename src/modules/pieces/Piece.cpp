#include "Piece.h"

Piece::Piece() : type_{'e'} {}
Piece::Piece(char type) : type_{type} {}

char Piece::get_type()
{
    return type_;
}
