#include "pch.h"

#include "doctest/doctest.h"
#include "game/cmbtfir2.h"
#include "game/combat1.h"
#include "game/random.h"

namespace TEST_STASIS_AREA_EFFECTS {
    void Reset_();
    void Set_Special_(int16_t ship_idx, int16_t special_id);
    void Set_In_Stasis_(int16_t ship_idx, bool in_stasis);
}

DOCTEST_TEST_CASE("stasis disables wide area jammer") {
    TEST_STASIS_AREA_EFFECTS::Reset_();
    MOX::_combat_data[0].owner = 0;
    TEST_STASIS_AREA_EFFECTS::Set_Special_(0, SPECIAL_WIDE_AREA_JAMMER);

    int16_t base_bonus = 0;
    DOCTEST_CHECK(CMBTFIR2::Fleet_Defensive_Bonus_(0, 1, &base_bonus) == 70);

    TEST_STASIS_AREA_EFFECTS::Set_In_Stasis_(0, true);
    base_bonus = 0;
    DOCTEST_CHECK(CMBTFIR2::Fleet_Defensive_Bonus_(0, 1, &base_bonus) == 0);
}

DOCTEST_TEST_CASE("stasis disables satellite attack bonus") {
    TEST_STASIS_AREA_EFFECTS::Reset_();
    s_combat_data& station = MOX::_combat_data[1];
    station.owner = 0;
    station.ship_idx = -1;
    station.ftl_type = DRIVE_NO_DRIVE;
    station.size_class = SHIP_SIZE_HUGE;

    DOCTEST_CHECK(CMBTFIR2::Fleet_Offensive_Bonus_(0) == 10);

    TEST_STASIS_AREA_EFFECTS::Set_In_Stasis_(1, true);
    DOCTEST_CHECK(CMBTFIR2::Fleet_Offensive_Bonus_(0) == 0);
}

DOCTEST_TEST_CASE("Star Base fleet attack bonus is configurable and disabled by stasis") {
    TEST_STASIS_AREA_EFFECTS::Reset_();
    s_combat_data& station = MOX::_combat_data[1];
    station.owner = 0;
    station.ship_idx = -1;
    station.ftl_type = DRIVE_NO_DRIVE;
    station.size_class = SHIP_SIZE_LARGE;

    DOCTEST_CHECK(CMBTFIR2::Fleet_Offensive_Bonus_(0) == 0);

    CMBTFIR2::_starbase_fleet_beam_offense = 17;
    DOCTEST_CHECK(CMBTFIR2::Fleet_Offensive_Bonus_(0) == 17);

    TEST_STASIS_AREA_EFFECTS::Set_In_Stasis_(1, true);
    DOCTEST_CHECK(CMBTFIR2::Fleet_Offensive_Bonus_(0) == 0);
}

DOCTEST_TEST_CASE("ordinary no-drive ships do not receive satellite attack bonuses") {
    TEST_STASIS_AREA_EFFECTS::Reset_();
    s_combat_data& ship = MOX::_combat_data[1];
    ship.owner = 0;
    ship.ship_idx = 4;
    ship.ftl_type = DRIVE_NO_DRIVE;
    ship.size_class = SHIP_SIZE_HUGE;

    DOCTEST_CHECK(CMBTFIR2::Fleet_Offensive_Bonus_(0) == 0);
}

DOCTEST_TEST_CASE("satellite attack bonuses remain tied to station tier") {
    TEST_STASIS_AREA_EFFECTS::Reset_();
    s_combat_data& station = MOX::_combat_data[1];
    station.owner = 0;
    station.ship_idx = -1;
    station.ftl_type = DRIVE_NO_DRIVE;
    station.size_class = SHIP_SIZE_TITAN;
    COMBINIT::_satellite_size_class_plus_one = 1;

    DOCTEST_CHECK(CMBTFIR2::Fleet_Offensive_Bonus_(0) == 10);

    COMBINIT::_satellite_size_class_plus_one = 0;
}

