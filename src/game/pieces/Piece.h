#pragma once

#include <iostream>
#include <map>
#include <boost/lexical_cast.hpp>
#include <vector>
#include "../Player.h"

class Piece {
protected:
    char symbol_;
    int player_id_;
    bool is_first_move_;

public:
    struct piece_coordinates {
        int line;
        int column;
    };

    static const signed int cols{8};
    static const signed int rows{8};

    static const std::map<char, size_t> cols_map_;

    Piece();

    Piece(char symbol, int player_id);

    virtual ~Piece();

    void did_move();

    inline char get_symbol() { return symbol_; }

    static piece_coordinates
    get_piece_coordinates_from_id(const std::string &id);

    static std::string get_id_from_coordinates(piece_coordinates coordinates);

    [[nodiscard]] int get_player_id() const;

    [[nodiscard]] bool is_first_move() const;

    void generate_horizontal_movements(std::vector<std::string> &possible_moves,
                                       piece_coordinates &source);

    void generate_vertical_movements(std::vector<std::string> &possible_moves,
                                     piece_coordinates &source);

    void generate_diagonal_movements(std::vector<std::string> &possible_moves,
                                     piece_coordinates &source);

    virtual std::vector<std::string>
    get_possible_moves(bool is_top, const std::string &from);

    virtual bool is_valid_move(Player &player, const std::string &from,
                               const std::string &to);
};
