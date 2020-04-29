#include "king.h"

namespace {
    TEST_CASE("King Movements Tests") {
        Player top_player{0, true, true, false};
        King king(top_player.player_id);

        king.did_move();

        std::map<std::string, std::vector<std::string>> testing_cases{
                {"F5", {"E6", "F6", "G6", "E5", "G5", "E4", "F4", "G4"}},
                {"A1", {"A2", "B2", "B1"}},
                {"A8", {"B8", "B7", "A7"}},
                {"H8", {"G8", "G7", "H7"}},
                {"H1", {"G2", "G1", "H2"}}
        };


        for (auto &test_case: testing_cases)
            SECTION("Test all the possible moves from " + test_case.first) {
                std::string from{test_case.first};

                CHECK_THAT(king.get_possible_moves(false, from),
                           Catch::UnorderedEquals(test_case.second)
                );
            }

        SECTION("Test the is valid move") {
            CHECK(king.is_valid_move(top_player, "F5", "E6"));
        }
    }
}
