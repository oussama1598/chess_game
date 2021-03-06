#include "Queen.h"

Queen::Queen(int player_id) : Piece{'Q', player_id} {}

std::vector<std::string>
Queen::get_possible_moves([[maybe_unused]] bool is_top,
                           const std::string &from) {
    piece_coordinates source = get_piece_coordinates_from_id(from);
    std::vector<std::string> possible_moves;

    generate_horizontal_movements(possible_moves, source);
    generate_vertical_movements(possible_moves, source);

    generate_diagonal_movements(possible_moves, source);

    return possible_moves;
}
