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
                           Catch::Matchers::UnorderedEquals(test_case.second)
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
            board_.add_piece("A3", new Queen(1));
            board_.add_piece("G1", new Rook(1));

            CHECK(!board_.player_has_valid_move(player));

            Board board1_;
            Player player1{0, true, true, true};

            board1_.add_piece("A1", new King(0));

            CHECK(board1_.player_has_valid_move(player1));
        }

        SECTION("A normal king move is not treated as castling") {
            Board board_;
            Player player{0, true, false, false};

            board_.add_piece("E1", new King(0));
            board_.add_piece("A8", new King(1));

            Piece *king = board_.get_piece_at(0, 4);
            Piece *destination = board_.get_piece_at(0, 5);

            REQUIRE(board_.can_make_move(king, player, "E1", "F1"));
            REQUIRE(board_.perform_move(player, {0, 4}, king, {0, 5}, destination));
            CHECK(board_.get_piece_at(0, 5) == king);
        }

        SECTION("Castling cannot cross an attacked square") {
            Board board_;
            Player player{0, true, false, false};

            board_.add_piece("E1", new King(0));
            board_.add_piece("H1", new Rook(0));
            board_.add_piece("A8", new King(1));
            board_.add_piece("F8", new Rook(1));

            CHECK(!board_.can_castle(player, board_.get_piece_at(0, 4), "E1", "G1"));
        }

        SECTION("Castling requires a friendly rook") {
            Board board_;
            Player player{0, true, false, false};

            board_.add_piece("E1", new King(0));
            board_.add_piece("H1", new Rook(1));
            board_.add_piece("A8", new King(1));

            CHECK(!board_.can_castle(player, board_.get_piece_at(0, 4), "E1", "G1"));
        }

        SECTION("A king cannot be captured") {
            Board board_;
            Player player{0, true, false, false};

            board_.add_piece("A1", new King(0));
            board_.add_piece("E7", new Queen(0));
            board_.add_piece("E8", new King(1));

            CHECK(!board_.can_make_move(board_.get_piece_at(6, 4), player, "E7", "E8"));
        }

        SECTION("A board without a king is unsafe") {
            Board board_;
            Player player{0, true, false, false};

            CHECK(!board_.is_king_safe(player));
        }

        SECTION("Coordinates must contain exactly one valid file and rank") {
            const auto valid = Piece::get_piece_coordinates_from_id("A1");
            const auto too_long = Piece::get_piece_coordinates_from_id("A10");
            const auto too_short = Piece::get_piece_coordinates_from_id("A");
            const auto bad_rank = Piece::get_piece_coordinates_from_id("Ax");

            CHECK(valid.line == 0);
            CHECK(valid.column == 0);
            CHECK(too_long.line == -1);
            CHECK(too_short.line == -1);
            CHECK(bad_rank.line == -1);
        }

        SECTION("Testing get possible moves for function") {
            Board board_;
            Player player{0, true, false, true};

            board_.add_piece("A1", new King(0));
            board_.add_piece("A2", new Queen(0));
            board_.add_piece("B1", new Rook(1));

            CHECK_THAT(board_.get_possible_moves_for(player, "A1"),
                       Catch::Matchers::UnorderedEquals(std::vector<std::string>{"B1"}));
        }

        SECTION("Test castling") {
            Board board_;
            Piece *piece_bottom = new King(0);
            Player player{0, true, false, true};

            Piece *piece_top = new King(1);
            Player player_top{1, true, true, true};

            board_.add_piece("E1", piece_bottom);
            board_.add_piece("H1", new Rook(0));
            board_.add_piece("B1", new Rook(0));

            CHECK(board_.can_castle(player, piece_bottom, "E1", "G1"));
            CHECK(!board_.can_castle(player, piece_bottom, "E1", "C1"));

            std::vector<Piece::piece_coordinates> to_castle_coords = board_.where_to_castle(player,
                                                                                            "G1");

            std::string rook_id = Piece::get_id_from_coordinates(to_castle_coords[0]);
            std::string empty_spot = Piece::get_id_from_coordinates(to_castle_coords[1]);

            CHECK_THAT((std::vector<std::string>{rook_id, empty_spot}),
                       Catch::Matchers::UnorderedEquals(
                               std::vector<std::string>{"H1",
                                                        "F1"}
                       ));

            board_.add_piece("E8", piece_top);
            board_.add_piece("H8", new Rook(1));
            board_.add_piece("B8", new Rook(1));

            CHECK(board_.can_castle(player_top, piece_top, "E8", "G8"));
            CHECK(!board_.can_castle(player_top, piece_top, "E8", "C8"));

        }
    }
}
