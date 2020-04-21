#include "Pawn.h"

Pawn::Pawn(int player_id) : Piece{{{"light", "♙"}, {"dark", "♟"}}, player_id} {}

bool Pawn::is_valid_move([[maybe_unused]] Player &player, [[maybe_unused]] const std::string &from,
                         [[maybe_unused]] const std::string &to) {
    piece_coordinates source = get_piece_coordinates_from_id(from);
    piece_coordinates destination = get_piece_coordinates_from_id(to);

    //TODO: create a list of possible movements from that place and check against it

    // Horizontal movement restriction
    if (source.column != destination.column)
        return false;

    int first_allowed_line = source.line + (player.is_top ? -1 : 1);
    int second_allowed_line = source.line + (player.is_top ? -2 : 2);

    if (is_first_move_ && (first_allowed_line != destination.line && second_allowed_line != destination.line))
        return false;

    if (!is_first_move_ && first_allowed_line != destination.line)
        return false;

    return true;
}
