#pragma once

class Piece
{
protected:
    char type_;

public:
    Piece();
    Piece(char type);

    char get_type();
};
