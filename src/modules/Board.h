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

    void generate_horizontal_movements(std::vector<std::string> &possible_moves,
                                       Piece::piece_coordinates &source);

    void generate_vertical_movements(std::vector<std::string> &possible_moves,
                                     Piece::piece_coordinates &source);

    void generate_diagonal_movements(std::vector<std::string> &possible_moves,
                                     Piece::piece_coordinates &source);

    Piece *get_piece_at(int line, int column);

    void add_piece(const std::string &place, Piece *piece);

    void build_board();

    std::vector<std::string> get_all_empty_spots(const std::string &from);

    bool is_valid_move(const std::string &from, const std::string &to);

    void render(std::vector<Player> &players);
};
