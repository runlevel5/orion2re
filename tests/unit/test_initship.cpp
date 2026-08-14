#include "pch.h"

#include "doctest/doctest.h"
#include "game/initship.h"
#include "game/shipstak.h"

namespace {
    constexpr int16_t kPlayer = 0;
    constexpr int16_t kDrive = DRIVE_NUCLEAR_DRIVE;
    constexpr int16_t kStrategicDrive = DRIVE_ANTI_MATTER_DRIVE;
    constexpr int16_t kMissileWeapon = 15;
    constexpr int16_t kTorpedoWeapon = 18;
    constexpr int16_t kTacticalBeamWeapon = 3;
    constexpr int16_t kStrategicBeamWeapon = 4;
    constexpr int16_t kTacticalBombWeapon = 21;
    constexpr int16_t kStrategicBombWeapon = 22;

    s_player players[MAX_PLAYERS];
    s_ship_data ships[1];

    void Reset_Initship_() {
        memset(players, 0, sizeof(players));
        memset(ships, 0, sizeof(ships));
        memset(MOX::_crew_data, 0, sizeof(MOX::_crew_data));
        memset(TECHDATA::_computers, 0, sizeof(TECHDATA::_computers));
        memset(TECHDATA::_drives, 0, sizeof(TECHDATA::_drives));
        memset(TECHDATA::_hull_data, 0, sizeof(TECHDATA::_hull_data));
        memset(TECHDATA::_specials, 0, sizeof(TECHDATA::_specials));
        memset(TECHDATA::_weapons, 0, sizeof(TECHDATA::_weapons));
        memset(TECHDATA::_weapon_modifications, 0,
               sizeof(TECHDATA::_weapon_modifications));

        TECHDATA::_drives[kDrive].tech_app_id = TECH_APP_NUCLEAR_DRIVE;
        TECHDATA::_drives[kDrive].warp_speed = 2;
        players[kPlayer].tech_applications[TECH_APP_NUCLEAR_DRIVE] =
            TECH_RESEARCH_STATUS_RESEARCHED;

        TECHDATA::_weapons[kMissileWeapon].tech_app_id =
            TECH_APP_NUCLEAR_MISSILE;
        TECHDATA::_weapons[kMissileWeapon].type = WEAPON_TYPE_MISSILE;
        TECHDATA::_weapons[kMissileWeapon].max_damage = 10;

        TECHDATA::_weapons[kTorpedoWeapon].tech_app_id =
            TECH_APP_ANTI_MATTER_TORPEDOES;
        TECHDATA::_weapons[kTorpedoWeapon].type = WEAPON_TYPE_TORPEDO;
        TECHDATA::_weapons[kTorpedoWeapon].max_damage = 20;

        players[kPlayer].tech_applications[TECH_APP_NUCLEAR_MISSILE] =
            TECH_RESEARCH_STATUS_RESEARCHED;
        players[kPlayer].tech_applications[TECH_APP_ANTI_MATTER_TORPEDOES] =
            TECH_RESEARCH_STATUS_RESEARCHED;

        INITSHIP::_satellite_uses_torpedoes = 0;
        INITSHIP::_td_ftl_bonus = 2;
        INITSHIP::_transport_ship_extended_range = 0;
        INITSHIP::_landers_size_class = SHIP_SIZE_SMALL;
        INITSHIP::lander_ship_uses_stealth = {};
        SHIPSTAK::stealth_bonus = { 500, 500, 500, 500 };
        MOX::_settings = {};
    }
}

namespace MOX {
    s_player* _player = players;
    s_ship_data* _ship = ships;
    s_crew_data _crew_data[CREW_QUALITY_COUNT];
    s_settings _settings = {};
    const char _ship_type_names[SHIP_TYPE_COUNT][6][16] = {};
}

namespace TECHDATA {
    s_tech_armor_data _armor[ARMOR_COUNT];
    s_tech_computer_data _computers[COMPUTER_COUNT];
    s_tech_drive_data _drives[DRIVE_COUNT];
    s_tech_hull_data _hull_data[HULL_CLASS_COUNT];
    s_tech_specials_data _specials[SPECIAL_COUNT];
    s_tech_weapon_data _weapons[WEAPON_COUNT];
    s_tech_weapon_mod_data _weapon_modifications[WEAPON_MOD_COUNT];
}

namespace CMBTFIR2 {
    int16_t __cdecl Helmsman_Bonus_(int16_t) {
        return 0;
    }

    int16_t __cdecl Weaponry_Bonus_(int16_t) {
        return 0;
    }
}

namespace DESIGN {
    int16_t __cdecl Miniaturization_Level_(int16_t, int16_t) {
        return 0;
    }
}

