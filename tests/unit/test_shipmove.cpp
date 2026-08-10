#include "pch.h"

#include "doctest/doctest.h"
#include "game/shipmove.h"

namespace TEST_SHIPMOVE {
    void Reset_();
}

DOCTEST_TEST_CASE("movement to assigned planet star keeps auto-colonization") {
    struct s_case {
        int16_t initial_location;
        int8_t turns_left;
        int16_t expected_location;
    };

    const s_case cases[] = {
        {0, 0, 7},
        {0, 1, 7 + SHIP_LOCATION_WORMHOLE_OFFSET},
        {SHIP_LOCATION_MOVING_OFFSET, 1, 7 + SHIP_LOCATION_MOVING_OFFSET},
    };

    for (const s_case& test_case : cases) {
        TEST_SHIPMOVE::Reset_();

        MOX::_planet[3].star_index = 7;
        MOX::_ship[0].d.ship_type = SHIP_TYPE_COLONY;
        MOX::_ship[0].d.ship_weapon[0].type = 3;
        MOX::_ship[0].d.ship_weapon[1].type = 1;
        MOX::_ship[0].location = test_case.initial_location;

        int16_t ship_list[] = {0, -1};
        s_ship_move_info move_info = {};
        move_info.turns_left = test_case.turns_left;

        SHIPMOVE::Make_Ships_Move_To_(ship_list, 7, &move_info);

        DOCTEST_CHECK(MOX::_ship[0].location == test_case.expected_location);
        DOCTEST_CHECK(MOX::_ship[0].d.ship_weapon[0].type == 3);
        DOCTEST_CHECK(MOX::_ship[0].d.ship_weapon[1].type == 1);
    }
}

DOCTEST_TEST_CASE("movement away from assigned planet clears auto-colonization") {
    TEST_SHIPMOVE::Reset_();

    MOX::_planet[3].star_index = 7;
    MOX::_ship[0].d.ship_type = SHIP_TYPE_OUTPOST;
    MOX::_ship[0].d.ship_weapon[0].type = 3;
    MOX::_ship[0].d.ship_weapon[1].type = 1;

    int16_t ship_list[] = {0, -1};
    s_ship_move_info move_info = {};

    SHIPMOVE::Make_Ships_Move_To_(ship_list, 8, &move_info);

    DOCTEST_CHECK(MOX::_ship[0].d.ship_weapon[0].type == PLANET_INDEX_NONE);
    DOCTEST_CHECK(MOX::_ship[0].d.ship_weapon[1].type == 0);
}