//#include <iostream>
//#include <bits/stdc++.h>
//#include <boost/algorithm/string.hpp>

//#include "renderers/console.h"
//#include "modules/Game.h"

#include "renderers/renderer_2D.h"

int main() {
    Renderer2D renderer("Title");

    while (renderer.is_running()) {
        renderer.render();
    }

//    Game game;
//
//    while (true) {
//        ConsoleRenderer::render(game);
//
//        std::string input;
//        std::vector<std::string> inputs;
//
//        std::cout << "Player: " << game.get_current_player()->player_id
//                  << " Make a move: ";
//        std::getline(std::cin, input);
//
//        boost::split(inputs, input, boost::is_any_of(" "));
//
//        ConsoleRenderer::clear();
//
//        try {
//            game.make_move(inputs[0], inputs[1]);
//        } catch (std::exception &error) {
//            std::cout << error.what() << std::endl;
//        }
//    }

    return 0;
}
