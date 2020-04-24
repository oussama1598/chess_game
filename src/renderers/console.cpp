#include "console.h"

void ConsoleRenderer::clear() {
    std::cout << "\x1B[2J\x1B[H";
}

void ConsoleRenderer::render(Game &game) {
    if (game.is_game_in_check())
        std::cout << "Game is in check" << std::endl;

    Board::piecesType pieces = game.get_board_pieces();
    std::vector<Player> players = game.get_players();
    std::string lineSeparator = "   +---+---+---+---+---+---+---+---+";

    std::cout << lineSeparator << std::endl;

    for (size_t y = 0; y < pieces.size(); ++y) {
        size_t inverse_y = pieces.size() - 1 - y;

        std::cout << " " << inverse_y + 1 << " ";

        for (auto &x : pieces[inverse_y]) {
            int player_id = x->get_player_id();
            std::string graphics_type = "dark";

            if (player_id != -1)
                [[maybe_unused]] graphics_type = players.at(x->get_player_id()).is_dark ? "dark"
                                                                                        : "light";

//            std::cout << "| " << x->get_graphic(graphics_type) << " ";
        }

        std::cout << "|" << std::endl
                  << lineSeparator << std::endl;
    }

    std::cout << "     ";
    for (auto &column : Piece::cols_map_) {
        std::cout << column.first << "   ";
    }
    std::cout << std::endl;
}
