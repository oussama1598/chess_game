#include "queen.h"

namespace {
    TEST_CASE("Queen Movements Tests") {
        Player top_player{0, true, true, false};
        Queen queen(top_player.player_id);

        std::map<std::string, std::vector<std::string>> testing_cases{
                {"A1", {"A2", "A3", "A4", "A5", "A6", "A7", "A8", "B1",
                               "C1", "D1", "E1", "F1", "G1", "H1", "B2", "C3", "D4",
                               "E5", "F6", "G7", "H8"}},
                {"H8", {"H1", "H2", "H3", "H4", "H5", "H6", "H7", "A8",
                               "B8", "C8", "D8", "E8", "F8", "G8", "A1", "B2", "C3",
                               "D4", "E5", "F6", "G7"}},
        };


        for (auto &test_case: testing_cases)
            SECTION("Test all the possible moves from " + test_case.first) {
                std::string from{test_case.first};

                CHECK_THAT(queen.get_possible_moves(false, from),
                           Catch::UnorderedEquals(test_case.second)
                );
            }

        SECTION("Test the is valid move") {
            CHECK(queen.is_valid_move(top_player, "E1", "F1"));
        }
    }
}
