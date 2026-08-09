#include "pch.h"

#include "doctest/doctest.h"
#include "game/combat.h"

namespace {
    constexpr int16_t kAttacker = 2;
    constexpr int16_t kBeamWeapon = 1;
    constexpr int16_t kMissileWeapon = 2;
    constexpr int16_t kTorpedoWeapon = 3;
    constexpr int16_t kBombWeapon = 4;
    constexpr int16_t kBestPointDefenseWeapon = 5;
    constexpr int16_t kBestBombWeapon = 6;

    s_combat_data combat_data[210];
    s_missile missiles[300];
    s_ship_data ships[1];
    s_colony colonies[1];
    int allocation_count;
}

namespace TEST_BOMBARDMENT_AMMO {
    void Reset_() {
        memset(combat_data, 0, sizeof(combat_data));
        memset(missiles, 0, sizeof(missiles));
        memset(ships, 0, sizeof(ships));
        memset(colonies, 0, sizeof(colonies));
        memset(TECHDATA::_weapons, 0, sizeof(TECHDATA::_weapons));
        memset(MOX::_fighter_beam_type, 0, sizeof(MOX::_fighter_beam_type));
        memset(MOX::_fighter_bomb_type, 0, sizeof(MOX::_fighter_bomb_type));

        ships[0].owner = kAttacker;
        ships[0].d.ship_type = SHIP_TYPE_COMBAT;
        colonies[0].owner = 1;

        TECHDATA::_weapons[kBeamWeapon].type = WEAPON_TYPE_BEAM;
        TECHDATA::_weapons[kMissileWeapon].type = WEAPON_TYPE_MISSILE;
        TECHDATA::_weapons[kTorpedoWeapon].type = WEAPON_TYPE_TORPEDO;
        TECHDATA::_weapons[kBombWeapon].type = WEAPON_TYPE_BOMB;

        allocation_count = 0;
    }
}

namespace MOX {
    s_animation_header* _screen_seg;
    s_ship_data* _ship = ships;
    s_colony* _colony = colonies;
    s_combat_data* _combat_data;
    s_missile* _missile;
    int16_t _fighter_beam_type[9];
    int16_t _fighter_bomb_type[9];
    int16_t _cmbt_colony_id;
    uint16_t _apply_damage_to_planet;
    int16_t _combat_total_ship_count;
    int16_t _combat_attacker_player;
    int16_t _combat_defender_player;
    s_combat_bomb_result _combat_bomb_result;
}

namespace TECHDATA {
    s_tech_weapon_data _weapons[WEAPON_COUNT];
}

namespace allocate {
    void __cdecl Push_Block_(void*) {
    }

    void* __cdecl Allocate_Next_Block_(void*, uint32_t) {
        return allocation_count++ == 0
            ? static_cast<void*>(combat_data)
            : static_cast<void*>(missiles);
    }

    void __cdecl Pop_Block_(void*) {
    }
}

namespace struct_ {
    void __cdecl Clear_Structure_(void* dest, uint32_t size) {
        memset(dest, 0, size);
    }
}

namespace INITSHIP {
    int16_t __cdecl Best_Point_Defense_Weapon_(int16_t) {
        return kBestPointDefenseWeapon;
    }

    int16_t __cdecl Best_Bomb_Weapon_(int16_t) {
        return kBestBombWeapon;
    }
}

namespace COMBINIT {
    void __cdecl Load_Combat_Ship_(int16_t, int16_t combat_idx) {
        s_combat_data* ship = &MOX::_combat_data[combat_idx];
        const int16_t weapon_ids[] = {
            kBeamWeapon,
            kMissileWeapon,
            kTorpedoWeapon,
            kBombWeapon,
        };

        for (int16_t i = 0; i < 4; ++i) {
            ship->weapons[i].weapon_id = weapon_ids[i];
            ship->weapons[i].count = 1;
            ship->weapons[i].ammo = 20;
        }
    }

    void __cdecl Load_Tactical_Colony_(int16_t, int16_t) {
        MOX::_combat_data[0].structure_max = 100;
    }

    void __cdecl Init_Special_Devices_(int16_t) {
    }
}

namespace CMBTFIR2 {
    void __cdecl Set_Combat_Ordnance_Values_() {
    }

    int16_t __cdecl Offensive_Combat_Bonus_(int16_t) {
        return 0;
    }

    int16_t __cdecl Defensive_Combat_Bonus_(int16_t, int16_t) {
        return 0;
    }
}

namespace CMBTFIRE {
    int16_t __cdecl Fire_Ship_(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t) {
        return 0;
    }
}

DOCTEST_TEST_CASE("tactical bombardment advances only torpedo cooldown") {
    TEST_BOMBARDMENT_AMMO::Reset_();
    int16_t ship_indices[] = {0};
    s_combat_bomb_result result = {};
    result.attacker_idx = kAttacker;

    COMBAT::Tactical_Bombardment_(ship_indices, 1, &result);

    const s_combat_data* ship = &combat_data[1];
    DOCTEST_CHECK(ship->weapons[0].ammo == 20);
    DOCTEST_CHECK(ship->weapons[1].ammo == 20);
    DOCTEST_CHECK(ship->weapons[2].ammo == 10);
    DOCTEST_CHECK(ship->weapons[3].ammo == 20);
}

DOCTEST_TEST_CASE("tactical bombardment refreshes attacker fighter weapons") {
    TEST_BOMBARDMENT_AMMO::Reset_();
    int16_t ship_indices[] = {0};
    s_combat_bomb_result result = {};
    result.attacker_idx = kAttacker;

    COMBAT::Tactical_Bombardment_(ship_indices, 1, &result);

    DOCTEST_CHECK(MOX::_fighter_beam_type[kAttacker] == kBestPointDefenseWeapon);
    DOCTEST_CHECK(MOX::_fighter_bomb_type[kAttacker] == kBestBombWeapon);
}

DOCTEST_TEST_CASE("empty tactical bombardment uses the context attacker") {
    TEST_BOMBARDMENT_AMMO::Reset_();
    ships[0].owner = 3;
    int16_t unused_ship_indices[] = {0};
    s_combat_bomb_result result = {};
    result.attacker_idx = kAttacker;

    COMBAT::Tactical_Bombardment_(unused_ship_indices, 0, &result);

    DOCTEST_CHECK(MOX::_combat_attacker_player == kAttacker);
    DOCTEST_CHECK(MOX::_fighter_beam_type[kAttacker] == kBestPointDefenseWeapon);
    DOCTEST_CHECK(MOX::_fighter_bomb_type[kAttacker] == kBestBombWeapon);
}