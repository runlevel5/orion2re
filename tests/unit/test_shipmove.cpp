#include "pch.h"

#include "doctest/doctest.h"
#include "game/shipmove.h"

namespace TEST_SHIPMOVE {
    void Reset_();
    void Set_Hyperspace_Flux_(bool active);
    int16_t Movement_Speed_();
    bool Navigator_Pointer_Supplied_();
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

DOCTEST_TEST_CASE("in-flight recalculation preserves departure speed during hyperspace flux") {
    TEST_SHIPMOVE::Reset_();
    TEST_SHIPMOVE::Set_Hyperspace_Flux_(true);

    MOX::_player[0].ship_speed = 8;
    MOX::_ship[0].status = 1;
    MOX::_ship[0].location = SHIP_LOCATION_MOVING_OFFSET + 1;
    MOX::_ship[0].x = 30;
    MOX::_ship[0].travelling_speed = 4;

    int16_t ship_list[] = {0, -1};
    s_ship_move_info move_info;
    SHIPMOVE::Ships_Try_To_Move_To_(ship_list, 1, &move_info);

    DOCTEST_CHECK(move_info.moving == 1);
    DOCTEST_CHECK(move_info.hyperspace_flux == 0);
    DOCTEST_CHECK(move_info.traveling_speed == 4);
    DOCTEST_CHECK(TEST_SHIPMOVE::Movement_Speed_() == 4);
}

DOCTEST_TEST_CASE("in-flight recalculation keeps route after fuel range is lost") {
    TEST_SHIPMOVE::Reset_();

    MOX::_player[0].ship_range = 0;
    MOX::_ship[0].status = 1;
    MOX::_ship[0].location = SHIP_LOCATION_MOVING_OFFSET + 1;
    MOX::_ship[0].x = 30;
    MOX::_ship[0].travelling_speed = 4;

    int16_t ship_list[] = {0, -1};
    s_ship_move_info move_info;
    SHIPMOVE::Ships_Try_To_Move_To_(ship_list, 1, &move_info);

    DOCTEST_CHECK(move_info.moving == 1);
    DOCTEST_CHECK(move_info.traveling_speed == 4);
}

DOCTEST_TEST_CASE("movement stores the numeric navigator bonus") {
    TEST_SHIPMOVE::Reset_();

    MOX::_leaders[0].eta = 0;
    MOX::_leaders[0].xp = 5;
    MOX::_leaders[0].special_skills = LEADER_SHIP_SKILL_NAVIGATOR2;
    MOX::_ship[0].officer_index = 0;

    int16_t ship_list[] = {0, -1};
    s_ship_move_info move_info;
    SHIPMOVE::Ships_Try_To_Move_To_(ship_list, 1, &move_info);
    SHIPMOVE::Make_Ships_Move_To_(ship_list, 1, &move_info);

    DOCTEST_CHECK(move_info.has_navigator == 2);
    DOCTEST_CHECK(MOX::_ship[0].group_has_navigator == 2);
    DOCTEST_CHECK(MOX::_ship[0].travelling_speed == 6);
}

DOCTEST_TEST_CASE("jump gate speed and nebula bypass use synchronized settings") {
    TEST_SHIPMOVE::Reset_();

    SHIPMOVE::_jump_gate_speed_bonus = 5;
    SHIPMOVE::_jump_gate_pass_nebula = true;
    MOX::_star[0].has_jumpgate = 1;
    MOX::_star[1].has_jumpgate = 1;

    int16_t ship_list[] = {0, -1};
    s_ship_move_info move_info;
    SHIPMOVE::Ships_Try_To_Move_To_(ship_list, 1, &move_info);

    DOCTEST_CHECK(move_info.has_jumpgate_route == 1);
    DOCTEST_CHECK(move_info.traveling_speed == 9);
    DOCTEST_CHECK(TEST_SHIPMOVE::Movement_Speed_() == 9);
    DOCTEST_CHECK_FALSE(TEST_SHIPMOVE::Navigator_Pointer_Supplied_());

    SHIPMOVE::_jump_gate_speed_bonus = 3;
    SHIPMOVE::_jump_gate_pass_nebula = false;
}

DOCTEST_TEST_CASE("splitting an in-flight fleet recalculates the remaining ships") {
    TEST_SHIPMOVE::Reset_();

    MOX::_NUM_SHIPS = 2;
    MOX::_NUM_STARS = 3;
    MOX::_star[2].x = 180;
    MOX::_star[2].y = 0;
    MOX::_leaders[0].eta = 0;
    MOX::_leaders[0].xp = 5;
    MOX::_leaders[0].special_skills = LEADER_SHIP_SKILL_NAVIGATOR2;

    for (int16_t i = 0; i < 2; ++i) {
        MOX::_ship[i].owner = 0;
        MOX::_ship[i].status = 1;
        MOX::_ship[i].location = SHIP_LOCATION_MOVING_OFFSET + 1;
        MOX::_ship[i].x = 30;
        MOX::_ship[i].y = 0;
        MOX::_ship[i].group_has_navigator = 2;
        MOX::_ship[i].travelling_speed = 6;
        MOX::_ship[i].turns_left = 10;
        MOX::_ship[i].officer_index = -1;
    }
    MOX::_ship[0].officer_index = 0;

    int16_t ship_list[] = {0, -1};
    s_ship_move_info move_info;
    SHIPMOVE::Ships_Try_To_Move_To_(ship_list, 2, &move_info);

    DOCTEST_REQUIRE(SHIPMOVE::Apply_Player_Movement_Order_(ship_list, 2, &move_info));
    DOCTEST_CHECK(SHIPMOVE::Ship_Destination_Star_(&MOX::_ship[0]) == 2);
    DOCTEST_CHECK(MOX::_ship[0].travelling_speed == 6);
    DOCTEST_CHECK(SHIPMOVE::Ship_Destination_Star_(&MOX::_ship[1]) == 1);
    DOCTEST_CHECK(MOX::_ship[1].group_has_navigator == 0);
    DOCTEST_CHECK(MOX::_ship[1].travelling_speed == 4);
}