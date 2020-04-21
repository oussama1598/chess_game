#include "Board.h"

Board::Board() {
    build_board();
}

Piece::piece_coordinates Board::get_piece_coordinates_from_id(const std::string &id) {
    int line = (int) boost::lexical_cast<size_t>(id[1]) - 1;

    // this checks if the id is valid
    if (cols_map_.find(id[0]) == cols_map_.end() || line > (cols - 1))
        return {-1, -1};

    int column = cols_map_.at(id[0]);

    return {line, column};
}

Piece *Board::get_piece_at(const int line, const int column) {
    return pieces_.at(line).at(column);
}

void Board::add_piece(const std::string &id, Piece *piece) {
    Piece::piece_coordinates piece_coordinates = get_piece_coordinates_from_id(id);

    if (piece_coordinates.line == -1 || piece_coordinates.column == -1) {
        std::cout << "Invalid place for a piece" << std::endl;
        return;
    }

    pieces_[piece_coordinates.line][piece_coordinates.column] = piece;
}

void Board::build_board() {
    for (auto &piece : pieces_)
        for (auto &x : piece)
            x = new Piece();
}

void Board::render(std::vector<Player> &players) {
    std::string lineSeparator = "   +---+---+---+---+---+---+---+---+";

    std::cout << lineSeparator << std::endl;

    for (size_t y = 0; y < pieces_.size(); ++y) {
        size_t inverse_y = pieces_.size() - 1 - y;

        std::cout << " " << inverse_y + 1 << " ";

        for (auto &x : pieces_[inverse_y]) {
            int player_id = x->get_player_id();
            std::string graphics_type = "dark";

            if (player_id != -1)
                graphics_type = players.at(x->get_player_id()).is_dark ? "dark" : "light";

            std::cout << "| " << x->get_graphic(graphics_type) << " ";
        }

        std::cout << "|" << std::endl
                  << lineSeparator << std::endl;
    }

    std::cout << "     ";
    for (auto &column : cols_map_) {
        std::cout << column.first << "   ";
    }
    std::cout << std::endl;
}
