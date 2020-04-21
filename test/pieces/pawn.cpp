#include "pawn.h"

namespace {

    TEST_CASE("Pawn Movements Tests") {
        Player top_player{0, true, true, false};
        Player down_player{0, true, false, false};
        Pawn pawn(top_player.player_id);

        std::map<std::string, std::vector<std::string>> down_player_testing_cases{
                {"A1", {"A2", "B2", "A3"}},
                {"B1", {"A2", "B2", "C2", "B3"}},
                {"H1", {"H2", "H3", "G2"}},
                {"A8", {}},
                {"H8", {}}
        };

        std::map<std::string, std::vector<std::string>> top_player_testing_cases{
                {"A8", {"A7", "A6", "B7"}},
                {"B8", {"A7", "B7", "B6", "C7"}},
                {"H8", {"H7", "H6", "G7"}},
                {"A1", {}},
                {"H1", {}}
        };

        for (auto &test_case: top_player_testing_cases)
            SECTION("Test all the possible moves from " + test_case.first +
                    " for top player") {
                std::string from{test_case.first};

                CHECK_THAT(pawn.get_possible_moves(true, from),
                           Catch::UnorderedEquals(test_case.second)
                );
            }

        SECTION("Test the is valid move") {
            CHECK(pawn.is_valid_move(top_player, "A8", "A7"));
            CHECK(pawn.is_valid_move(down_player, "A1", "A2"));
        }
    }
}
