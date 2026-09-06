#include "pawn.h"

namespace {

    TEST_CASE("Pawn Movements Tests") {
        Player top_player{0, true, true, false};
        Player down_player{0, true, false, false};
        Pawn pawn(top_player.player_id);

        std::map<std::string, std::vector<std::string>> down_player_testing_cases{
                {"A2", {"A3", "B3", "A4"}},
                {"B2", {"A3", "B3", "C3", "B4"}},
                {"H2", {"H3", "H4", "G3"}},
                {"A8", {}},
                {"H8", {}}
        };

        std::map<std::string, std::vector<std::string>> top_player_testing_cases{
                {"A7", {"A6", "A5", "B6"}},
                {"B7", {"A6", "B6", "B5", "C6"}},
                {"H7", {"H6", "H5", "G6"}},
                {"A1", {}},
                {"H1", {}}
        };

        for (auto &test_case: top_player_testing_cases)
            SECTION("Test all the possible moves from " + test_case.first +
                    " for top player") {
                std::string from{test_case.first};

                CHECK_THAT(pawn.get_possible_moves(true, from),
                           Catch::Matchers::UnorderedEquals(test_case.second)
                );
            }

        SECTION("Test the is valid move") {
            CHECK(pawn.is_valid_move(top_player, "A7", "A6"));
            CHECK(pawn.is_valid_move(down_player, "A2", "A3"));
        }

        SECTION("A pawn cannot move two squares after its first move") {
            Pawn moved_pawn(down_player.player_id);
            moved_pawn.did_move();

            CHECK_THAT(moved_pawn.get_possible_moves(false, "A2"),
                       Catch::Matchers::UnorderedEquals(std::vector<std::string>{"A3", "B3"}));
        }
    }
}
