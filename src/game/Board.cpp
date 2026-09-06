#include "Board.h"

#include <algorithm>
#include <cmath>

Board::Board() {
    build_board();
}

Board::Board(const Board &other) {
    for (int line = 0; line < rows; ++line) {
        for (int column = 0; column < cols; ++column) {
            Piece *piece = other.pieces_[line][column];
            pieces_[line][column] = piece == nullptr ? nullptr : piece->clone();
        }
    }
}

Board::Board(Board &&other) noexcept : pieces_{other.pieces_} {
    for (auto &line: other.pieces_) {
        line.fill(nullptr);
    }
}

Board &Board::operator=(const Board &other) {
    if (this == &other) {
        return *this;
    }

    Board copy(other);
    std::swap(pieces_, copy.pieces_);
    return *this;
}

Board &Board::operator=(Board &&other) noexcept {
    if (this == &other) {
        return *this;
    }

    clear();
    pieces_ = other.pieces_;
    for (auto &line: other.pieces_) {
        line.fill(nullptr);
    }
    return *this;
}

Board::~Board() {
    clear();
}

void Board::clear() noexcept {
    for (auto &line: pieces_) {
        for (Piece *&piece: line) {
            delete piece;
            piece = nullptr;
        }
    }
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
        delete piece;
        return;
    }

    delete pieces_[piece_coordinates.line][piece_coordinates.column];

    pieces_[piece_coordinates.line][piece_coordinates.column] = piece;
}

