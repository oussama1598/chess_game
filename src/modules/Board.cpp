#include "Board.h"

Board::Board()
{
    build_board();
}
Board::~Board() {}

void Board::add_piece(const std::string place, Piece piece)
{
    size_t column = cols_map_.at(place[0]);
    size_t line = boost::lexical_cast<size_t>(place[1]) - 1;

    pieces_[line][column] = piece;
}

void Board::build_board()
{
    for (size_t y = 0; y < pieces_.size(); ++y)
        for (size_t x = 0; x < pieces_[y].size(); ++x)
            pieces_[y][x] = Piece();

    // add pieces manually this must be changed after
    add_piece("A1", Rook());
    add_piece("B1", Knight());
    add_piece("C1", Bishop());
    add_piece("D1", Queen());
    add_piece("E1", King());
    add_piece("F1", Bishop());
    add_piece("G1", Knight());
    add_piece("H1", Rook());

    for (auto &column : cols_map_)
    {
        std::stringstream s{""};

        s << column.first << "2";

        add_piece(s.str(), Pawn());
    }
}

void Board::render()
{
    std::string line_seperator = "  +--+--+--+--+--+--+--+--+";

    std::cout << line_seperator << std::endl;

    for (size_t y = 0; y < pieces_.size(); ++y)
    {
        signed int inverse_y = pieces_.size() - 1 - y;

        std::cout << inverse_y + 1 << " ";

        for (size_t x = 0; x < pieces_[inverse_y].size(); ++x)
            std::cout << "|" << pieces_[inverse_y][x].get_graphic() << " ";

        std::cout << "|" << std::endl
                  << line_seperator << std::endl;
    }

    std::cout << "    ";
    for (auto &column : cols_map_)
    {
        std::cout << column.first << "  ";
    }
    std::cout << std::endl;
}