namespace COLCALC {
    int32_t __cdecl Ship_Type_Cost_For_Player_(int32_t, int16_t) {
        return 100;
    }
}

DOCTEST_TEST_CASE("Trans-Dimensional FTL speed uses the configured bonus") {
    Reset_Initship_();
    players[kPlayer].traits[TRAIT_TRANS_DIMENSIONAL] = 1;

    DOCTEST_CHECK(INITSHIP::Calc_Player_FTL_Speed_(kPlayer) == 4);
    DOCTEST_CHECK(INITSHIP::Get_FTL_Speed_(kPlayer, kDrive) == 4);

    INITSHIP::_td_ftl_bonus = 5;

    DOCTEST_CHECK(INITSHIP::Calc_Player_FTL_Speed_(kPlayer) == 7);
    DOCTEST_CHECK(INITSHIP::Get_FTL_Speed_(kPlayer, kDrive) == 7);

    players[kPlayer].traits[TRAIT_TRANS_DIMENSIONAL] = 0;
    DOCTEST_CHECK(INITSHIP::Get_FTL_Speed_(kPlayer, kDrive) == 2);
}

DOCTEST_TEST_CASE("planetary defenses select torpedoes only when enabled") {
    Reset_Initship_();

    DOCTEST_CHECK(INITSHIP::Best_Missile_Weapon_NT_(kPlayer) ==
                  kMissileWeapon);

    INITSHIP::_satellite_uses_torpedoes = 1;

    DOCTEST_CHECK(INITSHIP::Best_Missile_Weapon_NT_(kPlayer) ==
                  kTorpedoWeapon);
}

DOCTEST_TEST_CASE("strategic designs rank weapons by strategic damage") {
    Reset_Initship_();

    TECHDATA::_weapons[kTacticalBeamWeapon].tech_app_id =
        TECH_APP_LASER_CANNON;
    TECHDATA::_weapons[kTacticalBeamWeapon].type = WEAPON_TYPE_BEAM;
    TECHDATA::_weapons[kTacticalBeamWeapon].max_damage = 30;
    TECHDATA::_weapons[kTacticalBeamWeapon].strat_max_damage = 4;
    TECHDATA::_weapons[kStrategicBeamWeapon].tech_app_id =
        TECH_APP_PARTICLE_BEAM;
    TECHDATA::_weapons[kStrategicBeamWeapon].type = WEAPON_TYPE_BEAM;
    TECHDATA::_weapons[kStrategicBeamWeapon].max_damage = 10;
    TECHDATA::_weapons[kStrategicBeamWeapon].strat_max_damage = 25;

    TECHDATA::_weapons[kMissileWeapon].max_damage = 30;
    TECHDATA::_weapons[kMissileWeapon].strat_max_damage = 5;
    TECHDATA::_weapons[kTorpedoWeapon].max_damage = 10;
    TECHDATA::_weapons[kTorpedoWeapon].strat_max_damage = 25;

    TECHDATA::_weapons[kTacticalBombWeapon].tech_app_id =
        TECH_APP_NUCLEAR_BOMB;
    TECHDATA::_weapons[kTacticalBombWeapon].type = WEAPON_TYPE_BOMB;
    TECHDATA::_weapons[kTacticalBombWeapon].max_damage = 30;
    TECHDATA::_weapons[kTacticalBombWeapon].strat_max_damage = 3;
    TECHDATA::_weapons[kStrategicBombWeapon].tech_app_id =
        TECH_APP_FUSION_BOMB;
    TECHDATA::_weapons[kStrategicBombWeapon].type = WEAPON_TYPE_BOMB;
    TECHDATA::_weapons[kStrategicBombWeapon].max_damage = 10;
    TECHDATA::_weapons[kStrategicBombWeapon].strat_max_damage = 24;

    players[kPlayer].tech_applications[TECH_APP_LASER_CANNON] =
        TECH_RESEARCH_STATUS_RESEARCHED;
    players[kPlayer].tech_applications[TECH_APP_PARTICLE_BEAM] =
        TECH_RESEARCH_STATUS_RESEARCHED;
    players[kPlayer].tech_applications[TECH_APP_NUCLEAR_BOMB] =
        TECH_RESEARCH_STATUS_RESEARCHED;
    players[kPlayer].tech_applications[TECH_APP_FUSION_BOMB] =
        TECH_RESEARCH_STATUS_RESEARCHED;

    DOCTEST_CHECK(INITSHIP::Best_Beam_Weapon_(kPlayer) ==
                  kTacticalBeamWeapon);
    DOCTEST_CHECK(INITSHIP::Best_Missile_Weapon_(kPlayer) ==
                  kMissileWeapon);
    DOCTEST_CHECK(INITSHIP::Best_Bomb_Weapon_(kPlayer) ==
                  kTacticalBombWeapon);

    MOX::_settings.strategic_combat_flag = 1;

    DOCTEST_CHECK(INITSHIP::Best_Beam_Weapon_(kPlayer) ==
                  kStrategicBeamWeapon);
    DOCTEST_CHECK(INITSHIP::Best_Missile_Weapon_(kPlayer) ==
                  kTorpedoWeapon);
    DOCTEST_CHECK(INITSHIP::Best_Bomb_Weapon_(kPlayer) ==
                  kStrategicBombWeapon);
}

