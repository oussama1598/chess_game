#include "board.h"

namespace {
    TEST_CASE("Testing the board's functionality") {
        Board board;
        board.add_piece("E1", new Pawn(0));
        board.add_piece("A5", new Pawn(0));
        board.add_piece("C6", new Pawn(0));
        board.add_piece("D4", new Pawn(0));

        std::map<std::string, std::vector<std::string>> testing_cases{
                {"A1", {"B1", "C1", "D1", "E1", "A2", "A3", "A4", "A5", "B2", "C3", "D4"}},
                {"C1", {
                        "A1", "B1", "D1", "E1", "C2", "C3", "C4", "C5",
                        "C6", "D2", "E3", "F4", "G5", "H6", "B2", "A3"}
                }
        };


        for (auto &test_case: testing_cases)
            SECTION("Testing the empty spots function from " +
                    test_case.first) {
                std::string from{test_case.first};

                CHECK_THAT(board.get_all_empty_spots(from),
                           Catch::UnorderedEquals(test_case.second)
                );
            }
    }
}
