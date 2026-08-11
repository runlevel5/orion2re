#include "pch.h"

#include "doctest/doctest.h"
#include "game/initship.h"

namespace {
    constexpr int16_t kPlayer = 0;
    constexpr int16_t kDrive = DRIVE_NUCLEAR_DRIVE;
    constexpr int16_t kMissileWeapon = 15;
    constexpr int16_t kTorpedoWeapon = 18;

    s_player players[MAX_PLAYERS];

    void Reset_Initship_() {
        memset(players, 0, sizeof(players));
        memset(TECHDATA::_drives, 0, sizeof(TECHDATA::_drives));
        memset(TECHDATA::_weapons, 0, sizeof(TECHDATA::_weapons));

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
    }
}

namespace MOX {
    s_player* _player = players;
}

namespace TECHDATA {
    s_tech_drive_data _drives[DRIVE_COUNT];
    s_tech_weapon_data _weapons[WEAPON_COUNT];
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