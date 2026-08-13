#include "pch.h"

#include "doctest/doctest.h"
#include "game/combinit.h"
#include "game/design.h"

namespace TEST_SHIELD_TABLE {
    void Reset_();
    s_combat_data* Combat_Ship_(int16_t ship_idx = 0);
    s_current_design* Design_();
    void Set_Combat_Special_(int16_t special_id);
    void Set_Design_Special_(int16_t special_id);
}

DOCTEST_TEST_CASE("shield capacity uses configured hull multipliers") {
    TEST_SHIELD_TABLE::Reset_();
    TECHDATA::_shield_hull_multiplier[HULL_CRUISER] = 23;
    TECHDATA::_shields[SHIELD_CLASS_V_SHIELD].shield_class = 5;

    DOCTEST_CHECK(
        TECHDATA::Shield_Strength_(HULL_CRUISER, SHIELD_CLASS_V_SHIELD) == 115);
}

DOCTEST_TEST_CASE("combat satellites select orbital defense shield rows") {
    TEST_SHIELD_TABLE::Reset_();
    s_combat_data* ship = TEST_SHIELD_TABLE::Combat_Ship_(1);
    ship->ship_idx = -1;
    ship->ftl_type = DRIVE_NO_DRIVE;
    ship->size_class = SHIP_SIZE_LARGE;
    ship->shield_type = SHIELD_CLASS_V_SHIELD;

    DOCTEST_CHECK(COMBINIT::Calc_Combat_Shield_Max_(1) ==
                  TECHDATA::_shield_hull_multiplier[HULL_STAR_BASE] *
                      TECHDATA::_shields[SHIELD_CLASS_V_SHIELD].shield_class);
}

DOCTEST_TEST_CASE("satellite shield rows remain stable across configured tactical sizes") {
    TEST_SHIELD_TABLE::Reset_();
    s_combat_data* ship = TEST_SHIELD_TABLE::Combat_Ship_(1);
    ship->ship_idx = -1;
    ship->ftl_type = DRIVE_NO_DRIVE;
    ship->shield_type = SHIELD_CLASS_V_SHIELD;

    COMBINIT::_satellite_size_class_plus_one = 0;
    ship->size_class = SHIP_SIZE_LARGE;
    DOCTEST_CHECK(COMBINIT::Calc_Combat_Shield_Max_(1) ==
                  TECHDATA::_shield_hull_multiplier[HULL_STAR_BASE] *
                      TECHDATA::_shields[SHIELD_CLASS_V_SHIELD].shield_class);

    COMBINIT::_satellite_size_class_plus_one = 1;
    ship->size_class = SHIP_SIZE_HUGE;
    DOCTEST_CHECK(COMBINIT::Calc_Combat_Shield_Max_(1) ==
                  TECHDATA::_shield_hull_multiplier[HULL_STAR_BASE] *
                      TECHDATA::_shields[SHIELD_CLASS_V_SHIELD].shield_class);
}

DOCTEST_TEST_CASE("combat shield capacity keeps nebula and special behavior") {
    TEST_SHIELD_TABLE::Reset_();
    s_combat_data* ship = TEST_SHIELD_TABLE::Combat_Ship_();
    ship->ship_idx = 4;
    ship->size_class = SHIP_SIZE_HUGE;
    ship->shield_type = SHIELD_CLASS_III_SHIELD;
    TECHDATA::_shield_hull_multiplier[HULL_BATTLESHIP] = 22;

    MOX::_in_nebula_flag = 1;
    DOCTEST_CHECK(COMBINIT::Calc_Combat_Shield_Max_(0) == 0);

    TEST_SHIELD_TABLE::Set_Combat_Special_(SPECIAL_HARD_SHIELDS);
    TEST_SHIELD_TABLE::Set_Combat_Special_(SPECIAL_MULTI_PHASED_SHIELDS);
    DOCTEST_CHECK(COMBINIT::Calc_Combat_Shield_Max_(0) == 99);
}

DOCTEST_TEST_CASE("design shield capacity applies Multi-Phased Shields after configured base") {
    TEST_SHIELD_TABLE::Reset_();
    s_current_design* design = TEST_SHIELD_TABLE::Design_();
    design->ship_size = SHIP_SIZE_TITAN;
    design->shield = SHIELD_CLASS_VII_SHIELD;
    TEST_SHIELD_TABLE::Set_Design_Special_(SPECIAL_MULTI_PHASED_SHIELDS);

    const int16_t base =
        (int16_t)TECHDATA::Shield_Strength_(HULL_TITAN, SHIELD_CLASS_VII_SHIELD);
    DOCTEST_CHECK(DESIGN::Shield_Strength_() == base * 3 / 2);
}

DOCTEST_TEST_CASE("design Hard Shields use configured damage reduction") {
    TEST_SHIELD_TABLE::Reset_();
    TEST_SHIELD_TABLE::Design_()->shield = SHIELD_CLASS_I_SHIELD;
    TEST_SHIELD_TABLE::Set_Design_Special_(SPECIAL_HARD_SHIELDS);
    TECHDATA::_hard_shields_damage_reduction = 8;

    DOCTEST_CHECK(DESIGN::Current_Shield_Damage_Points_Blocked_() == 9);
}