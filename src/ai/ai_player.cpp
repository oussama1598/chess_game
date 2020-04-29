#include "ai_player.h"

// TODO: random seed
std::default_random_engine AI_Player::generator_{0};

int AI_Player::get_random_index_(int size) {
    std::uniform_int_distribution<int> distribution(0, size - 1);

    return distribution(generator_);
}

void AI_Player::make_a_move(Game *game, int level) {
    switch (level) {
        case 1:
            random_move(game);
            break;
        case 2:
            high_value_move(game);
            break;
    }
}

void AI_Player::random_move(Game *game) {
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

void AI_Player::high_value_move(Game *game) {
    Player *ai_player = game->get_current_player();
    std::vector < std::pair < std::string, std::vector < std::string >> >
                                           all_possible_moves = game->get_board().get_all_valid_moves_for(
            *ai_player);

    int best_value = INT_MIN;
    std::string best_from_move;
    std::string best_to_move;

    for (auto &move: all_possible_moves)
        for (auto &to: move.second) {
            Game temp_game(game);

            temp_game.make_move(move.first, to);

            int board_value = evaluate_board(ai_player->player_id, temp_game);

            if (board_value > best_value) {
                best_from_move = move.first;
                best_to_move = to;

                best_value = board_value;
            }
        }

    game->make_move(best_from_move, best_to_move);
}

int AI_Player::evaluate_board(int player_id, Game &game) {
    int value{0};

    for (int i = 0; i < Piece::rows; ++i)
        for (int j = 0; j < Piece::cols; ++j) {
            value += get_piece_value(player_id, game.get_board_pieces()[i][j]);
        }

    return value;
}

int AI_Player::get_piece_value(int player_id, Piece *piece) {
    int piece_value = 0;

    switch (piece->get_symbol()) {
        case 'P':
            piece_value = 10;
            break;
        case 'k':
            piece_value = 30;
            break;
        case 'B':
            piece_value = 30;
            break;
        case 'R':
            piece_value = 50;
            break;
        case 'Q':
            piece_value = 90;
            break;
        case 'K':
            piece_value = 900;
            break;
        default:
            piece_value = 0;
            break;
    }

    return (piece->get_player_id() == player_id) ? piece_value : -piece_value;
}
