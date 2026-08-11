#include "pch.h"

#include "doctest/doctest.h"
#include "game/boarding.h"
#include "game/cmbtfir2.h"
#include "game/combat1.h"
#include "game/techdata.h"

namespace TEST_BOARDING {
    void Reset_();
    int16_t Killed_Officer_();
    int16_t Pooled_Officer_();
    void Set_Combat_Shield_Max_(int16_t value);
    void Prepare_Long_Raid_();
    const char* Last_Raid_Message_();
    int16_t Boarding_Attacker_Bonus_();
    int16_t Boarding_Defender_Bonus_();
}

DOCTEST_TEST_CASE("boarding rejects targets in stasis") {
    TEST_BOARDING::Reset_();
    MOX::_combat_data[1].stasis_source_idx = 0;

    DOCTEST_CHECK(
        BOARDING::Filter_Boarding_(0, 1) ==
        BOARDING::e_boarding_filter::target_in_stasis);
}

DOCTEST_TEST_CASE("classic boarding permits black-hole affected ships") {
    TEST_BOARDING::Reset_();
    MOX::_combat_data[0].black_hole_source_idx = 1;
    MOX::_combat_data[1].black_hole_source_idx = 0;

    DOCTEST_CHECK(
        BOARDING::Filter_Boarding_(0, 1) ==
        BOARDING::e_boarding_filter::allowed);
}

DOCTEST_TEST_CASE("disabled black-hole boarding filters both participants") {
    TEST_BOARDING::Reset_();
    BOARDING::_board_in_black_hole = 0;
    MOX::_combat_data[1].black_hole_source_idx = 0;

    DOCTEST_CHECK(
        BOARDING::Filter_Boarding_(0, 1) ==
        BOARDING::e_boarding_filter::target_in_black_hole);

    MOX::_combat_data[1].black_hole_source_idx = UINT8_MAX;
    MOX::_combat_data[0].black_hole_source_idx = 1;
    DOCTEST_CHECK(
        BOARDING::Filter_Boarding_(0, 1) ==
        BOARDING::e_boarding_filter::attacker_in_black_hole);
}

DOCTEST_TEST_CASE("classic captures retain marines above hull capacity") {
    TEST_BOARDING::Reset_();
    MOX::_combat_data[1].ship_idx = 1;
    MOX::_combat_data[1].size_class = HULL_CRUISER;
    TECHDATA::_hull_data[HULL_CRUISER].marines = 12;

    DOCTEST_CHECK(BOARDING::Captured_Marine_Count_(1, 20) == 20);
}

DOCTEST_TEST_CASE("enabled capture limits account for working troop pods") {
    TEST_BOARDING::Reset_();
    BOARDING::_kill_excess_marines_after_capture = 1;
    MOX::_combat_data[1].ship_idx = 1;
    MOX::_combat_data[1].ftl_type = DRIVE_HYPER_DRIVE;
    MOX::_combat_data[1].size_class = HULL_CRUISER;
    TECHDATA::_hull_data[HULL_CRUISER].marines = 12;

    DOCTEST_CHECK(BOARDING::Captured_Marine_Count_(1, 30) == 12);

    struct_::Set_Bit_Field_(
        MOX::_combat_data[1].special_device_flags, SPECIAL_TROOP_PODS);
    DOCTEST_CHECK(BOARDING::Captured_Marine_Count_(1, 30) == 24);

    struct_::Set_Bit_Field_(
        MOX::_combat_data[1].special_device_damage_flags, SPECIAL_TROOP_PODS);
    DOCTEST_CHECK(BOARDING::Captured_Marine_Count_(1, 30) == 12);
}

DOCTEST_TEST_CASE("captured bases use orbital hull marine capacity") {
    TEST_BOARDING::Reset_();
    BOARDING::_kill_excess_marines_after_capture = 1;
    MOX::_combat_data[1].ship_idx = -1;
    MOX::_combat_data[1].ftl_type = DRIVE_NO_DRIVE;
    MOX::_combat_data[1].size_class = HULL_CRUISER;
    TECHDATA::_hull_data[HULL_STAR_BASE].marines = 20;

    DOCTEST_CHECK(BOARDING::Captured_Marine_Count_(1, 30) == 20);
}

DOCTEST_TEST_CASE("generated Antaran ships use normal hull marine capacity") {
    TEST_BOARDING::Reset_();
    BOARDING::_kill_excess_marines_after_capture = 1;
    MOX::_combat_data[1].ship_idx = -1;
    MOX::_combat_data[1].ftl_type = DRIVE_HYPER_DRIVE;
    MOX::_combat_data[1].size_class = HULL_CRUISER;
    TECHDATA::_hull_data[HULL_CRUISER].marines = 12;
    TECHDATA::_hull_data[HULL_STAR_BASE].marines = 20;

    DOCTEST_CHECK(BOARDING::Captured_Marine_Count_(1, 30) == 12);
}

