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

    struct piece_coordinates
    {
        int line;
        int column;
    };

    const std::map<char, size_t> cols_map_{{'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7}};
    std::array<std::array<Piece, cols>, 8> pieces_;

    Board();

    ~Board() = default;

    piece_coordinates get_piece_coordinates_from_id(const std::string &id);
    Piece get_piece_at(const int line, const int column);
    void add_piece(const std::string& place, Piece piece);
    void build_board();
    void render();
};
