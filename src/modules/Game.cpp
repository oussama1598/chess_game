#include "Game.h"

Game::Game() {
    add_player(1, true, true);
    add_player(1, false, false);

    initialize_game();
}

void Game::add_player(const int player_number, const bool is_dark, const bool is_top) {
    players_.push_back((Player) {player_number, is_dark, is_top});
}

void Game::initialize_game() {
    for (auto &player : players_) {
        const signed int first_row_index = player.is_top ? Board::cols : 1;
        const signed int second_row_index = player.is_top ? Board::cols - 1 : 2;

        const std::string fist_row_index_string = std::to_string(first_row_index);
        const std::string second_row_index_string = std::to_string(second_row_index);

        // add pieces manually this must be changed after
        game_board_.add_piece("A" + fist_row_index_string, Rook(player.is_dark));
        game_board_.add_piece("B" + fist_row_index_string, Knight(player.is_dark));
        game_board_.add_piece("C" + fist_row_index_string, Bishop(player.is_dark));
        game_board_.add_piece("D" + fist_row_index_string, Queen(player.is_dark));
        game_board_.add_piece("E" + fist_row_index_string, King(player.is_dark));
        game_board_.add_piece("F" + fist_row_index_string, Bishop(player.is_dark));
        game_board_.add_piece("G" + fist_row_index_string, Knight(player.is_dark));
        game_board_.add_piece("H" + fist_row_index_string, Rook(player.is_dark));

        for (auto &column : game_board_.cols_map_)
            game_board_.add_piece(std::string(1, column.first) + second_row_index_string, Pawn(player.is_dark));
    }
}

void Game::make_move(const std::string &from, const std::string &to) {
    Board::piece_coordinates from_piece_coordinates = game_board_.get_piece_coordinates_from_id(from);
    Board::piece_coordinates to_piece_coordinates = game_board_.get_piece_coordinates_from_id(to);

    if (from_piece_coordinates.line == -1 || from_piece_coordinates.column == -1) {
        std::cout << "Invalid from position!" << std::endl;

        return;
    }

    if (to_piece_coordinates.line == -1 || to_piece_coordinates.column == -1) {
        std::cout << "Invalid to position!" << std::endl;

        return;
    }

    Piece source_piece = game_board_.get_piece_at(
            from_piece_coordinates.line,
            from_piece_coordinates.column
    );

    Piece distance_piece = game_board_.get_piece_at(
            to_piece_coordinates.line,
            to_piece_coordinates.column
    );

    // swap the pieces together
    game_board_.pieces_[from_piece_coordinates.line][from_piece_coordinates.column] = distance_piece;
    game_board_.pieces_[to_piece_coordinates.line][to_piece_coordinates.column] = source_piece;
}

void Game::render() {
    game_board_.render();
}