DOCTEST_TEST_CASE("capture includes both ships' crew experience bonuses") {
    TEST_BOARDING::Reset_();
    MOX::_combat_data[0].owner = 0;
    MOX::_combat_data[0].crew_quality = CREW_QUALITY_VETERAN;
    MOX::_combat_data[1].owner = 1;
    MOX::_combat_data[1].crew_quality = CREW_QUALITY_ELITE;
    MOX::_crew_data[CREW_QUALITY_VETERAN].ground = 10;
    MOX::_crew_data[CREW_QUALITY_ELITE].ground = 15;

    CMBTFIR2::Resolve_Capture_(0, 1, 1, 0, 0);

    DOCTEST_CHECK(TEST_BOARDING::Boarding_Attacker_Bonus_() == 10);
    DOCTEST_CHECK(TEST_BOARDING::Boarding_Defender_Bonus_() == 15);
}

DOCTEST_TEST_CASE("classic telepathic recapture retains captured state") {
    TEST_BOARDING::Reset_();
    MOX::_combat_data[1].owner = 1;
    MOX::_combat_data[1].is_captured = 1;
    MOX::_player[0].traits[TRAIT_TELEPATHIC] = 1;

    CMBTFIR2::Capture_Ship_(1, 0, 0);

    DOCTEST_CHECK(MOX::_combat_data[1].owner == 0);
    DOCTEST_CHECK(MOX::_combat_data[1].is_captured == 1);
}

DOCTEST_TEST_CASE("enabled telepathic recapture clears captured state") {
    TEST_BOARDING::Reset_();
    BOARDING::_telepathic_multiple_capture = 1;
    MOX::_combat_data[1].owner = 1;
    MOX::_combat_data[1].is_captured = 1;
    MOX::_player[0].traits[TRAIT_TELEPATHIC] = 1;

    CMBTFIR2::Capture_Ship_(1, 0, 0);

    DOCTEST_CHECK(MOX::_combat_data[1].owner == 0);
    DOCTEST_CHECK(MOX::_combat_data[1].is_captured == 0);
}

DOCTEST_TEST_CASE("enabled non-telepathic capture sets captured state") {
    TEST_BOARDING::Reset_();
    BOARDING::_telepathic_multiple_capture = 1;
    MOX::_combat_data[1].owner = 1;

    CMBTFIR2::Capture_Ship_(1, 0, 0);

    DOCTEST_CHECK(MOX::_combat_data[1].is_captured == 1);
}

DOCTEST_TEST_CASE("capture preserves Antaran origin") {
    TEST_BOARDING::Reset_();
    MOX::_combat_data[1].owner = 1;
    MOX::_combat_data[1].previous_owner = MAX_PLAYERS;

    CMBTFIR2::Capture_Ship_(1, 0, 0);

    DOCTEST_CHECK(MOX::_combat_data[1].owner == 0);
    DOCTEST_CHECK(MOX::_combat_data[1].previous_owner == MAX_PLAYERS);
}

DOCTEST_TEST_CASE("captured ships retain an existing turn-order entry") {
    int16_t turn_order[4] = { 3, 1, -10, -1 };

    COMBAT1::Restore_Captured_Ship_To_Turn_Order_(
        turn_order, 4, 1);

    DOCTEST_CHECK(turn_order[0] == 3);
    DOCTEST_CHECK(turn_order[1] == 1);
    DOCTEST_CHECK(turn_order[2] == -10);
}

DOCTEST_TEST_CASE("captured ships reuse a vacant turn-order entry") {
    int16_t turn_order[4] = { 3, -1, -10, -1 };

    COMBAT1::Restore_Captured_Ship_To_Turn_Order_(
        turn_order, 4, 1);

    DOCTEST_CHECK(turn_order[0] == 3);
    DOCTEST_CHECK(turn_order[1] == 1);
    DOCTEST_CHECK(turn_order[2] == -10);
}

DOCTEST_TEST_CASE("captured ships append when turn order has capacity") {
    int16_t turn_order[4] = { 3, -10, -1, -1 };

    COMBAT1::Restore_Captured_Ship_To_Turn_Order_(
        turn_order, 4, 1);

    DOCTEST_CHECK(turn_order[0] == 3);
    DOCTEST_CHECK(turn_order[1] == 1);
    DOCTEST_CHECK(turn_order[2] == -10);
}

