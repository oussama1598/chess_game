#pragma once

#include <iostream>
#include <array>
#include <map>
#include <boost/lexical_cast.hpp>
#include <string>
#include "pieces/Piece.h"
#include "pieces/King.h"
#include "pieces/Queen.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Rook.h"
#include "pieces/Pawn.h"

class Board
{
public:
    static const signed int cols{8};
    static const signed int rows{8};

    const std::map<char, size_t> cols_map_{{'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7}};
    std::array<std::array<Piece, cols>, 8> pieces_;

    Board();
    ~Board();

    void add_piece(std::string place, Piece piece);
    void build_board();
    void render();
};