void Board::build_board() {
    clear();

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

    if (source.line == -1 || source.column == -1) {
        return empty_spots;
    }

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

    const Piece::piece_coordinates from_coordinates =
            Piece::get_piece_coordinates_from_id(from);
    const Piece::piece_coordinates to_coordinates =
            Piece::get_piece_coordinates_from_id(to);

    if (source_piece == nullptr || from_coordinates.line == -1 ||
        to_coordinates.line == -1 ||
        get_piece_at(from_coordinates.line, from_coordinates.column) != source_piece ||
        source_piece->get_player_id() != source_player.player_id) {
        return false;
    }

    if (!source_piece->is_valid_move(source_player, from, to)) {
        return false;
    }

    Piece *destination_piece = get_piece_at(to_coordinates.line,
                                            to_coordinates.column);

    // check if the current piece is a knight since
    // the next check is not applicable to it
    bool is_knight = dynamic_cast<const Knight *>(source_piece) != nullptr;
    bool is_pawn = dynamic_cast<const Pawn *>(source_piece) != nullptr;
    bool is_king = dynamic_cast<const King *>(source_piece) != nullptr;

    if (destination_piece->get_player_id() == source_player.player_id)
        return false;

    // Kings are never captured in chess; checkmate ends the game first.
    if (dynamic_cast<const King *>(destination_piece) != nullptr)
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

    if (is_king && is_castling_move(source_piece, from, to)) {
        return can_castle(source_player, source_piece, from, to);
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

    if (king_coordinates.line == -1 || king_coordinates.column == -1) {
        return false;
    }

    return !is_square_attacked(player, king_coordinates);
}

bool Board::is_square_attacked(Player &player, Piece::piece_coordinates target) {
    const std::string target_id = Piece::get_id_from_coordinates(target);

    if (target_id.empty()) {
        return false;
    }

    for (int line = 0; line < rows; ++line) {
        for (int column = 0; column < cols; ++column) {
            Piece *piece = pieces_[line][column];

            if (piece == nullptr || piece->get_player_id() == -1 ||
                piece->get_player_id() == player.player_id) {
                continue;
            }

            const int line_delta = target.line - line;
            const int column_delta = target.column - column;

            if (dynamic_cast<const Pawn *>(piece) != nullptr) {
                const bool attacker_is_top = !player.is_top;
                const int direction = attacker_is_top ? -1 : 1;
                if (line_delta == direction && std::abs(column_delta) == 1) {
                    return true;
                }
                continue;
            }

            if (dynamic_cast<const King *>(piece) != nullptr) {
                if (std::abs(line_delta) <= 1 && std::abs(column_delta) <= 1 &&
                    (line_delta != 0 || column_delta != 0)) {
                    return true;
                }
                continue;
            }

            Player attacker{piece->get_player_id(), false, !player.is_top, false};
            const std::string from = Piece::get_id_from_coordinates({line, column});

            if (!piece->is_valid_move(attacker, from, target_id)) {
                continue;
            }

            if (dynamic_cast<const Knight *>(piece) != nullptr || is_valid_move(from, target_id)) {
                return true;
            }
        }
    }

    return false;
}

bool Board::perform_move(Player &source_player,
                         Piece::piece_coordinates from_piece_coordinates,
                         Piece *source_piece,
                         Piece::piece_coordinates to_piece_coordinates,
                         Piece *destination_piece, bool reset) {
    std::string from = Piece::get_id_from_coordinates(from_piece_coordinates);
    std::string to = Piece::get_id_from_coordinates(to_piece_coordinates);

    bool is_king = dynamic_cast<const King *>(source_piece) != nullptr;
    bool did_castle = is_king && is_castling_move(source_piece, from, to) &&
                      can_castle(source_player, source_piece, from, to);
    auto *new_piece = new Piece();

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
    } else {
        if (did_castle) {
            const auto castle_coordinates = where_to_castle(source_player, to);
            pieces_[castle_coordinates[1].line][castle_coordinates[1].column]->did_move();
        }

        delete destination_piece;
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

    if (coordinates.line == -1 || coordinates.column == -1) {
        return {};
    }

    Piece *piece = get_piece_at(coordinates.line, coordinates.column);

    if (piece == nullptr || piece->get_player_id() != player.player_id) {
        return {};
    }

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
    if (!is_castling_move(source_piece, from, to) || !source_piece->is_first_move() ||
        source_piece->get_player_id() != player.player_id) {
        return false;
    }

    const Piece::piece_coordinates from_coordinates =
            Piece::get_piece_coordinates_from_id(from);
    const Piece::piece_coordinates to_coordinates =
            Piece::get_piece_coordinates_from_id(to);
    const int home_line = player.is_top ? rows - 1 : 0;

    if (from_coordinates.line != home_line || from_coordinates.column != 4 ||
        to_coordinates.line != home_line ||
        pieces_[from_coordinates.line][from_coordinates.column] != source_piece) {
        return false;
    }

    const bool king_side = to_coordinates.column == 6;
    const Piece::piece_coordinates rook_coordinates{home_line, king_side ? cols - 1 : 0};
    const Piece::piece_coordinates rook_destination{home_line, king_side ? 5 : 3};
    Piece *rook = pieces_[rook_coordinates.line][rook_coordinates.column];

    if (dynamic_cast<const Rook *>(rook) == nullptr ||
        rook->get_player_id() != player.player_id || !rook->is_first_move()) {
        return false;
    }

    const int first_column = std::min(from_coordinates.column, rook_coordinates.column) + 1;
    const int last_column = std::max(from_coordinates.column, rook_coordinates.column);
    for (int column = first_column; column < last_column; ++column) {
        if (pieces_[home_line][column]->get_player_id() != -1) {
            return false;
        }
    }

    if (!is_king_safe(player)) {
        return false;
    }

    auto king_is_safe_on = [&](Piece::piece_coordinates destination,
                               bool move_rook) {
        Piece *destination_piece = pieces_[destination.line][destination.column];
        pieces_[from_coordinates.line][from_coordinates.column] = destination_piece;
        pieces_[destination.line][destination.column] = source_piece;

        Piece *rook_destination_piece = nullptr;
        if (move_rook) {
            rook_destination_piece = pieces_[rook_destination.line][rook_destination.column];
            pieces_[rook_coordinates.line][rook_coordinates.column] = rook_destination_piece;
            pieces_[rook_destination.line][rook_destination.column] = rook;
        }

        const bool safe = is_king_safe(player);

        if (move_rook) {
            pieces_[rook_coordinates.line][rook_coordinates.column] = rook;
            pieces_[rook_destination.line][rook_destination.column] = rook_destination_piece;
        }

        pieces_[from_coordinates.line][from_coordinates.column] = source_piece;
        pieces_[destination.line][destination.column] = destination_piece;
        return safe;
    };

    const Piece::piece_coordinates transit{home_line, king_side ? 5 : 3};
    return king_is_safe_on(transit, false) && king_is_safe_on(to_coordinates, true);
}

bool Board::is_castling_move(Piece *source_piece, const std::string &from,
                             const std::string &to) const {
    if (dynamic_cast<const King *>(source_piece) == nullptr) {
        return false;
    }

    const Piece::piece_coordinates from_coordinates =
            Piece::get_piece_coordinates_from_id(from);
    const Piece::piece_coordinates to_coordinates =
            Piece::get_piece_coordinates_from_id(to);

    return from_coordinates.line != -1 && to_coordinates.line != -1 &&
           from_coordinates.line == to_coordinates.line &&
           std::abs(from_coordinates.column - to_coordinates.column) == 2;
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

void Board::swap_castle(Player &source_player, const std::string &to) {
    std::vector<Piece::piece_coordinates> castle_coordinates = where_to_castle(
            source_player,
            to
    );

    if (castle_coordinates[0].line == -1 || castle_coordinates[1].line == -1) {
        return;
    }

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
