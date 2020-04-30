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

    if (!source_piece->is_valid_move(source_player, from, to)) {
        return false;
    }

    Piece::piece_coordinates from_coordinates = Piece::get_piece_coordinates_from_id(
            from);
    Piece::piece_coordinates to_coordinates = Piece::get_piece_coordinates_from_id(
            to);

    Piece *destination_piece = get_piece_at(to_coordinates.line,
                                            to_coordinates.column);

    // check if the current piece is a knight since
    // the next check is not applicable to it
    bool is_knight = dynamic_cast<const Knight *>(source_piece) != nullptr;
    bool is_pawn = dynamic_cast<const Pawn *>(source_piece) != nullptr;
    bool is_king = dynamic_cast<const King *>(source_piece) != nullptr;

    if (destination_piece->get_player_id() == source_player.player_id)
        return false;

    if (is_knight) return true;

    if (!is_valid_move(from, to)) return false;

    // checking if the movement was diagonally for pawn
    if (is_pawn) {
        if (from_coordinates.column != to_coordinates.column) {
            // moved to an empty piece diagonally (not allowed)
            if (destination_piece->get_player_id() == -1) return false;
        } else {
            // if the direct spot is not empty (not allowed)
            if (destination_piece->get_player_id() != -1) return false;
        }
    }

    return !(is_king && !can_castle(source_player, source_piece, from, to));

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
    King *king = (King *) pieces_[king_coordinates.line][king_coordinates.column];

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
    auto *new_piece = new Piece();

    std::string from = Piece::get_id_from_coordinates(from_piece_coordinates);
    std::string to = Piece::get_id_from_coordinates(to_piece_coordinates);

    bool is_king = dynamic_cast<const King *>(source_piece) != nullptr;
    bool did_castle = is_king && can_castle(source_player, source_piece, from, to);

    // Change the current piece to the wanted place
    pieces_[to_piece_coordinates.line][to_piece_coordinates.column]
            = source_piece;
    // create a new empty piece
    pieces_[from_piece_coordinates.line][from_piece_coordinates.column]
            = new_piece;

    if (did_castle) swap_castle(source_player, to);


    if (!is_king_safe(source_player)) {
        // revert back the actions
        pieces_[from_piece_coordinates.line][from_piece_coordinates.column] = source_piece;
        pieces_[to_piece_coordinates.line][to_piece_coordinates.column]
                = destination_piece;

        delete new_piece;

        if (did_castle) swap_castle(source_player, to);

        return false;
    } else if (reset) {
        // revert back the actions
        pieces_[from_piece_coordinates.line][from_piece_coordinates.column] = source_piece;
        pieces_[to_piece_coordinates.line][to_piece_coordinates.column]
                = destination_piece;

        delete new_piece;

        if (did_castle) swap_castle(source_player, to);
    }

    return true;
}

bool Board::player_has_valid_move(Player &player) {
    for (int i = 0; (size_t) i < pieces_.size(); ++i)
        for (int j = 0; (size_t) j < pieces_[i].size(); ++j) {
            Piece *source_piece = pieces_[i][j];
            std::string from = Piece::get_id_from_coordinates({i, j});

            if (source_piece->get_player_id() == player.player_id) {
                std::vector<std::string> possible_moves = get_possible_moves_for(player, from);

                if (!possible_moves.empty())
                    return true;
            }
        }

    return false;
}

std::vector<std::string> Board::get_possible_moves_for(Player &player, const std::string &from) {
    Piece::piece_coordinates coordinates = Piece::get_piece_coordinates_from_id(from);
    Piece *piece = get_piece_at(coordinates.line, coordinates.column);

    std::vector<std::string> piece_possible_spots = piece->get_possible_moves(player.is_top, from);
    std::vector<std::string> board_empty_spots = get_all_empty_spots(from);

    bool is_knight = dynamic_cast<const Knight *>(piece) != nullptr;

    std::vector<std::string> spots_matching;
    std::vector<std::string> possible_moves;

    // if it's knight, the possible moves follow the rules applied by the board
    if (!is_knight) {
        for (auto &spot: piece_possible_spots) {
            if (find(board_empty_spots.begin(), board_empty_spots.end(), spot) !=
                board_empty_spots.end()) {
                spots_matching.push_back(spot);
            }
        }
    } else {
        // the moves are only the one provided by the piece it self
        spots_matching = piece_possible_spots;
    }

    for (auto &spot: spots_matching) {
        if (can_make_move(piece, player, from, spot)) {
            Piece::piece_coordinates destination_coordinates = Piece::get_piece_coordinates_from_id(
                    spot);
            Piece *destination_piece = get_piece_at(destination_coordinates.line,
                                                    destination_coordinates.column);

            if (perform_move(player,
                             coordinates,
                             piece,
                             destination_coordinates,
                             destination_piece, true)) {
                possible_moves.push_back(spot);
            }
        }
    }

    return possible_moves;
}

