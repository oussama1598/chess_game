#pragma once
#include <iostream>

class Piece
{
protected:
    std::string graphic_;

public:
    Piece();
    Piece(std::string graphic_);

    std::string get_graphic();
};
