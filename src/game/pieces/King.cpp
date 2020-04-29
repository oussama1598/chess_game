#include "King.h"

King::King(int player_id) : Piece{'K', player_id} {}

std::vector<std::string>
King::get_possible_moves(bool is_top,
                         const std::string &from) {
    piece_coordinates source = get_piece_coordinates_from_id(from);
    std::vector<std::string> possible_moves;

    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            int line = source.line + i;
            int column = source.column + j;

            // avoid the the element it self
            if (line == source.line && column == source.column)
                continue;

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
    }


    if (is_first_move_) {
        if (!is_top) {
            possible_moves.emplace_back("G1");
            possible_moves.emplace_back("C1");
        } else {
            possible_moves.emplace_back("G8");
            possible_moves.emplace_back("C8");
        }
    }


    return possible_moves;
}