DOCTEST_TEST_CASE("fighter point defense skips shield-only beams") {
    Reset_Initship_();

    TECHDATA::_weapon_modifications[WEAPON_MOD_POINT_DEFENSE].level = 0;

    TECHDATA::_weapons[kMissileWeapon].type = WEAPON_TYPE_BEAM;
    TECHDATA::_weapons[kMissileWeapon].available_mods =
        WEAPON_MOD_FLAG_POINT_DEFENSE;
    TECHDATA::_weapons[kMissileWeapon].max_damage = 10;

    TECHDATA::_weapons[kTorpedoWeapon].type = WEAPON_TYPE_BEAM;
    TECHDATA::_weapons[kTorpedoWeapon].available_mods =
        WEAPON_MOD_FLAG_POINT_DEFENSE;
    TECHDATA::_weapons[kTorpedoWeapon].natural_mods =
        WEAPON_NATURAL_FLAG_ANTI_SHIELD;
    TECHDATA::_weapons[kTorpedoWeapon].max_damage = 20;

    DOCTEST_CHECK(
        INITSHIP::Best_Point_Defense_Weapon_(kPlayer) == kMissileWeapon);
}

DOCTEST_TEST_CASE("strategic ship bonuses use hull defense and suppress lander defense") {
    Reset_Initship_();
    s_ship_data& ship = ships[0];
    ship.owner = kPlayer;
    ship.d.ship_type = SHIP_TYPE_COMBAT;
    ship.d.size = SHIP_SIZE_SMALL;
    ship.d.combat_speed = 4;
    ship.d.computer_type = COMPUTER_ELECTRONIC_COMPUTER;
    ship.crew_quality = CREW_QUALITY_GREEN;
    ship.officer_index = -1;

    TECHDATA::_drives[kStrategicDrive].tech_app_id =
        TECH_APP_NUCLEAR_DRIVE;
    TECHDATA::_computers[COMPUTER_ELECTRONIC_COMPUTER].bonus = 10;
    TECHDATA::_hull_data[SHIP_SIZE_SMALL].strat_def_bonus = 50;
    players[kPlayer].traits[TRAIT_TRANS_DIMENSIONAL] = 1;

    int16_t defense;
    int16_t missile_defense;
    int16_t attack;

    MOX::_settings.strategic_combat_flag = 1;
    INITSHIP::Get_Ship_Combat_Bonuses_(
        0, &defense, &missile_defense, &attack);
    DOCTEST_CHECK(defense == 100);
    DOCTEST_CHECK(missile_defense == 0);
    DOCTEST_CHECK(attack == 10);

    MOX::_settings.strategic_combat_flag = 0;
    INITSHIP::Get_Ship_Combat_Bonuses_(
        0, &defense, &missile_defense, &attack);
    DOCTEST_CHECK(defense == 40);

    MOX::_settings.strategic_combat_flag = 1;
    ship.d.ship_type = SHIP_TYPE_COLONY;
    INITSHIP::Get_Ship_Combat_Bonuses_(
        0, &defense, &missile_defense, &attack);
    DOCTEST_CHECK(defense == 0);
    DOCTEST_CHECK(missile_defense == 0);
    DOCTEST_CHECK(attack == 10);
}

DOCTEST_TEST_CASE("transport extended range follows its setting") {
    Reset_Initship_();

    s_ship_design design = {};
    INITSHIP::Load_Transport_Ship_Design_(kPlayer, &design);
    DOCTEST_CHECK_FALSE(struct_::Test_Bit_Field_(
        design.special_device_flags, SPECIAL_EXTENDED_FUEL_TANKS));

    INITSHIP::_transport_ship_extended_range = 1;
    INITSHIP::Load_Transport_Ship_Design_(kPlayer, &design);
    DOCTEST_CHECK(struct_::Test_Bit_Field_(
        design.special_device_flags, SPECIAL_EXTENDED_FUEL_TANKS));
}

