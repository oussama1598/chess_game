#include "Game.h"

Game::Game() : is_game_in_check_{false}, is_game_ended_{false} {
    add_player(0, true, false);
    add_player(1, false, true);

    initialize_game();
}

std::vector<Player> &Game::get_players() {
    return players_;
}

bool Game::is_game_in_check() const {
    return is_game_in_check_;
}

bool Game::is_game_ended() const {
    return is_game_ended_;
}

Board::piecesType &Game::get_board_pieces() {
    return game_board_.get_pieces();
}

Player *Game::get_current_player() {
    return current_player_;
}

void Game::add_player(const int player_id, const bool is_dark,
                      const bool is_top) {
    players_.push_back((Player) {player_id, is_dark, is_top, false});
}

void Game::initialize_game() {
    current_player_ = &players_.at(0);

    for (auto &player : players_) {
        const signed int first_row_index = player.is_top ? Board::cols : 1;
        const signed int second_row_index = player.is_top ? Board::cols - 1 : 2;

        const std::string fist_row_index_string = std::to_string(
                first_row_index);
        const std::string second_row_index_string =
                std::to_string(second_row_index);

        // add pieces manually this must be changed after
        game_board_.add_piece("A" + fist_row_index_string,
                              new Rook(player.player_id));
        game_board_.add_piece("B" + fist_row_index_string,
                              new Knight(player.player_id));
        game_board_.add_piece("C" + fist_row_index_string,
                              new Bishop(player.player_id));
        game_board_.add_piece("D" + fist_row_index_string,
                              new Queen(player.player_id));
        game_board_.add_piece("E" + fist_row_index_string,
                              new King(player.player_id));
        game_board_.add_piece("F" + fist_row_index_string,
                              new Bishop(player.player_id));
        game_board_.add_piece("G" + fist_row_index_string,
                              new Knight(player.player_id));
        game_board_.add_piece("H" + fist_row_index_string,
                              new Rook(player.player_id));

        for (auto &column : Piece::cols_map_)
            game_board_.add_piece(std::string(1, column.first)
                                  + second_row_index_string,
                                  new Pawn(player.player_id));
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
    if (is_game_ended_) return;

    if (from == to)
        throw std::runtime_error(Errors::SOURCE_SAME_AS_DESTINATION);

    Piece::piece_coordinates from_piece_coordinates =
            Piece::get_piece_coordinates_from_id(from);
    Piece::piece_coordinates to_piece_coordinates =
            Piece::get_piece_coordinates_from_id(to);

    if (from_piece_coordinates.line == -1 ||
        from_piece_coordinates.column == -1)
        throw std::runtime_error(Errors::INVALID_SOURCE_POSITION);

    if (to_piece_coordinates.line == -1 || to_piece_coordinates.column == -1)
        throw std::runtime_error(Errors::INVALID_DESTINATION_POSITION);

    Piece *source_piece = game_board_.get_piece_at(from_piece_coordinates.line,
                                                   from_piece_coordinates.column);

    if (source_piece->get_player_id() == -1)
        throw std::runtime_error(Errors::ILLEGAL_MOVE);

    Player source_player = players_.at(source_piece->get_player_id());

    if (source_player.player_id != current_player_->player_id)
        throw std::runtime_error(Errors::NOT_PLAYERS_PIECE);

    if (!game_board_.can_make_move(source_piece, source_player, from, to))
        throw std::runtime_error(Errors::ILLEGAL_MOVE);

    Piece *destination_piece = game_board_.get_piece_at(
            to_piece_coordinates.line,
            to_piece_coordinates.column
    );

    if (destination_piece->get_player_id() == source_piece->get_player_id())
        throw std::runtime_error(Errors::ILLEGAL_MOVE);

    if (!game_board_.perform_move(source_player, from_piece_coordinates,
                                  source_piece,
                                  to_piece_coordinates, destination_piece))
        throw std::runtime_error(Errors::KING_IS_NOT_SAFE);

    // if it was previously in check reset it
    is_game_in_check_ = false;

    // check if the other player king's safe
    Player opponent_player = players_.at(
            (source_player.player_id + 1) % players_.size());

    is_game_in_check_ = !game_board_.is_king_safe(opponent_player);

    if (!game_board_.player_has_valid_move(opponent_player)) {
        // TODO: Make the console renderer print those messages
//        if (is_game_in_check_)
//            std::cout << "Check mate" << std::endl;
//        else
//            std::cout << "Stale mate" << std::endl;

        is_game_ended_ = true;
        return;
    }

    current_player_ = &players_.at(opponent_player.player_id);
}
