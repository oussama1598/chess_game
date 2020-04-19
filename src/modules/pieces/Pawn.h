#pragma once

#include <iostream>
#include "Piece.h"

class Pawn : public Piece
{
public:
    Pawn(bool is_dark = false);
};
