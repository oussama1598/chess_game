#include "Board.h"

Board::Board() {
    build_board();
}

Piece *Board::get_piece_at(const int line, const int column) {
    return pieces_.at(line).at(column);
}

void Board::add_piece(const std::string &id, Piece *piece) {
    Piece::piece_coordinates piece_coordinates = Piece::get_piece_coordinates_from_id(
            id);

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

void Board::generate_horizontal_movements(
        std::vector<std::string> &possible_moves,
        Piece::piece_coordinates &source) {
    // Horizontal moves
    for (int i = 0; i < cols; ++i) {
        int line = source.line;
        int column = source.column + i;

        // avoid the the element it self
        if (line == source.line && column == source.column)
            continue;

        if (column >= cols)
            continue;

        possible_moves.push_back(
                Piece::get_id_from_coordinates(
                        {
                                line,
                                column
                        }
                )
        );

        if (pieces_[line][column]->get_player_id() != -1)
            break;
    }

    for (int i = 0; i < cols; ++i) {
        int line = source.line;
        int column = source.column - i;

        // avoid the the element it self
        if (line == source.line && column == source.column)
            continue;

        if (column < 0)
            continue;

        possible_moves.push_back(
                Piece::get_id_from_coordinates(
                        {
                                line,
                                column
                        }
                )
        );

        if (pieces_[line][column]->get_player_id() != -1)
            break;
    }
}

void Board::generate_vertical_movements(
        std::vector<std::string> &possible_moves,
        Piece::piece_coordinates &source) {
    // Horizontal moves
    for (int i = 0; i < rows; ++i) {
        int line = source.line + i;
        int column = source.column;

        // avoid the the element it self
        if (line == source.line && column == source.column)
            continue;

        if (line >= rows)
            continue;

        possible_moves.push_back(
                Piece::get_id_from_coordinates(
                        {
                                line,
                                column
                        }
                )
        );

        if (pieces_[line][column]->get_player_id() != -1)
            break;
    }

    for (int i = 0; i < rows; ++i) {
        int line = source.line - i;
        int column = source.column;

        // avoid the the element it self
        if (line == source.line && column == source.column)
            continue;

        if (line < 0)
            continue;

        possible_moves.push_back(
                Piece::get_id_from_coordinates(
                        {
                                line,
                                column
                        }
                )
        );

        if (pieces_[line][column]->get_player_id() != -1)
            break;
    }
}

void
Board::generate_diagonal_movements(std::vector<std::string> &possible_moves,
                                   Piece::piece_coordinates &source) {
    for (int i = 0; i < cols; ++i) {
        int line = source.line + i;
        int column = source.column + i;

        // avoid the the element it self
        if (line == source.line && column == source.column)
            continue;

        // avoid left/right edges
        if (column >= cols)
            continue;

        // avoid top/down edges
        if (line >= rows)
            continue;

        possible_moves.push_back(
                Piece::get_id_from_coordinates(
                        {
                                line,
                                column
                        }
                )
        );

        if (pieces_[line][column]->get_player_id() != -1)
            break;
    }

    for (int i = 0; i < cols; ++i) {
        int line = source.line - i;
        int column = source.column - i;

        // avoid the the element it self
        if (line == source.line && column == source.column)
            continue;

        // avoid left/right edges
        if (column < 0)
            continue;

        // avoid top/down edges
        if (line < 0)
            continue;

        possible_moves.push_back(
                Piece::get_id_from_coordinates(
                        {
                                line,
                                column
                        }
                )
        );

        if (pieces_[line][column]->get_player_id() != -1)
            break;
    }

    for (int i = 0; i < cols; ++i) {
        int line = source.line - i;
        int column = source.column + i;

        // avoid the the element it self
        if (line == source.line && column == source.column)
            continue;

        // avoid left/right edges
        if (column >= cols)
            continue;

        // avoid top/down edges
        if (line < 0)
            continue;

        possible_moves.push_back(
                Piece::get_id_from_coordinates(
                        {
                                line,
                                column
                        }
                )
        );

        if (pieces_[line][column]->get_player_id() != -1)
            break;
    }

    for (int i = 0; i < cols; ++i) {
        int line = source.line + i;
        int column = source.column - i;

        // avoid the the element it self
        if (line == source.line && column == source.column)
            continue;

        // avoid left/right edges
        if (column < 0)
            continue;

        // avoid top/down edges
        if (line >= cols)
            continue;

        possible_moves.push_back(
                Piece::get_id_from_coordinates(
                        {
                                line,
                                column
                        }
                )
        );

        [[maybe_unused]] std::string id = Piece::get_id_from_coordinates(
                {
                        line,
                        column
                }
        );

        [[maybe_unused]]  Piece *p = pieces_[line][column];

        if (pieces_[line][column]->get_player_id() != -1)
            break;
    }
}

std::vector<std::string> Board::get_all_empty_spots(const std::string &from) {
    Piece::piece_coordinates source = Piece::get_piece_coordinates_from_id(
            from);
    std::vector<std::string> empty_spots{};

    generate_horizontal_movements(empty_spots, source);
    generate_vertical_movements(empty_spots, source);
    generate_diagonal_movements(empty_spots, source);

    return empty_spots;
}

bool Board::is_valid_move(const std::string &from, const std::string &to) {
    std::vector<std::string> empty_spots = get_all_empty_spots(
            from
    );

    return find(empty_spots.begin(), empty_spots.end(), to) !=
           empty_spots.end();
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
                graphics_type = players.at(x->get_player_id()).is_dark ? "dark"
                                                                       : "light";

            std::cout << "| " << x->get_graphic(graphics_type) << " ";
        }

        std::cout << "|" << std::endl
                  << lineSeparator << std::endl;
    }

    std::cout << "     ";
    for (auto &column : Piece::cols_map_) {
        std::cout << column.first << "   ";
    }
    std::cout << std::endl;
}
