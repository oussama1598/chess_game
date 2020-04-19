#include "Board.h"

Board::Board()
{
    build_board();
}
Board::~Board() {}

void Board::build_board()
{
    for (size_t y = 0; y < pieces_.size(); ++y)
        for (size_t x = 0; x < pieces_[y].size(); ++x)
            pieces_[y][x] = Piece();

    // add pieces manually this must be changed after
    pieces_[0][0] = King();
    pieces_[0][1] = Queen();
    pieces_[0][2] = Rook();
    pieces_[0][3] = Bishop();
    pieces_[0][4] = Pawn();
    pieces_[0][5] = Knight();
}

void Board::render()
{
    std::string line_seperator = "  +--+--+--+--+--+--+--+--+";

    std::cout << line_seperator << std::endl;

    for (size_t y = 0; y < pieces_.size(); ++y)
    {
        std::cout << pieces_.size() - y << " ";

        for (size_t x = 0; x < pieces_[y].size(); ++x)
            std::cout << "|" << pieces_[y][x].get_graphic() << " ";

        std::cout << "|" << std::endl
                  << line_seperator << std::endl;
    }

    std::cout << "    A  B  C  D  E  F  G  H" << std::endl;
}
