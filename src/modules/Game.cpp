#include "Game.h"

Game::Game() {
    add_player(0, true, true);
    add_player(1, false, false);

    initialize_game();
}

void Game::add_player(const int player_id, const bool is_dark, const bool is_top) {
    players_.push_back((Player) {player_id, is_dark, is_top, false});
}

void Game::initialize_game() {
    for (auto &player : players_) {
        const signed int first_row_index = player.is_top ? Board::cols : 1;
        const signed int second_row_index = player.is_top ? Board::cols - 1 : 2;

        const std::string fist_row_index_string = std::to_string(first_row_index);
        const std::string second_row_index_string = std::to_string(second_row_index);

        // TODO: delete the previous objects

        // add pieces manually this must be changed after
        game_board_.add_piece("A" + fist_row_index_string, new Rook(player.player_id));
        game_board_.add_piece("B" + fist_row_index_string, new Knight(player.player_id));
        game_board_.add_piece("C" + fist_row_index_string, new Bishop(player.player_id));
        game_board_.add_piece("D" + fist_row_index_string, new Queen(player.player_id));
        game_board_.add_piece("E" + fist_row_index_string, new King(player.player_id));
        game_board_.add_piece("F" + fist_row_index_string, new Bishop(player.player_id));
        game_board_.add_piece("G" + fist_row_index_string, new Knight(player.player_id));
        game_board_.add_piece("H" + fist_row_index_string, new Rook(player.player_id));

        for (auto &column : Piece::cols_map_)
            game_board_.add_piece(std::string(1, column.first) + second_row_index_string, new Pawn(player.player_id));
    }
}

Player &Game::get_player_by_number(const size_t id) {
    try {
        return players_.at(id);
    } catch (std::out_of_range &e) {
        throw std::runtime_error(Errors::PLAYER_DOES_NOT_EXIST);
    }
}


void Game::make_move(const std::string &from, const std::string &to) {
    // TODO: check if its the current's player piece

    if (from == to)
        throw std::runtime_error(Errors::SOURCE_SAME_AS_DESTINATION);

    Piece::piece_coordinates from_piece_coordinates = Piece::get_piece_coordinates_from_id(from);
    Piece::piece_coordinates to_piece_coordinates = Piece::get_piece_coordinates_from_id(to);

    if (from_piece_coordinates.line == -1 || from_piece_coordinates.column == -1)
        throw std::runtime_error(Errors::INVALID_SOURCE_POSITION);

    if (to_piece_coordinates.line == -1 || to_piece_coordinates.column == -1)
        throw std::runtime_error(Errors::INVALID_DESTINATION_POSITION);

    Piece *source_piece = game_board_.get_piece_at(
            from_piece_coordinates.line,
            from_piece_coordinates.column
    );

    if (source_piece->get_player_id() == -1)
        throw std::runtime_error(Errors::ILLEGAL_MOVE);

    Player source_player = players_.at(source_piece->get_player_id());

    if (!source_piece->is_valid_move(source_player, from, to))
        throw std::runtime_error(Errors::ILLEGAL_MOVE);

//    Piece distance_piece = game_board_.get_piece_at(
//            to_piece_coordinates.line,
//            to_piece_coordinates.column
//    );





    // swap the pieces together
//    game_board_.pieces_[from_piece_coordinates.line][from_piece_coordinates.column] = distance_piece;
//    game_board_.pieces_[to_piece_coordinates.line][to_piece_coordinates.column] = source_piece;
}

void Game::render() {
    game_board_.render(players_);
}
