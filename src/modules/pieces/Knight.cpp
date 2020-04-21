#include "Knight.h"

Knight::Knight(int player_id) : Piece{{{"light", "♘"}, {"dark", "♞"}},
                                      player_id} {}

std::vector<std::string>
Knight::get_possible_moves([[maybe_unused]] bool is_top,
                           const std::string &from) {
    piece_coordinates source = get_piece_coordinates_from_id(from);
    std::array<std::pair<int, int>, 8> possible_moves_coordinates{
            {
                    {1, 2},
                    {2, 1},
                    {-1, 2},
                    {-2, 1},
                    {1, -2},
                    {2, -1},
                    {-1, -2},
                    {-2, -1}
            }
    };
    std::vector<std::string> possible_moves;

    for (auto &coordinate: possible_moves_coordinates) {
        int line = source.line + coordinate.first;
        int column = source.column + coordinate.second;

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

    return possible_moves;
}
