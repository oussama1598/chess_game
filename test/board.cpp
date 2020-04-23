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

        SECTION("Testing find the king function") {
            Board board_;
            Player player{0, true, true, true};
            King *king1 = new King(0);
            King *king2 = new King(1);

            board_.add_piece("A1", king1);
            board_.add_piece("A2", king2);

            Piece::piece_coordinates king1_coord = Piece::get_piece_coordinates_from_id(
                    "A1");
            Piece::piece_coordinates king2_coord = Piece::get_piece_coordinates_from_id(
                    "A2");

            Piece::piece_coordinates found_king1_coord = board_.find_king(
                    player);

            CHECK((found_king1_coord.line == king1_coord.line &&
                   found_king1_coord.column == king1_coord.column));
            CHECK((found_king1_coord.line != king2_coord.line ||
                   found_king1_coord.column != king2_coord.column));
        }

        SECTION("Testing is king safe function") {
            Board board_;
            Player player{0, true, true, true};

            board_.add_piece("A1", new King(0));
            board_.add_piece("A7", new King(1));
            board_.add_piece("A8", new Queen(1));

            CHECK(board_.is_king_safe(player));
        }

        SECTION("Testing has valid moves function") {
            Board board_;
            Player player{0, true, true, true};

            board_.add_piece("A1", new King(0));
            board_.add_piece("A2", new Queen(1));
            board_.add_piece("B1", new Rook(1));

            CHECK(!board_.player_has_valid_move(player));

            Board board1_;
            Player player1{0, true, true, true};

            board1_.add_piece("A1", new King(0));

            CHECK(board1_.player_has_valid_move(player1));
        }
    }
}
