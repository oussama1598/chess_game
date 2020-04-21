#include "Piece.h"

Piece::Piece() : graphics_{{"light", " "},
                           {"dark",  " "}}, player_id_{-1}, is_first_move_{true} {}

Piece::Piece(std::map<std::string, std::string> graphics, int player_id) : graphics_{graphics}, player_id_{player_id},
                                                                           is_first_move_{true} {}

const std::map<char, size_t> Piece::cols_map_{{'A', 0},
                                              {'B', 1},
                                              {'C', 2},
                                              {'D', 3},
                                              {'E', 4},
                                              {'F', 5},
                                              {'G', 6},
                                              {'H', 7}};

Piece::piece_coordinates Piece::get_piece_coordinates_from_id(const std::string &id) {
    int line = (int) boost::lexical_cast<size_t>(id[1]) - 1;

    // this checks if the id is valid
    if (Piece::cols_map_.find(id[0]) == Piece::cols_map_.end() || line > (Piece::cols - 1))
        return {-1, -1};

    int column = Piece::cols_map_.at(id[0]);

    return {line, column};
}

int Piece::get_player_id() const {
    return player_id_;
}

bool Piece::is_first_move() const {
    return is_first_move_;
}

std::string Piece::get_graphic(std::string &type) {
    return graphics_.at(type);
}

bool Piece::is_valid_move([[maybe_unused]] Player &player, [[maybe_unused]] const std::string &from,
                          [[maybe_unused]] const std::string &to) {
    return false;
}
