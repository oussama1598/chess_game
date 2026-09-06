#include "game.h"

// helper to load plays
movesType load_moves(std::string &file_path) {
    movesType moves_{};
    std::ifstream play_file(file_path);
    std::string buffer;

    if (!play_file.is_open())
        throw std::runtime_error("Could not open the file.");

    while (std::getline(play_file, buffer)) {
        std::istringstream move_stream(buffer);
        std::string from;
        std::string to;
        if (!(move_stream >> from >> to)) {
            throw std::runtime_error("Invalid move in play file: " + buffer);
        }
        moves_.push_back({from, to});
    }

    play_file.close();

    return moves_;
}

namespace {
    void clear_board(Game &game) {
        for (int line = 0; line < Board::rows; ++line) {
            for (int column = 0; column < Board::cols; ++column) {
                game.get_board().add_piece(
                        Piece::get_id_from_coordinates({line, column}), new Piece());
            }
        }
    }

    TEST_CASE("Testing the game it self") {
        struct Play {
            std::string name;
            std::string path;
            Game::Result expected_result;
        };

        std::vector<Play> plays{
                {"Fool's mate", "./plays/fools_mate.txt", Game::Result::Checkmate},
                {"Scholar's mate", "./plays/scholars_mate.txt", Game::Result::Checkmate},
                {"Hohmeister vs Frank (1993)", "./plays/hohmeister_vs_frank.txt", Game::Result::Stalemate},
                {"Anderssen vs Kieseritzky (1851) - Immortal Game", "./plays/anderssen_vs_kieseritzky.txt", Game::Result::Checkmate}
        };

        for (auto &play: plays)
            SECTION("Test " + play.name + " play") {
                Game game;
                movesType moves = load_moves(play.path);


                for (auto &move: moves)
                    game.make_move(move.first, move.second);

                CHECK(game.is_game_ended());
                CHECK(game.get_result() == play.expected_result);
            }

        SECTION("Game copies own independent pieces") {
            Game original;
            Game copy{original};

            Piece *original_king = original.get_board_pieces()[0][4];
            Piece *copied_king = copy.get_board_pieces()[0][4];

            REQUIRE(original_king != copied_king);
            copied_king->did_move();
            CHECK(original_king->is_first_move());
        }

        SECTION("An unmoved pawn exposes both opening advances") {
            Game game;
            Player *white = game.get_current_player();

            REQUIRE(white != nullptr);
            CHECK_THAT(game.get_board().get_possible_moves_for(*white, "E2"),
                       Catch::Matchers::UnorderedEquals(
                               std::vector<std::string>{"E3", "E4"}));
        }

        SECTION("Stalemate is reported as a draw") {
            Game game;
            clear_board(game);
            game.get_board().add_piece("A8", new King(1));
            game.get_board().add_piece("C6", new King(0));
            game.get_board().add_piece("B6", new Queen(0));

            game.make_move("B6", "C7");

            CHECK(game.is_game_ended());
            CHECK(game.get_result() == Game::Result::Stalemate);
            CHECK(game.get_winner_id() == -1);
        }
    }
}
