#pragma once

#include <iostream>
#include <array>
#include <map>
#include <string>
#include "pieces/Piece.h"
#include "pieces/King.h"
#include "pieces/Queen.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Rook.h"
#include "pieces/Pawn.h"

class Board {
public:
    static const signed int cols{8};
    static const signed int rows{8};

    std::array<std::array<Piece *, cols>, 8> pieces_{};

    Board();

    Piece::piece_coordinates get_piece_coordinates_from_id(const std::string &id);

    Piece* get_piece_at(int line, int column);

    void add_piece(const std::string &place, Piece *piece);

    void build_board();

    void render(std::vector<Player> &players);
};
