#include "ai_player.h"

bool AI_Player::thinking_ = false;

eval_array AI_Player::pawn_eval_player_2 = {
        {
                {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
                {5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0},
                {1.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 1.0},
                {0.5, 0.5, 1.0, 2.5, 2.5, 1.0, 0.5, 0.5},
                {0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 0.0},
                {0.5, -0.5, -1.0, 0.0, 0.0, -1.0, -0.5, 0.5},
                {0.5, 1.0, 1.0, -2.0, -2.0, 1.0, 1.0, 0.5},
                {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
        }
};

eval_array AI_Player::knight_eval_player_2 = {
        {
                {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0},
                {-4.0, -2.0, 0.0, 0.0, 0.0, 0.0, -2.0, -4.0},
                {-3.0, 0.0, 1.0, 1.5, 1.5, 1.0, 0.0, -3.0},
                {-3.0, 0.5, 1.5, 2.0, 2.0, 1.5, 0.5, -3.0},
                {-3.0, 0.0, 1.5, 2.0, 2.0, 1.5, 0.0, -3.0},
                {-3.0, 0.5, 1.0, 1.5, 1.5, 1.0, 0.5, -3.0},
                {-4.0, -2.0, 0.0, 0.5, 0.5, 0.0, -2.0, -4.0},
                {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0}
        }
};

eval_array AI_Player::bishop_eval_player_2 = {
        {
                {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0},
                {-1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0},
                {-1.0, 0.0, 0.5, 1.0, 1.0, 0.5, 0.0, -1.0},
                {-1.0, 0.5, 0.5, 1.0, 1.0, 0.5, 0.5, -1.0},
                {-1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, -1.0},
                {-1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0},
                {-1.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, -1.0},
                {-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0}
        }
};

eval_array AI_Player::rook_eval_player_2 = {
        {
                {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
                {0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5},
                {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
                {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
                {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
                {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
                {-0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5},
                {0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0}
        }
};

eval_array AI_Player::queen_eval_player_2 = {
        {
                {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0},
                {-1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0},
                {-1.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0, -1.0},
                {-0.5, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0, -0.5},
                {0.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0, -0.5},
                {-1.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.0, -1.0},
                {-1.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, -1.0},
                {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0}
        }
};

eval_array AI_Player::king_eval_player_2 = {
        {

                {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
                {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
                {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
                {-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
                {-2.0, -3.0, -3.0, -4.0, -4.0, -3.0, -3.0, -2.0},
                {-1.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.0},
                {2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0},
                {2.0, 3.0, 1.0, 0.0, 0.0, 1.0, 3.0, 2.0}
        }
};

std::default_random_engine AI_Player::generator_{std::random_device{}()};

int AI_Player::get_random_index_(int size) {
    std::uniform_int_distribution<int> distribution(0, size - 1);

    return distribution(generator_);
}

void AI_Player::make_a_move(Game *game, int level) {
    thinking_ = true;

    switch (level) {
        case 1:
            random_move(game);
            break;
        case 2:
            high_value_move(game);
            break;
        default:
            try {
                mini_max_algorithm(game, level - 1);
            } catch (std::exception &error) {
                high_value_move(game);
            }
            break;
    }

    thinking_ = false;
}

void AI_Player::random_move(Game *game) {
    Player *ai_player = game->get_current_player();
    std::vector<std::pair<std::string, std::vector<std::string >>>
            all_possible_moves = game->get_board().get_all_valid_moves_for(
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
    std::vector<std::pair<std::string, std::vector<std::string >>>
            all_possible_moves = game->get_board().get_all_valid_moves_for(
            *ai_player);

    float best_value = INT_MIN;
    std::string best_from_move;
    std::string best_to_move;

    for (auto &move: all_possible_moves)
        for (auto &to: move.second) {
            Game temp_game(game);

            temp_game.make_move(move.first, to);

            float board_value = evaluate_board(ai_player->player_id, temp_game);

            if (board_value > best_value) {
                best_from_move = move.first;
                best_to_move = to;

                best_value = board_value;
            }
        }

    game->make_move(best_from_move, best_to_move);
}

float AI_Player::evaluate_board(int player_id, Game &game) {
    float value{0};

    for (int i = 0; i < Piece::rows; ++i)
        for (int j = 0; j < Piece::cols; ++j) {
            value += get_piece_value(player_id, game.get_board_pieces()[i][j], i, j);
        }

    return value;
}

float AI_Player::get_piece_value(int player_id, Piece *piece, int i, int j) {
    float piece_value = 0;
    char symbol = piece->get_symbol();

    if (symbol == 'P') {
        eval_array eval(pawn_eval_player_2);

        if (player_id != 1)
            std::reverse(std::begin(eval), std::end(eval));

        piece_value = 10 + eval[i][j];
    } else if (symbol == 'k') {
        eval_array eval(knight_eval_player_2);

        if (player_id != 1)
            std::reverse(std::begin(eval), std::end(eval));

        piece_value = 30 + eval[i][j];
    } else if (symbol == 'B') {
        eval_array eval(bishop_eval_player_2);

        if (player_id != 1)
            std::reverse(std::begin(eval), std::end(eval));

        piece_value = 30 + eval[i][j];
    } else if (symbol == 'R') {
        eval_array eval(rook_eval_player_2);

        if (player_id != 1)
            std::reverse(std::begin(eval), std::end(eval));

        piece_value = 50 + eval[i][j];
    } else if (symbol == 'Q') {
        eval_array eval(queen_eval_player_2);

        if (player_id != 1)
            std::reverse(std::begin(eval), std::end(eval));

        piece_value = 90 + eval[i][j];
    } else if (symbol == 'K') {
        eval_array eval(king_eval_player_2);

        if (player_id != 1)
            std::reverse(std::begin(eval), std::end(eval));

        piece_value = 900 + eval[i][j];
    }


    return (piece->get_player_id() == player_id) ? piece_value : -piece_value;
}

void AI_Player::mini_max_algorithm(Game *game, int depth) {
    Player *ai_player = game->get_current_player();
    std::vector<std::pair<std::string, std::vector<std::string >>>
            all_possible_moves = game->get_board().get_all_valid_moves_for(
            *ai_player);

    float best_value = -MAXFLOAT;
    std::string best_from_move;
    std::string best_to_move;

    auto start = std::chrono::system_clock::now();

#pragma omp parallel
    {
#pragma omp single
        {
            for (auto &move: all_possible_moves) {
#pragma omp task
                {
                    for (auto &to: move.second) {
                        Game temp_game(game);

                        temp_game.make_move(move.first, to);

                        float board_value = mini_max(ai_player, depth - 1, &temp_game, false,
                                                     -MAXFLOAT,
                                                     MAXFLOAT);

                        if (board_value > best_value) {
                            best_from_move = move.first;
                            best_to_move = to;

                            best_value = board_value;
                        }
                    }
                }
            }
        }
    }

    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";

    try {
        game->make_move(best_from_move, best_to_move);
    } catch (std::exception &error) {
        mini_max_algorithm(game, depth - 1);
    }
}

float AI_Player::mini_max(Player *ai_player, int depth, Game *game, bool is_max, float alpha,
                          float beta) {
    Player *player = game->get_current_player();

    if (depth == 0) {
        return evaluate_board(ai_player->player_id, *game);
    }

    std::vector<std::pair<std::string, std::vector<std::string >>>
            all_possible_moves = game->get_board().get_all_valid_moves_for(
            *player);

    if (is_max) {
        float best_move = -MAXFLOAT;

        for (auto &move: all_possible_moves)
            for (auto &to: move.second) {
                Game temp_game{game};
                temp_game.make_move(move.first, to);

                best_move = std::max(best_move,
                                     mini_max(ai_player, depth - 1, &temp_game, !is_max, alpha,
                                              beta));

                alpha = std::max(alpha, best_move);

                if (beta <= alpha) {
                    return best_move;
                }
            }

        return best_move;
    } else {
        float best_move = MAXFLOAT;

        for (auto &move: all_possible_moves)
            for (auto &to: move.second) {
                Game temp_game{game};
                temp_game.make_move(move.first, to);

                best_move = std::min(best_move,
                                     mini_max(ai_player, depth - 1, &temp_game, !is_max, alpha,
                                              beta));

                beta = std::min(beta, best_move);

                if (beta <= alpha) {
                    return best_move;
                }

            }

        return best_move;
    }
}
