#include "Piece.h"

Piece::Piece() : graphics_{{"light", " "},
                           {"dark",  " "}}, player_id_{-1}, is_first_move_{true} {}

Piece::Piece(std::map<std::string, std::string> graphics, int player_id) : graphics_{graphics}, player_id_{player_id},
                                                                           is_first_move_{true} {}

int Piece::get_player_id() const {
    return player_id_;
}

bool Piece::is_first_move() const {
    return is_first_move_;
}

std::string Piece::get_graphic(std::string &type) {
    return graphics_.at(type);
}

bool Piece::is_valid_move([[maybe_unused]] Player &player, [[maybe_unused]] Piece::piece_coordinates source,
                          [[maybe_unused]] Piece::piece_coordinates destination) {
    return false;
}
