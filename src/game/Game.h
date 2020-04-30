#pragma once

#include <variant>
#include "Errors.h"
#include "Board.h"

class Game {
private:
    Board game_board_;
    std::vector<Player> players_;
    bool is_game_in_check_;
    bool is_game_ended_;
    Player *current_player_{};

public:
    Game();

    explicit Game(Game *game);

    inline std::vector<Player> get_players() { return players_; }

    [[nodiscard]] inline bool is_game_in_check() const { return is_game_in_check_; }

    [[nodiscard]] inline bool is_game_ended() const { return is_game_ended_; }

    inline Board::piecesType &get_board_pieces() { return game_board_.get_pieces(); }

    inline Player *get_current_player() { return current_player_; }

    inline Board &get_board() { return game_board_; }

    void add_player(int player_id, bool is_dark, bool is_top);

    void initialize_game();

    void switch_players();

    void make_move(const std::string &from, const std::string &to);
};
