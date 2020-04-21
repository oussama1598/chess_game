#pragma once

#include <iostream>
#include <map>
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

    Piece();

    Piece(std::map<std::string, std::string>, int player_id);

    [[nodiscard]] int get_player_id() const;

    bool is_first_move() const;

    virtual bool is_valid_move(Player &player, piece_coordinates source, piece_coordinates destination);

    std::string get_graphic(std::string &type);
};
