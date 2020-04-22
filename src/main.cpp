#include <iostream>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>

#include "modules/Game.h"

void clear() {
    // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
    std::cout << "\x1B[2J\x1B[H";
}

int main() {
    Game game;

    while (true) {
        game.render();

        std::string input;
        std::vector<std::string> inputs;

        std::cout << "Player: " << game.get_current_player()->player_id << " Make a move: ";
        std::getline(std::cin, input);

        boost::split(inputs, input, boost::is_any_of(" "));

        clear();

        try {
            game.make_move(inputs[0], inputs[1]);
        } catch (std::exception &error) {
            std::cout << error.what() << std::endl;
        }
    }

    return 0;
}
