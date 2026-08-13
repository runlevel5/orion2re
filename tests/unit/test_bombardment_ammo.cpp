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
    s_player players[MAX_PLAYERS];
    s_qship qcombat_data[2];
    int allocation_count;
}

namespace TEST_BOMBARDMENT_AMMO {
    void Reset_() {
        memset(combat_data, 0, sizeof(combat_data));
        memset(missiles, 0, sizeof(missiles));
        memset(ships, 0, sizeof(ships));
        memset(colonies, 0, sizeof(colonies));
        memset(players, 0, sizeof(players));
        memset(qcombat_data, 0, sizeof(qcombat_data));
        memset(TECHDATA::_hull_data, 0, sizeof(TECHDATA::_hull_data));
        memset(TECHDATA::_shields, 0, sizeof(TECHDATA::_shields));
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

        MOX::_qcombat_data = qcombat_data;
        MOX::_combat_total_ship_count = 2;
        MOX::_cmbt_colony_id = -1;
        allocation_count = 0;
    }
}

namespace MOX {
    s_animation_header* _screen_seg;
    s_player* _player = players;
    s_ship_data* _ship = ships;
    s_colony* _colony = colonies;
    s_qship* _qcombat_data = qcombat_data;
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
    s_tech_hull_data _hull_data[HULL_CLASS_COUNT];
    s_tech_shield_data _shields[SHIELD_COUNT];
    s_tech_weapon_data _weapons[WEAPON_COUNT];
}

namespace game_random {
    uint32_t __cdecl Random_(uint32_t) {
        return 1;
    }
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
    int16_t __cdecl Best_Armor_(int16_t) {
        return 0;
    }

    int16_t __cdecl Best_Point_Defense_Weapon_(int16_t) {
        return kBestPointDefenseWeapon;
    }

    int16_t __cdecl Best_Bomb_Weapon_(int16_t) {
        return kBestBombWeapon;
    }

    int16_t __cdecl Strat_Missile_Weapon_Count_(int16_t hull_idx) {
        return TECHDATA::_hull_data[hull_idx].strat_missile;
    }

    int16_t __cdecl Strat_Special_Weapon_Count_(int16_t hull_idx) {
        return TECHDATA::_hull_data[hull_idx].strat_special;
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

void Legacy_Qsort_(void*, size_t, size_t, legacy_qsort_compare) {
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

DOCTEST_TEST_CASE("strategic attacks use strategic weapon damage") {
    TEST_BOMBARDMENT_AMMO::Reset_();
    constexpr int16_t attacker_idx = 1;
    constexpr int16_t target_idx = 0;
    constexpr int16_t weapon_id = 7;
    constexpr int16_t hull_idx = 0;

    s_tech_weapon_data& weapon = TECHDATA::_weapons[weapon_id];
    weapon.min_damage = 100;
    weapon.max_damage = 100;
    weapon.strat_min_damage = 7;
    weapon.strat_max_damage = 7;

    TECHDATA::_hull_data[hull_idx].strat_beam = 1;
    TECHDATA::_hull_data[hull_idx].strat_missile = 1;
    TECHDATA::_hull_data[hull_idx].strat_special = 1;
    TECHDATA::_hull_data[hull_idx].strat_bomb = 1;

    qcombat_data[attacker_idx].player = kAttacker;
    qcombat_data[attacker_idx].size = hull_idx;
    qcombat_data[attacker_idx].base_ocv = 100;
    qcombat_data[attacker_idx].beam_weapon_id = weapon_id;
    qcombat_data[attacker_idx].missile_weapon_id = weapon_id;
    qcombat_data[attacker_idx].special_weapon_id = weapon_id;
    qcombat_data[attacker_idx].bomb_weapon_id = weapon_id;
    qcombat_data[target_idx].max_hits = 30000;

    COMBAT::Beam_Attack_(attacker_idx, target_idx);
    DOCTEST_CHECK(qcombat_data[target_idx].structural_damage == 7);

    qcombat_data[target_idx].structural_damage = 0;
    COMBAT::Missile_Attack_(attacker_idx, target_idx);
    DOCTEST_CHECK(qcombat_data[target_idx].structural_damage == 7);

    qcombat_data[target_idx].structural_damage = 0;
    COMBAT::Strat_Special_Attack_(attacker_idx, target_idx);
    DOCTEST_CHECK(qcombat_data[target_idx].structural_damage == 7);

    qcombat_data[target_idx].structural_damage = 0;
    COMBAT::Strat_Bomb_Attack_(attacker_idx);
    DOCTEST_CHECK(qcombat_data[target_idx].structural_damage == 7);

    qcombat_data[target_idx].structural_damage = 0;
    TECHDATA::_weapons[WEAPON_STELLAR_CONVERTER].max_damage = 200;
    TECHDATA::_weapons[WEAPON_STELLAR_CONVERTER].strat_max_damage = 9;
    COMBAT::Stellar_Converter_Attack_(target_idx);
    DOCTEST_CHECK(qcombat_data[target_idx].structural_damage == 36);
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