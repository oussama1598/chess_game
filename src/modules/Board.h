#pragma once

#include <iostream>
#include <array>
#include "pieces/Piece.h"
#include "pieces/King.h"
#include "pieces/Queen.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Rook.h"
#include "pieces/Pawn.h"

class Board
{
private:
    static const signed int cols{8};
    static const signed int rows{8};

public:
    std::array<std::array<Piece, cols>, 8> pieces_;

    Board();
    ~Board();

    void build_board();
    void render();
};