DOCTEST_TEST_CASE("configured landing-ship size applies to every lander design") {
    Reset_Initship_();
    INITSHIP::_landers_size_class = SHIP_SIZE_MEDIUM;

    s_ship_design colony = {};
    s_ship_design outpost = {};
    s_ship_design transport = {};
    INITSHIP::Load_Colony_Ship_Design_(kPlayer, &colony);
    INITSHIP::Load_Outpost_Ship_Design_(kPlayer, &outpost);
    INITSHIP::Load_Transport_Ship_Design_(kPlayer, &transport);

    DOCTEST_CHECK(colony.size == SHIP_SIZE_MEDIUM);
    DOCTEST_CHECK(outpost.size == SHIP_SIZE_MEDIUM);
    DOCTEST_CHECK(transport.size == SHIP_SIZE_MEDIUM);
}

DOCTEST_TEST_CASE("lander stealth remains disabled by default") {
    Reset_Initship_();
    players[kPlayer].tech_applications[TECH_APP_PHASING_CLOAK] =
        TECH_RESEARCH_STATUS_RESEARCHED;

    s_ship_design colony = {};
    s_ship_design outpost = {};
    s_ship_design transport = {};
    INITSHIP::Load_Colony_Ship_Design_(kPlayer, &colony);
    INITSHIP::Load_Outpost_Ship_Design_(kPlayer, &outpost);
    INITSHIP::Load_Transport_Ship_Design_(kPlayer, &transport);

    DOCTEST_CHECK(struct_::Test_Bit_Field_(
        colony.special_device_flags, SPECIAL_EXTENDED_FUEL_TANKS));
    DOCTEST_CHECK(struct_::Test_Bit_Field_(
        outpost.special_device_flags, SPECIAL_EXTENDED_FUEL_TANKS));
    DOCTEST_CHECK_FALSE(struct_::Test_Bit_Field_(
        colony.special_device_flags, SPECIAL_PHASING_CLOAK));
    DOCTEST_CHECK_FALSE(struct_::Test_Bit_Field_(
        outpost.special_device_flags, SPECIAL_PHASING_CLOAK));
    DOCTEST_CHECK_FALSE(struct_::Test_Bit_Field_(
        transport.special_device_flags, SPECIAL_PHASING_CLOAK));
}

DOCTEST_TEST_CASE("enabled landers install the best researched stealth device") {
    Reset_Initship_();
    players[kPlayer].tech_applications[TECH_APP_PHASING_CLOAK] =
        TECH_RESEARCH_STATUS_RESEARCHED;
    INITSHIP::lander_ship_uses_stealth = { 1, 1, 1 };

    s_ship_design colony = {};
    s_ship_design outpost = {};
    s_ship_design transport = {};
    INITSHIP::Load_Colony_Ship_Design_(kPlayer, &colony);
    INITSHIP::Load_Outpost_Ship_Design_(kPlayer, &outpost);
    INITSHIP::Load_Transport_Ship_Design_(kPlayer, &transport);

    DOCTEST_CHECK(struct_::Test_Bit_Field_(
        colony.special_device_flags, SPECIAL_PHASING_CLOAK));
    DOCTEST_CHECK(struct_::Test_Bit_Field_(
        outpost.special_device_flags, SPECIAL_PHASING_CLOAK));
    DOCTEST_CHECK(struct_::Test_Bit_Field_(
        transport.special_device_flags, SPECIAL_PHASING_CLOAK));
}

DOCTEST_TEST_CASE("lander stealth selection works for nonzero players") {
    Reset_Initship_();
    constexpr int16_t player_idx = 1;
    players[player_idx].tech_applications[TECH_APP_CLOAKING_DEVICE] =
        TECH_RESEARCH_STATUS_RESEARCHED;
    players[player_idx].tech_applications[TECH_APP_PHASING_CLOAK] =
        TECH_RESEARCH_STATUS_RESEARCHED;
    INITSHIP::lander_ship_uses_stealth.colony_ship = 1;
    SHIPSTAK::stealth_bonus.cloaking_device = 600;

    s_ship_design design = {};
    INITSHIP::Load_Colony_Ship_Design_(player_idx, &design);

    DOCTEST_CHECK(struct_::Test_Bit_Field_(
        design.special_device_flags, SPECIAL_CLOAKING_DEVICE));
    DOCTEST_CHECK_FALSE(struct_::Test_Bit_Field_(
        design.special_device_flags, SPECIAL_PHASING_CLOAK));

    SHIPSTAK::stealth_bonus.phasing_cloak = 600;
    INITSHIP::Load_Colony_Ship_Design_(player_idx, &design);

    DOCTEST_CHECK_FALSE(struct_::Test_Bit_Field_(
        design.special_device_flags, SPECIAL_CLOAKING_DEVICE));
    DOCTEST_CHECK(struct_::Test_Bit_Field_(
        design.special_device_flags, SPECIAL_PHASING_CLOAK));
}