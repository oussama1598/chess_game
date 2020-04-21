#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "src/modules/pieces/Pawn.h"

TEST_CASE("Pawn Movements Tests") {
    Player top_player{0, true, true, false};
    Player down_player{0, true, false, false};

    Pawn pawn(top_player.player_id);

    SECTION("Should not move horizontally") {
        CHECK(!pawn.is_valid_move(top_player, "A1", "B1"));
    }

    SECTION("Should not move backward") {
        CHECK(!pawn.is_valid_move(top_player, "A7", "A8"));
        CHECK(!pawn.is_valid_move(down_player, "A2", "A1"));
    }

    SECTION("Should move forward") {
        CHECK(pawn.is_valid_move(top_player, "A8", "A7"));
        CHECK(pawn.is_valid_move(down_player, "A1", "A2"));
    }
}
