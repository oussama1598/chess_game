#include "game.h"

// helper to load plays
movesType load_moves(std::string &file_path) {
    movesType moves_{};
    std::ifstream play_file(file_path);
    std::string buffer;

    if (!play_file.is_open())
        throw std::runtime_error("Could not open the file.");

    while (std::getline(play_file, buffer)) {
        std::vector<std::string> inputs;

        boost::split(inputs, buffer, boost::is_any_of(" "));

        moves_.push_back({inputs[0], inputs[1]});
    }

    play_file.close();

    return moves_;
}

namespace {
    TEST_CASE("Testing the game it self") {
        std::vector<std::pair<std::string, std::string>> plays{
                {"Fool's mate",    "./plays/fools_mate.txt"},
                {"Scholar's mate", "./plays/scholars_mate.txt"},
                {"Hohmeister vs Frank (1993)", "./plays/hohmeister_vs_frank.txt"},
                {"Anderssen vs Kieseritzky (1851) - Immortal Game", "./plays/anderssen_vs_kieseritzky.txt"}
        };

        for (auto &play: plays)
            SECTION("Test " + play.first + " play") {
                Game game;
                movesType moves = load_moves(play.second);


                for (auto &move: moves)
                    game.make_move(move.first, move.second);

                CHECK(game.is_game_ended());
            }
    }
}
