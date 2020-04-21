#include "king.h"

namespace {
    TEST_CASE("King Movements Tests") {
        Player top_player{0, true, true, false};
        Player down_player{0, true, false, false};
        King king(top_player.player_id);


        SECTION("Test the is valid move") {
            CHECK(!king.is_valid_move(down_player, "A1", "A2"));
        }
    }
}
