#include "rook.h"

namespace {
    TEST_CASE("Rook Movements Tests") {
        Player top_player{0, true, true, false};
        Rook rook(top_player.player_id);

        std::map<std::string, std::vector<std::string>> testing_cases{
                {"D5", {"D1", "D2", "D3", "D4", "D6", "D7", "D8", "A5", "B5", "C5", "E5", "F5", "G5", "H5"}},
                {"A1", {"A2", "A3", "A4", "A5", "A6", "A7", "A8", "B1", "C1", "D1", "E1", "F1", "G1", "H1"}},
                {"A8", {"A1", "A2", "A3", "A4", "A5", "A6", "A7", "B8", "C8", "D8", "E8", "F8", "G8", "H8"}},
                {"H1", {"H2", "H3", "H4", "H5", "H6", "H7", "H8", "A1", "B1", "C1", "D1", "E1", "F1", "G1"}},
                {"H8", {"H1", "H2", "H3", "H4", "H5", "H6", "H7", "A8", "B8", "C8", "D8", "E8", "F8", "G8"}}
        };


        for (auto &test_case: testing_cases)
            SECTION("Test all the possible moves from " + test_case.first) {
                std::string from{test_case.first};

                CHECK_THAT(rook.get_possible_moves(false, from),
                           Catch::UnorderedEquals(test_case.second)
                );
            }

        SECTION("Test the is valid move") {
            CHECK(rook.is_valid_move(top_player, "D2", "D3"));
        }
    }
}
