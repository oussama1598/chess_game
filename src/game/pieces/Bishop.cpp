#include "Bishop.h"

Bishop::Bishop(int player_id) : Piece{'B', player_id} {}

std::vector<std::string>
Bishop::get_possible_moves([[maybe_unused]] bool is_top,
                           const std::string &from) {
    piece_coordinates source = get_piece_coordinates_from_id(from);
    std::vector<std::string> possible_moves;

    generate_diagonal_movements(possible_moves, source);

    return possible_moves;
}
