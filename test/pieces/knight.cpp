#include "knight.h"

namespace {
    TEST_CASE("Knight Movements Tests") {
        Player top_player{0, true, true, false};
        Knight knight(top_player.player_id);

        std::map<std::string, std::vector<std::string>> testing_cases{
                {"D4", {"B5", "C6", "E6", "F5", "F3", "E2", "C2", "B3"}},
                {"A1", {"C2", "B3"}},
                {"A8", {"C7", "B6"}},
                {"H1", {"F2", "G3"}},
                {"H8", {"F7", "G6"}},
        };


        for (auto &test_case: testing_cases)
            SECTION("Test all the possible moves from " + test_case.first) {
                std::string from{test_case.first};

                CHECK_THAT(knight.get_possible_moves(false, from),
                           Catch::UnorderedEquals(test_case.second)
                );
            }

        SECTION("Test the is valid move") {
            CHECK(knight.is_valid_move(top_player, "E5", "F7"));
        }
    }
}
