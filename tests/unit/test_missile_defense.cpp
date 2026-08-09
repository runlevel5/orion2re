#include "pch.h"

#include "doctest/doctest.h"
#include "game/cmbtmis.h"

namespace {
    s_combat_data combat_data[3];
    s_missile missiles[300];
    s_player players[MAX_PLAYERS];
    int16_t target_ranges[3];
    int16_t fleet_pilot_bonus;
    int16_t fleet_defensive_bonus;
    int16_t fleet_defensive_player;
    int16_t fleet_defensive_mode;
}

namespace TEST_FIGHTER_RETARGETING {
    void Reset_() {
        memset(combat_data, 0, sizeof(combat_data));
        memset(missiles, 0, sizeof(missiles));
        memset(players, 0, sizeof(players));
        memset(TECHDATA::_weapons, 0, sizeof(TECHDATA::_weapons));
        memset(target_ranges, 0, sizeof(target_ranges));
    }

    void Set_Range_(int16_t ship_idx, int16_t range) {
        target_ranges[ship_idx] = range;
    }
}

namespace MOX {
    s_combat_data* _combat_data = combat_data;
    s_missile* _missile = missiles;
    s_player* _player = players;
    int16_t _combat_total_ship_count;
    int16_t _combat_attacker_player;
    int16_t _combat_defender_player;
}

namespace TECHDATA {
    s_tech_armor_data _armor[ARMOR_COUNT];
    s_tech_weapon_data _weapons[WEAPON_COUNT];
}

namespace CMBTFIRE {
    int16_t __cdecl Does_Combat_Planet_Have_Defenses_() {
        return 1;
    }

    int16_t __cdecl Target_Is_Legal_For_Weapon_(int16_t, uint16_t, int16_t, int16_t) {
        return 1;
    }
}

namespace CMBTMOV1 {
    int8_t __cdecl Ship_Is_In_Stasis_(int16_t) {
        return 0;
    }
}

namespace CMBTAI {
    int16_t __cdecl Range_To_Missile_(int16_t unit_idx, int16_t) {
        return target_ranges[unit_idx];
    }
}

namespace INITSHIP {
    int16_t __cdecl Best_Armor_(int16_t) {
        return ARMOR_NO_ARMOR;
    }

    int16_t __cdecl Best_Warp_Drive_(int16_t) {
        return 1;
    }
}

namespace CMBTFIR2 {
    int16_t __cdecl Get_Fleet_Pilot_Bonus_(int16_t) {
        return fleet_pilot_bonus;
    }

    int16_t __cdecl Fleet_Defensive_Bonus_(int16_t player_idx, int16_t mode, int16_t*) {
        fleet_defensive_player = player_idx;
        fleet_defensive_mode = mode;
        return fleet_defensive_bonus;
    }
}

DOCTEST_TEST_CASE("fighter defense includes the fleet helmsman bonus") {
    memset(TECHDATA::_weapons, 0, sizeof(TECHDATA::_weapons));
    memset(players, 0, sizeof(players));

    TECHDATA::_weapons[WEAPON_INTERCEPTOR].type = WEAPON_TYPE_FIGHTER;
    players[3].traits[TRAIT_SHIP_DEFENSE] = 7;
    fleet_pilot_bonus = 10;
    fleet_defensive_bonus = 15;
    fleet_defensive_player = -1;
    fleet_defensive_mode = -1;

    DOCTEST_CHECK(CMBTMIS::Missile_Dcv_(WEAPON_INTERCEPTOR, 3) == 82);
    DOCTEST_CHECK(fleet_defensive_player == 3);
    DOCTEST_CHECK(fleet_defensive_mode == 0);
}

DOCTEST_TEST_CASE("missile defense percentage rolls include their threshold") {
    DOCTEST_CHECK_FALSE(CMBTMIS::Missile_Defense_Roll_Succeeds_(1, 0));

    DOCTEST_CHECK(CMBTMIS::Missile_Defense_Roll_Succeeds_(29, 30));
    DOCTEST_CHECK(CMBTMIS::Missile_Defense_Roll_Succeeds_(30, 30));
    DOCTEST_CHECK_FALSE(CMBTMIS::Missile_Defense_Roll_Succeeds_(31, 30));

    DOCTEST_CHECK(CMBTMIS::Missile_Defense_Roll_Succeeds_(49, 50));
    DOCTEST_CHECK(CMBTMIS::Missile_Defense_Roll_Succeeds_(50, 50));
    DOCTEST_CHECK_FALSE(CMBTMIS::Missile_Defense_Roll_Succeeds_(51, 50));
}

DOCTEST_TEST_CASE("missile hit points never fall to zero") {
    memset(TECHDATA::_armor, 0, sizeof(TECHDATA::_armor));
    TECHDATA::_armor[ARMOR_NO_ARMOR].ships_bonus = -100;

    const int16_t fighter_types[] = {
        WEAPON_ASSAULT_SHUTTLE,
        WEAPON_HEAVY_FIGHTER,
        WEAPON_BOMBER,
        WEAPON_INTERCEPTOR,
    };

    for (int16_t fighter_type : fighter_types) {
        DOCTEST_CHECK(CMBTMIS::Get_Missile_Hits_(fighter_type, 0, 0) == 1);
    }

    DOCTEST_CHECK(CMBTMIS::Get_Missile_Hits_(WEAPON_NO_WEAPONS, 0, 0) == 1);
    DOCTEST_CHECK(CMBTMIS::Get_Missile_Hits_(WEAPON_NO_WEAPONS, 0, WEAPON_MOD_FLAG_HEAVILY_ARMORED) == 1);
    DOCTEST_CHECK(CMBTMIS::Get_Missile_Hits_(WEAPON_NUCLEAR_MISSILE, 0, 0) == 4);
    DOCTEST_CHECK(CMBTMIS::Get_Missile_Hits_(WEAPON_NUCLEAR_MISSILE, 0, WEAPON_MOD_FLAG_HEAVILY_ARMORED) == 8);
}

