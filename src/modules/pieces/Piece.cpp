#include "Piece.h"

Piece::Piece() : graphics_{{"light", " "},
                           {"dark",  " "}}, player_id_{-1},
                 is_first_move_{true} {}

Piece::Piece(std::map<std::string, std::string> graphics, int player_id)
        : graphics_{graphics}, player_id_{player_id},
          is_first_move_{true} {}

const std::map<char, size_t> Piece::cols_map_{{'A', 0},
                                              {'B', 1},
                                              {'C', 2},
                                              {'D', 3},
                                              {'E', 4},
                                              {'F', 5},
                                              {'G', 6},
                                              {'H', 7}};

Piece::piece_coordinates
Piece::get_piece_coordinates_from_id(const std::string &id) {
    int line = (int) boost::lexical_cast<size_t>(id[1]) - 1;

    // this checks if the id is valid
    if (Piece::cols_map_.find(id[0]) == Piece::cols_map_.end() ||
        line > (Piece::cols - 1))
        return {-1, -1};

    int column = Piece::cols_map_.at(id[0]);

    return {line, column};
}

std::string
Piece::get_id_from_coordinates(Piece::piece_coordinates coordinates) {
    std::string line = std::to_string(coordinates.line + 1);

    int column = coordinates.column;

    char c = std::find_if(cols_map_.begin(), cols_map_.end(),
                          [&column](const std::pair<char, int> &p) {
                              return p.second == column;
                          })->first;

    return std::string{c} + line;
}

int Piece::get_player_id() const {
    return player_id_;
}

bool Piece::is_first_move() const {
    return is_first_move_;
}

std::vector<std::string>
Piece::get_possible_moves([[maybe_unused]] bool is_top,
                          [[maybe_unused]] const std::string &from) {
    return std::vector<std::string>();
}

bool Piece::is_valid_move(Player &player,
                          const std::string &from,
                          const std::string &to) {

    std::vector<std::string> possible_moves = get_possible_moves(
            player.is_top,
            from
    );

    return find(possible_moves.begin(), possible_moves.end(), to) !=
           possible_moves.end();
}

std::string Piece::get_graphic(std::string &type) {
    return graphics_.at(type);
}
