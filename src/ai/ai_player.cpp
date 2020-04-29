#include "ai_player.h"

std::default_random_engine AI_Player::generator_{0};

int AI_Player::get_random_index_(int size) {
    std::uniform_int_distribution<int> distribution(0, size - 1);

    return distribution(generator_);
}

void AI_Player::make_a_move(Game *game, int level) {
    switch (level) {
        case 1:
            make_a_move_level1(game);
            break;
    }
}

void AI_Player::make_a_move_level1(Game *game) {
    Player *ai_player = game->get_current_player();
    std::vector<std::pair<std::string, std::vector<std::string>>> all_possible_moves = game->get_board().get_all_valid_moves_for(
            *ai_player);


    if (!all_possible_moves.empty()) {
        int from_index = get_random_index_(all_possible_moves.size());

        std::string selected_from = all_possible_moves.at(from_index).first;
        std::vector<std::string> moves_from = all_possible_moves.at(from_index).second;

        if (!moves_from.empty()) {
            int to_index = get_random_index_(moves_from.size());

            game->make_move(selected_from, moves_from.at(to_index));
        }
    }
}
