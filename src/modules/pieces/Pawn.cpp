#include "Pawn.h"

Pawn::Pawn(int player_id) : Piece{{{"light", "♙"}, {"dark", "♟"}}, player_id} {}

bool Pawn::is_valid_move([[maybe_unused]] Player &player, [[maybe_unused]] Piece::piece_coordinates source,
                         [[maybe_unused]] Piece::piece_coordinates destination) {
    // TODO: check for horizontal movement

    int first_allowed_line = source.line + (player.is_top ? -1 : 1);
    int second_allowed_line = source.line + (player.is_top ? -2 : 2);

    std::cout << is_first_move_ << " " << second_allowed_line << " " << destination.line << std::endl;

    if (is_first_move_ && (first_allowed_line != destination.line && second_allowed_line != destination.line))
        return false;

    if (!is_first_move_ && first_allowed_line != destination.line)
        return false;

    return true;
}