bool Board::can_castle(Player &player, Piece *source_piece, const std::string &from,
                       const std::string &to) {
    if (!source_piece->is_first_move()) return true;

    Piece::piece_coordinates left_rook_coordinates{0, 0};
    Piece::piece_coordinates right_rook_coordinates{0, Piece::cols - 1};

    if (player.is_top) {
        left_rook_coordinates.line = Piece::rows - 1;
        right_rook_coordinates.line = Piece::rows - 1;
    }

    std::string to_left_id = Piece::get_id_from_coordinates({left_rook_coordinates.line, 2});
    std::string to_right_id = Piece::get_id_from_coordinates(
            {right_rook_coordinates.line, Piece::cols - 2});

    if (to != to_left_id && to != to_right_id) return true;

    Piece *left_rook = pieces_[left_rook_coordinates.line][left_rook_coordinates.column];
    Piece *right_rook = pieces_[right_rook_coordinates.line][right_rook_coordinates.column];

    bool is_left_rook = dynamic_cast<const Rook *>(left_rook) != nullptr;
    bool is_right_rook = dynamic_cast<const Rook *>(right_rook) != nullptr;


    if (to == to_left_id && is_left_rook && left_rook->is_first_move()) {
        return is_valid_move(from, Piece::get_id_from_coordinates(left_rook_coordinates));
    }

    if (to == to_right_id && is_right_rook && right_rook->is_first_move())
        return is_valid_move(from, Piece::get_id_from_coordinates(right_rook_coordinates));

    return false;
}

std::vector<Piece::piece_coordinates>
Board::where_to_castle(Player &player, const std::string &to) {
    Piece::piece_coordinates left_rook_coordinates{0, 0};
    Piece::piece_coordinates right_rook_coordinates{0, Piece::cols - 1};

    if (player.is_top) {
        left_rook_coordinates.line = Piece::rows - 1;
        right_rook_coordinates.line = Piece::rows - 1;
    }

    std::string to_left_id = Piece::get_id_from_coordinates({left_rook_coordinates.line, 2});
    std::string to_right_id = Piece::get_id_from_coordinates(
            {right_rook_coordinates.line, Piece::cols - 2});

    if (to == to_left_id) {
        return {left_rook_coordinates,
                {left_rook_coordinates.line, left_rook_coordinates.column + 3}};
    }

    if (to == to_right_id) {
        return {right_rook_coordinates,
                {right_rook_coordinates.line, right_rook_coordinates.column - 2}};
    }

    return {{-1, -1},
            {-1, -1}};
}

void Board::swap_castle(Player &source_player, std::string &to) {
    std::vector<Piece::piece_coordinates> castle_coordinates = where_to_castle(
            source_player,
            to
    );

    Piece *rook = pieces_[castle_coordinates[0].line][castle_coordinates[0].column];
    Piece *empty_spot = pieces_[castle_coordinates[1].line][castle_coordinates[1].column];

    pieces_[castle_coordinates[0].line][castle_coordinates[0].column]
            = empty_spot;
    // create a new empty piece
    pieces_[castle_coordinates[1].line][castle_coordinates[1].column]
            = rook;
}

std::vector<std::pair<std::string, std::vector<std::string>>>
Board::get_all_valid_moves_for(Player &player) {
    std::vector<std::pair<std::string, std::vector<std::string>>> all_possible_moves;

    for (int i = 0; (size_t) i < pieces_.size(); ++i)
        for (int j = 0; (size_t) j < pieces_[i].size(); ++j) {
            Piece *source_piece = pieces_[i][j];
            std::string from = Piece::get_id_from_coordinates({i, j});

            if (source_piece->get_player_id() == player.player_id) {
                std::vector<std::string> possible_moves = get_possible_moves_for(player, from);

                if (!possible_moves.empty())
                    all_possible_moves.emplace_back(from, possible_moves);
            }
        }

    return all_possible_moves;
}
