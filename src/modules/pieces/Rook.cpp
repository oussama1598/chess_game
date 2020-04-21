#include "Rook.h"

Rook::Rook(int player_id) : Piece{{{"light", "♖"}, {"dark", "♜"}}, player_id} {}

std::vector<std::string>
Rook::get_possible_moves([[maybe_unused]] bool is_top,
                           const std::string &from) {
    piece_coordinates source = get_piece_coordinates_from_id(from);
    std::vector<std::string> possible_moves;

    // Horizontal moves
    for (int i = -cols; i < cols; ++i) {
        int line = source.line;
        int column = source.column + i;

        // avoid the the element it self
        if (line == source.line && column == source.column)
            continue;

        // avoid left/right edges
        if (column < 0 || column >= cols)
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

    // Vertical moves
    for (int i = -cols; i < cols; ++i) {
        int line = source.line + i;
        int column = source.column;

        // avoid the the element it self
        if (line == source.line && column == source.column)
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


    return possible_moves;
}
