#include "Board.h"

Board::Board() {
    build_board();
}

Board::piecesType &Board::get_pieces() {
    return pieces_;
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

    delete pieces_[piece_coordinates.line][piece_coordinates.column];

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

bool Board::can_make_move(Piece *source_piece, Player &source_player,
                          const std::string &from,
                          const std::string &to) {
    if (!source_piece->is_valid_move(source_player, from, to))
        return false;

    Piece::piece_coordinates from_coordinates = Piece::get_piece_coordinates_from_id(
            from);
    Piece::piece_coordinates to_coordinates = Piece::get_piece_coordinates_from_id(
            to);

    // check if the current piece is a knight since
    // the next check is not applicable to it
    bool is_knight = dynamic_cast<const Knight *>(source_piece) != nullptr;
    bool is_pawn = dynamic_cast<const Pawn *>(source_piece) != nullptr;

    if (is_knight) return true;

    if (!is_valid_move(from, to)) return false;

    // checking if the movement was diagonally for pawn
    if (is_pawn) {
        Piece *destination_piece = get_piece_at(to_coordinates.line,
                                                to_coordinates.column);
        if (from_coordinates.column != to_coordinates.column) {
            // moved to an empty piece diagonally (not allowed)
            if (destination_piece->get_player_id() == -1) return false;
            // move diagonally to spot where i the player owns the piece within it
            if (destination_piece->get_player_id() ==
                source_player.player_id)
                return false;
        } else {
            // if the direct spot is not empty (not allowed)
            if (destination_piece->get_player_id() != -1) return false;
        }
    }

    return true;
}

Piece::piece_coordinates Board::find_king(Player &player) {
    for (int i = 0; (size_t) i < pieces_.size(); ++i)
        for (int j = 0; (size_t) j < pieces_[i].size(); ++j) {
            Piece *piece = pieces_[i][j];
            bool is_king = dynamic_cast<const King *>(piece) != nullptr;

            if (is_king && piece->get_player_id() == player.player_id)
                return {i, j};

        }

    return {-1, -1};
}

bool Board::is_king_safe(Player &player) {
    Piece::piece_coordinates king_coordinates = find_king(player);
    std::string king_id = Piece::get_id_from_coordinates(king_coordinates);
    [[maybe_unused]] King *king = (King *) pieces_[king_coordinates.line][king_coordinates.column];

    for (int i = 0; (size_t) i < pieces_.size(); ++i)
        for (int j = 0; (size_t) j < pieces_[i].size(); ++j) {
            Piece *piece = pieces_[i][j];
            std::string target_id = Piece::get_id_from_coordinates({i, j});

            if (piece->get_player_id() == -1 ||
                piece->get_player_id() == player.player_id || piece == king)
                continue;

            if (can_make_move(piece, player, king_id, target_id))
                return false;
        }

    return true;
}

bool Board::perform_move(Player &source_player,
                         Piece::piece_coordinates from_piece_coordinates,
                         Piece *source_piece,
                         Piece::piece_coordinates to_piece_coordinates,
                         Piece *destination_piece, bool reset) {
    Piece *new_piece = new Piece();

    // Change the current piece to the wanted place
    pieces_[to_piece_coordinates.line][to_piece_coordinates.column]
            = source_piece;
    // create a new empty piece
    pieces_[from_piece_coordinates.line][from_piece_coordinates.column]
            = new_piece;

    if (!is_king_safe(source_player)) {
        // revert back the actions
        pieces_[from_piece_coordinates.line][from_piece_coordinates.column] = source_piece;
        pieces_[to_piece_coordinates.line][to_piece_coordinates.column]
                = destination_piece;

        delete new_piece;

        return false;
    } else if (reset) {
        // revert back the actions
        pieces_[from_piece_coordinates.line][from_piece_coordinates.column] = source_piece;
        pieces_[to_piece_coordinates.line][to_piece_coordinates.column]
                = destination_piece;

        delete new_piece;
    }

    return true;
}

bool Board::player_has_valid_move(Player &player) {
    for (int i = 0; (size_t) i < pieces_.size(); ++i)
        for (int j = 0; (size_t) j < pieces_[i].size(); ++j) {
            Piece *source_piece = pieces_[i][j];
            std::string from = Piece::get_id_from_coordinates({i, j});

            if (source_piece->get_player_id() == player.player_id) {
                for (int k = 0; (size_t) k < pieces_.size(); ++k)
                    for (int l = 0; (size_t) l < pieces_[k].size(); ++l) {
                        Piece *destination_piece = pieces_[k][l];
                        std::string to = Piece::get_id_from_coordinates({k, l});

                        if (destination_piece->get_player_id() == -1) {
                            if (can_make_move(source_piece, player, from, to)) {
                                if (perform_move(player,
                                                 {i, j},
                                                 source_piece,
                                                 {k, l},
                                                 destination_piece, true)) {
                                    return true;
                                }
                            }
                        }
                    }
            }
        }

    return false;
}
