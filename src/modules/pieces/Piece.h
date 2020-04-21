#pragma once

#include <iostream>
#include <map>
#include <boost/lexical_cast.hpp>
#include "../Player.h"

class Piece {
protected:
    std::map<std::string, std::string> graphics_;
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

    Piece(std::map<std::string, std::string>, int player_id);

    static piece_coordinates get_piece_coordinates_from_id(const std::string &id);

    [[nodiscard]] int get_player_id() const;

    bool is_first_move() const;

    virtual bool is_valid_move(Player &player, const std::string &from, const std::string &to);

    std::string get_graphic(std::string &type);
};
