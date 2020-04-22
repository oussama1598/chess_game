#include <iostream>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>

#include "renderers/Console.h"
#include "modules/Game.h"

int main() {
    Game game;

    while (true) {
        ConsoleRenderer::render(game);

        std::string input;
        std::vector<std::string> inputs;

        std::cout << "Player: " << game.get_current_player()->player_id
                  << " Make a move: ";
        std::getline(std::cin, input);

        boost::split(inputs, input, boost::is_any_of(" "));

        ConsoleRenderer::clear();

        try {
            game.make_move(inputs[0], inputs[1]);
        } catch (std::exception &error) {
            std::cout << error.what() << std::endl;
        }
    }

    return 0;
}
