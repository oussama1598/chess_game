#pragma once

#include <random>
#include <cmath>
#include <array>
#include <chrono>
#include <ctime>
#include "game/Game.h"

typedef std::array<std::array<float, Piece::cols>, 8> eval_array;

class AI_Player {
private:
    static bool thinking_;

    static eval_array pawn_eval_player_2;
    static eval_array knight_eval_player_2;
    static eval_array bishop_eval_player_2;
    static eval_array rook_eval_player_2;
    static eval_array queen_eval_player_2;
    static eval_array king_eval_player_2;

    static std::default_random_engine generator_;

    static int get_random_index_(int size);

public:

    static inline bool is_thinking() { return thinking_; }

    static void make_a_move(Game *game, int level);

    static void random_move(Game *game);

    static void high_value_move(Game *game);

    static float evaluate_board(int player_id, Game &game);

    static float get_piece_value(int player_id, Piece *piece, int i, int j);

    static void mini_max_algorithm(Game *game, int depth);

    static float mini_max(Player *ai_player, int depth, Game *game, bool is_max, float alpha, float beta);
};
