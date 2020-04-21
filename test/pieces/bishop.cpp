#include "bishop.h"

namespace {
    TEST_CASE("Bishop Movements Tests") {
        Player top_player{0, true, true, false};
        Bishop bishop(top_player.player_id);

        std::map<std::string, std::vector<std::string>> testing_cases{
                {"A1", {"B2", "C3", "D4", "E5", "F6", "G7", "H8"}},
                {"H8", {"A1", "B2", "C3", "D4", "E5", "F6", "G7"}},
                {"H1", {"A8", "B7", "C6", "D5", "E4", "F3", "G2"}},
                {"A8", {"B7", "C6", "D5", "E4", "F3", "G2", "H1"}},
                {"D5", {"A2", "B3", "C4", "E6", "F7", "G8", "A8", "B7", "C6", "E4", "F3", "G2", "H1"}},
                {"H5", {"E8", "F7", "G6", "G4", "F3", "E2", "D1"}},
                {"A3", {"B4", "C5", "D6", "E7", "F8", "B2", "C1"}}
        };


        for (auto &test_case: testing_cases)
            SECTION("Test all the possible moves from " + test_case.first) {
                std::string from{test_case.first};

                CHECK_THAT(bishop.get_possible_moves(false, from),
                           Catch::UnorderedEquals(test_case.second)
                );
            }

//        SECTION("Test the is valid move") {
//            CHECK(bishop.is_valid_move(top_player, "A1", "A2"));
//        }
    }
}
