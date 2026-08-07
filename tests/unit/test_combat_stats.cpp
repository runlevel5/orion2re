#include "pch.h"

#include "doctest/doctest.h"
#include "game/combat1.h"
#include "game/combinit.h"

DOCTEST_TEST_CASE("interactive speed loss preserves movement already spent") {
    int16_t reserve = 0;

    int16_t movement = COMBAT1::Adjust_Movement_After_Speed_Change_(
        10, 4, 10, 7, &reserve);

    DOCTEST_CHECK(movement == 1);
    DOCTEST_CHECK(reserve == 6);

    movement = COMBAT1::Adjust_Movement_After_Speed_Change_(
        4, 10, 10, movement, &reserve);

    DOCTEST_CHECK(movement == 7);
    DOCTEST_CHECK(reserve == 0);
}

DOCTEST_TEST_CASE("interactive immobilization never makes movement negative") {
    int16_t reserve = 0;

    int16_t movement = COMBAT1::Adjust_Movement_After_Speed_Change_(
        10, 0, 10, 3, &reserve);

    DOCTEST_CHECK(movement == 0);
    DOCTEST_CHECK(reserve == 3);

    movement = COMBAT1::Adjust_Movement_After_Speed_Change_(
        0, 10, 10, movement, &reserve);

    DOCTEST_CHECK(movement == 3);
    DOCTEST_CHECK(reserve == 0);
}

DOCTEST_TEST_CASE("destroyed cloak devices update combat cloak state") {
    int8_t cloak_state = 1;
    COMBAT1::Handle_Destroyed_Cloak_(&cloak_state, 0, false, false, false);
    DOCTEST_CHECK(cloak_state == 2);

    cloak_state = 4;
    COMBAT1::Handle_Destroyed_Cloak_(&cloak_state, 10, true, true, false);
    DOCTEST_CHECK(cloak_state == 1);

    cloak_state = 6;
    COMBAT1::Handle_Destroyed_Cloak_(&cloak_state, 10, true, true, false);
    DOCTEST_CHECK(cloak_state == 2);

    cloak_state = 7;
    COMBAT1::Handle_Destroyed_Cloak_(&cloak_state, 10, true, true, false);
    DOCTEST_CHECK(cloak_state == 3);

    cloak_state = 4;
    COMBAT1::Handle_Destroyed_Cloak_(&cloak_state, 10, true, false, false);
    DOCTEST_CHECK(cloak_state == 6);
}

DOCTEST_TEST_CASE("combat damage storage uses the percentage of lost hit points") {
    DOCTEST_CHECK(COMBINIT::Damage_Percent_From_Hits_(14, 10) == 28);
    DOCTEST_CHECK(COMBINIT::Damage_Percent_From_Hits_(9, 6) == 33);
    DOCTEST_CHECK(COMBINIT::Damage_Percent_From_Hits_(20, 13) == 35);
    DOCTEST_CHECK(COMBINIT::Damage_Percent_From_Hits_(0, 0) == 0);
}

DOCTEST_TEST_CASE("combat damage storage reconstructs surviving fighter units") {
    DOCTEST_CHECK(COMBINIT::Surviving_Weapon_Count_From_Hits_(7, 3) == 3);
    DOCTEST_CHECK(COMBINIT::Surviving_Weapon_Count_From_Hits_(6, 3) == 2);
    DOCTEST_CHECK(COMBINIT::Surviving_Weapon_Count_From_Hits_(0, 3) == 0);
    DOCTEST_CHECK(COMBINIT::Surviving_Weapon_Count_From_Hits_(7, 0) == 0);
}