DOCTEST_TEST_CASE("stasis disables scout lab") {
    TEST_STASIS_AREA_EFFECTS::Reset_();
    MOX::_combat_attacker_player = 9;
    MOX::_combat_data[0].owner = 0;
    MOX::_combat_data[0].ftl_type = 1;
    TEST_STASIS_AREA_EFFECTS::Set_Special_(0, SPECIAL_SCOUT_LAB);

    DOCTEST_CHECK(CMBTFIR2::Fleet_Offensive_Bonus_(0) == 20);

    TEST_STASIS_AREA_EFFECTS::Set_In_Stasis_(0, true);
    DOCTEST_CHECK(CMBTFIR2::Fleet_Offensive_Bonus_(0) == 0);
}

DOCTEST_TEST_CASE("generated Antaran ships retain tactical movement") {
    TEST_STASIS_AREA_EFFECTS::Reset_();
    s_combat_data& ship = MOX::_combat_data[0];
    ship.owner = MAX_PLAYERS;
    ship.previous_owner = MAX_PLAYERS;
    ship.ship_idx = -1;
    ship.ftl_type = DRIVE_HYPER_DRIVE;
    ship.combat_speed_base = 12;
    ship.drive_max_hits = 10;
    ship.drive_current_hits = 10;

    DOCTEST_CHECK(COMBAT1::Calc_Current_Speed_(0) == 12);

    ship.ftl_type = DRIVE_NO_DRIVE;
    DOCTEST_CHECK(COMBAT1::Calc_Current_Speed_(0) == 0);
}

DOCTEST_TEST_CASE("stasis disables warp dissipator") {
    TEST_STASIS_AREA_EFFECTS::Reset_();
    MOX::_combat_data[1].owner = 1;
    TEST_STASIS_AREA_EFFECTS::Set_Special_(1, SPECIAL_WARP_DISSIPATOR);

    DOCTEST_CHECK(COMBAT1::Enemy_Dissipator_Exists_(0) == 1);

    TEST_STASIS_AREA_EFFECTS::Set_In_Stasis_(1, true);
    DOCTEST_CHECK(COMBAT1::Enemy_Dissipator_Exists_(0) == 0);
}

DOCTEST_TEST_CASE("continuous BHG keeps one shot committed to an active target") {
    TEST_STASIS_AREA_EFFECTS::Reset_();
    MOX::_combat_data[0].weapons[0].weapon_id = WEAPON_BLACK_HOLE_GENERATOR;
    MOX::_combat_data[0].weapons[0].count = 2;
    MOX::_combat_data[1].black_hole_source_idx = 0;

    COMBAT1::Init_Ship_For_Start_Of_Turn_(0);
    DOCTEST_CHECK(MOX::_combat_data[0].weapons[0].shots_left == 2);

    COMBAT1::_continuous_bhg = 1;
    COMBAT1::Init_Ship_For_Start_Of_Turn_(0);
    DOCTEST_CHECK(MOX::_combat_data[0].weapons[0].shots_left == 1);
}

DOCTEST_TEST_CASE("artemis restores the network turn random seed before rolling") {
    TEST_STASIS_AREA_EFFECTS::Reset_();
    MOX::_game_type = 3;
    MOX::_qcombat_data[1].size = 0;
    MOX::_qcombat_data[1].status = 1;
    CMBTFIR2::_artemis_net_chance[0] = 100;

    constexpr uint32_t synchronized_seed = 0x13572468u;
    game_random::Set_Random_Seed_(synchronized_seed);
    CMBTFIR2::Save_Artemis_Random_Seed_();

    game_random::Set_Random_Seed_(synchronized_seed);
    game_random::Random_(100);
    game_random::Random_(201);
    const uint32_t expected_seed = game_random::Get_Random_Seed_();

    game_random::Set_Random_Seed_(synchronized_seed);
    game_random::Random_(5);
    game_random::Random_(5);

    CMBTFIR2::Apply_Combat_Artemis_Net_(1);

    DOCTEST_CHECK(game_random::Get_Random_Seed_() == expected_seed);

    CMBTFIR2::_artemis_net_chance[0] = 20;
}