DOCTEST_TEST_CASE("captured ships cannot overflow a full turn order") {
    struct {
        int16_t turn_order[210];
        int16_t canary;
    } state;

    for (int16_t i = 0; i < 209; ++i) {
        state.turn_order[i] = 3;
    }
    state.turn_order[209] = -10;
    state.canary = 0x1234;

    COMBAT1::Restore_Captured_Ship_To_Turn_Order_(
        state.turn_order, 210, 1);

    DOCTEST_CHECK(state.turn_order[209] == -10);
    DOCTEST_CHECK(state.canary == 0x1234);
}

DOCTEST_TEST_CASE("capture returns a strategic leader to the officer pool") {
    TEST_BOARDING::Reset_();
    MOX::_combat_data[1].owner = 0;
    MOX::_combat_data[1].ship_idx = 1;
    MOX::_combat_data[1].officer_idx = -1;
    MOX::_ship[1].officer_index = 3;

    CMBTFIR2::Capture_Ship_(1, 0, 0);

    DOCTEST_CHECK(TEST_BOARDING::Killed_Officer_() == -1);
    DOCTEST_CHECK(TEST_BOARDING::Pooled_Officer_() == 3);
}

DOCTEST_TEST_CASE("shield generator damage refreshes stored shield capacity") {
    TEST_BOARDING::Reset_();
    TEST_BOARDING::Set_Combat_Shield_Max_(100);
    s_combat_data& ship = MOX::_combat_data[1];
    ship.shield_current_hits = 2;
    ship.shield_arc_max = 150;
    ship.shield_arc_current[0] = 150;
    ship.shield_arc_current[1] = 120;
    ship.shield_arc_current[2] = 100;
    ship.shield_arc_current[3] = 80;
    int16_t remaining_damage = 1;

    CMBTFIR2::Apply_Internal_Damage_(1, &remaining_damage, 2, 0, 0);

    DOCTEST_CHECK(ship.shield_arc_max == 100);
    DOCTEST_CHECK(ship.shield_arc_current[0] == 100);
    DOCTEST_CHECK(ship.shield_arc_current[1] == 100);
    DOCTEST_CHECK(ship.shield_arc_current[2] == 100);
    DOCTEST_CHECK(ship.shield_arc_current[3] == 80);
}

DOCTEST_TEST_CASE("destroyed multi-phased shields reduce shield capacity") {
    TEST_BOARDING::Reset_();
    TEST_BOARDING::Set_Combat_Shield_Max_(100);
    s_combat_data& ship = MOX::_combat_data[1];
    struct_::Set_Bit_Field_(
        ship.special_device_flags, SPECIAL_MULTI_PHASED_SHIELDS);
    ship.special_hits[0] = 10;
    ship.shield_arc_max = 150;
    ship.shield_arc_current[0] = 150;
    ship.shield_arc_current[1] = 120;
    ship.shield_arc_current[2] = 100;
    ship.shield_arc_current[3] = 80;
    int16_t remaining_damage = 10;

    CMBTFIR2::Apply_Internal_Damage_(1, &remaining_damage, 9, 0, 0);

    DOCTEST_CHECK(struct_::Test_Bit_Field_(
        ship.special_device_damage_flags,
        SPECIAL_MULTI_PHASED_SHIELDS) == 1);
    DOCTEST_CHECK(ship.shield_arc_max == 100);
    DOCTEST_CHECK(ship.shield_arc_current[0] == 100);
    DOCTEST_CHECK(ship.shield_arc_current[1] == 100);
    DOCTEST_CHECK(ship.shield_arc_current[2] == 100);
    DOCTEST_CHECK(ship.shield_arc_current[3] == 80);
}

DOCTEST_TEST_CASE("raid reports retain all damaged systems and correct special status") {
    TEST_BOARDING::Reset_();
    TEST_BOARDING::Prepare_Long_Raid_();
    int16_t raiders = 10;

    CMBTFIR2::Resolve_Raid_(0, 1, &raiders);

    const char* message = TEST_BOARDING::Last_Raid_Message_();
    DOCTEST_CHECK(strlen(message) > 500);
    DOCTEST_CHECK(strstr(message, "Special device seven final marker") != nullptr);
    DOCTEST_CHECK(strstr(message, "1 Special device zero with a deliberately long raid report name damaged") != nullptr);
    DOCTEST_CHECK(strstr(message, "3 Special device zero with a deliberately long raid report name") == nullptr);
}

DOCTEST_TEST_CASE("super-fast combat suppresses raid reports") {
    TEST_BOARDING::Reset_();
    TEST_BOARDING::Prepare_Long_Raid_();
    MOX::_super_fast_flag = 1;
    int16_t raiders = 10;

    CMBTFIR2::Resolve_Raid_(0, 1, &raiders);

    DOCTEST_CHECK(TEST_BOARDING::Last_Raid_Message_()[0] == '\0');
}
