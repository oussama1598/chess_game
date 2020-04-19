#pragma once
#include <iostream>
#include <array>

class Piece
{
protected:
    std::array<std::string, 2> graphics_;
    bool is_dark_;

public:
    Piece();
    Piece(std::array<std::string, 2> graphics, bool is_dark);

    std::string get_graphic();
};
