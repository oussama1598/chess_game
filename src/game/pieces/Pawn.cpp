#include "Pawn.h"

Pawn::Pawn(int player_id) : Piece{'P', player_id} {}

Piece *Pawn::clone() const {
    return new Pawn(*this);
}

std::vector<std::string>
Pawn::get_possible_moves(const bool is_top, const std::string &from) {
    piece_coordinates source = get_piece_coordinates_from_id(from);
    bool can_move_two_squares = is_first_move_ &&
                                (is_top ? source.line == rows - 2 : source.line == 1);

    std::vector<std::string> possible_moves;

    for (int i = -1; i < 2; ++i) {
        int line = source.line + (is_top ? -1 : 1);
        int column = source.column + i;

        // avoid left/right edges
        if (column < 0 || column >= cols)
            continue;

        // avoid top/down edges
        if (line < 0 || line >= rows)
            continue;

        possible_moves.push_back(
                get_id_from_coordinates(
                        {
                                line,
                                column
                        }
                )
        );
    }

    // add the first move, two steps manually
    if (can_move_two_squares)
        possible_moves.push_back(
                get_id_from_coordinates(
                        {
                                source.line + (is_top ? -2 : 2),
                                source.column
                        }
                )
        );

    return possible_moves;
}