DOCTEST_TEST_CASE("expected missile damage applies only the active cloak defense mode") {
    CMBTMIS::missile_defense.cloaking_device_missile_defense = 0;
    CMBTMIS::missile_defense.cloaking_device_missile_miss_percent = 50;

    DOCTEST_CHECK(CMBTMIS::Expected_Damage_After_Cloak_(100, false) == 100);
    DOCTEST_CHECK(CMBTMIS::Expected_Damage_After_Cloak_(100, true) == 50);

    CMBTMIS::missile_defense.cloaking_device_missile_miss_percent = 30;
    DOCTEST_CHECK(CMBTMIS::Expected_Damage_After_Cloak_(100, true) == 70);

    CMBTMIS::missile_defense.cloaking_device_missile_defense = 40;
    DOCTEST_CHECK(CMBTMIS::Expected_Damage_After_Cloak_(100, true) == 100);

    CMBTMIS::missile_defense = { 0, 50, 30 };
}

DOCTEST_TEST_CASE("flyer targeting follows anti-missile chains to the final ship") {
    s_missile missiles[3] = {};

    missiles[0].is_anti_missile_rocket = 1;
    missiles[0].target_unit_idx = 1;
    missiles[1].is_anti_missile_rocket = 1;
    missiles[1].target_unit_idx = 2;
    missiles[2].target_unit_idx = 7;

    DOCTEST_CHECK(CMBTMIS::Final_Flyer_Target_(missiles, 0) == 7);
    DOCTEST_CHECK(CMBTMIS::Final_Flyer_Target_(missiles, 2) == 7);
}

DOCTEST_TEST_CASE("ship movement excludes flyers targeting another missile") {
    s_missile missile = {};
    missile.type = WEAPON_NUCLEAR_MISSILE;
    missile.target_unit_idx = 4;

    DOCTEST_CHECK(CMBTMIS::Missile_Targets_Ship_(&missile, 4));

    missile.is_anti_missile_rocket = 1;
    DOCTEST_CHECK_FALSE(CMBTMIS::Missile_Targets_Ship_(&missile, 4));

    missile.is_anti_missile_rocket = 0;
    missile.type = WEAPON_NO_WEAPONS;
    DOCTEST_CHECK_FALSE(CMBTMIS::Missile_Targets_Ship_(&missile, 4));
}

DOCTEST_TEST_CASE("returning fighters refill surviving bays in slot order") {
    TEST_FIGHTER_RETARGETING::Reset_();

    s_combat_data* source = &MOX::_combat_data[1];
    source->weapons[0].weapon_id = WEAPON_INTERCEPTOR;
    source->weapons[0].count = 2;
    source->weapons[0].ammo = 0;
    source->weapons[0].shots_left = 2;
    source->weapon_hits[0] = 13;
    source->weapon_hits_per_unit[0] = 3;

    source->weapons[1].weapon_id = WEAPON_INTERCEPTOR;
    source->weapons[1].count = 4;
    source->weapons[1].ammo = 0;
    source->weapons[1].shots_left = 4;
    source->weapon_hits[1] = 10;
    source->weapon_hits_per_unit[1] = 3;

    source->weapons[2].weapon_id = WEAPON_BOMBER;
    source->weapons[2].count = 3;
    source->weapons[2].ammo = 0;

    CMBTMIS::Land_Returning_Fighters_(source, WEAPON_INTERCEPTOR, 6);

    DOCTEST_CHECK(source->weapons[0].count == 5);
    DOCTEST_CHECK(source->weapons[0].ammo == 1);
    DOCTEST_CHECK(source->weapons[0].shots_left == 0);
    DOCTEST_CHECK(source->weapons[1].count == 1);
    DOCTEST_CHECK(source->weapons[1].ammo == 1);
    DOCTEST_CHECK(source->weapons[1].shots_left == 0);
    DOCTEST_CHECK(source->weapons[2].count == 3);
    DOCTEST_CHECK(source->weapons[2].ammo == 0);
}

DOCTEST_TEST_CASE("anti-missile fighter can retarget a ship sharing its missile target index") {
    TEST_FIGHTER_RETARGETING::Reset_();

    MOX::_combat_attacker_player = 0;
    MOX::_combat_defender_player = 1;
    MOX::_combat_total_ship_count = 3;
    MOX::_combat_data[0].owner = 0;
    MOX::_combat_data[1].owner = 1;
    MOX::_combat_data[2].owner = 1;

    TECHDATA::_weapons[WEAPON_INTERCEPTOR].type = WEAPON_TYPE_FIGHTER;
    MOX::_missile[0].type = WEAPON_INTERCEPTOR;
    MOX::_missile[0].source_unit_idx = 0;
    MOX::_missile[0].target_unit_idx = 1;
    MOX::_missile[0].is_anti_missile_rocket = 1;
    MOX::_missile[0].is_active = 1;

    TEST_FIGHTER_RETARGETING::Set_Range_(1, 5);
    TEST_FIGHTER_RETARGETING::Set_Range_(2, 10);

    CMBTMIS::Retarget_Missile_(0);

    DOCTEST_CHECK(MOX::_missile[0].target_unit_idx == 1);
    DOCTEST_CHECK(MOX::_missile[0].is_anti_missile_rocket == 0);
}