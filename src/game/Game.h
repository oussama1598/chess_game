#pragma once

#include "Errors.h"
#include "Board.h"

class Game {
public:
    enum class Result {
        InProgress,
        Checkmate,
        Stalemate
    };

private:
    Board game_board_;
    std::vector<Player> players_;
    bool is_game_in_check_;
    bool is_game_ended_;
    Result result_{Result::InProgress};
    int winner_id_{-1};
    Player *current_player_{};

    std::string latest_from_{};
    std::string latest_to_{};

public:
    Game();

    Game(const Game &game);

    Game &operator=(const Game &game);

    inline std::vector<Player> get_players() { return players_; }

    [[nodiscard]] inline bool is_game_in_check() const { return is_game_in_check_; }

    [[nodiscard]] inline bool is_game_ended() const { return is_game_ended_; }

    [[nodiscard]] inline Result get_result() const { return result_; }

    [[nodiscard]] inline int get_winner_id() const { return winner_id_; }

    inline Board::piecesType &get_board_pieces() { return game_board_.get_pieces(); }

    inline Player *get_current_player() { return current_player_; }

    inline Board &get_board() { return game_board_; }

    void add_player(int player_id, bool is_dark, bool is_top);

    void initialize_game();

    void switch_players();

    void make_move(const std::string &from, const std::string &to);

    std::pair<std::string, std::string> get_latest_move();
};
