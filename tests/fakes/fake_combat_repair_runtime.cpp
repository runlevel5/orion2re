#include "pch.h"

namespace {
    s_combat_data combat_data[2];
    s_ship_data ships[2];
    s_player players[MAX_PLAYERS];
    int16_t engineer_bonus[MAX_PLAYERS];
    int16_t repair_amount[2];
    int repair_count[2];
    int speed_calc_count;
}

namespace TEST_COMBAT_REPAIR {
    void Reset_() {
        memset(combat_data, 0, sizeof(combat_data));
        memset(ships, 0, sizeof(ships));
        memset(players, 0, sizeof(players));
        memset(engineer_bonus, 0, sizeof(engineer_bonus));
        memset(repair_amount, 0, sizeof(repair_amount));
        memset(repair_count, 0, sizeof(repair_count));
        memset(TECHDATA::_weapons, 0, sizeof(TECHDATA::_weapons));
        speed_calc_count = 0;

        MOX::_combat_total_ship_count = 2;
        MOX::_combat_attacker_player = 0;
        MOX::_combat_defender_player = 1;
        combat_data[0].owner = 0;
        combat_data[1].owner = 1;
        combat_data[0].stasis_source_idx = UINT8_MAX;
        combat_data[1].stasis_source_idx = UINT8_MAX;
        COMBINIT::_amoeba_regeneration = 0;
        COMBINIT::repair_rate = {10, 20, 20};
    }

    void Set_Zero_Speed_Unload_() {
        combat_data[0].ship_idx = 0;
        combat_data[0].current_speed = 0;
        combat_data[0].drive_current_hits = 0;
        combat_data[0].drive_max_hits = 10;
        ships[0].d.combat_speed = 0;
    }

    int Speed_Calc_Count_() {
        return speed_calc_count;
    }

    int16_t Drive_Current_Hits_() {
        return combat_data[0].drive_current_hits;
    }

    uint8_t Stored_Drive_Damage_() {
        return ships[0].drive_damage_percent;
    }

    void Set_Special_(int16_t ship_idx, int16_t special_id) {
        combat_data[ship_idx].special_device_flags[special_id / 8] |=
            (uint8_t)(1u << (special_id % 8));
    }

    void Damage_Special_(int16_t ship_idx, int16_t special_id) {
        combat_data[ship_idx].special_device_damage_flags[special_id / 8] |=
            (uint8_t)(1u << (special_id % 8));
    }

    void Set_Cybernetic_(int16_t player_idx) {
        players[player_idx].traits[TRAIT_CYBERNETIC] = 1;
    }

    void Set_Engineer_Bonus_(int16_t player_idx, int16_t amount) {
        engineer_bonus[player_idx] = amount;
    }

    int Repair_Count_(int16_t ship_idx) {
        return repair_count[ship_idx];
    }

    int16_t Repair_Amount_(int16_t ship_idx) {
        return repair_amount[ship_idx];
    }
}

namespace MOX {
    s_combat_data* _combat_data = combat_data;
    s_ship_data* _ship = ships;
    s_player* _player = players;
    int16_t _combat_total_ship_count;
    int16_t _combat_attacker_player;
    int16_t _combat_defender_player;
}

namespace TECHDATA {
    s_tech_weapon_data _weapons[WEAPON_COUNT];
}

namespace COMBAT1 {
    int16_t __cdecl Calc_Current_Speed_(int16_t) {
        speed_calc_count++;
        return 0;
    }
}

namespace INITSHIP {
    int16_t __cdecl Get_Ship_Armor_Hits_(int16_t) {
        return 0;
    }
}

namespace struct_ {
    int16_t __cdecl Test_Bit_Field_(void* bit_field, uint16_t bit_index) {
        const uint8_t* bytes = static_cast<const uint8_t*>(bit_field);
        return (bytes[bit_index / 8] >> (bit_index % 8)) & 1;
    }
}

namespace CMBTMOV1 {
    int8_t __cdecl Ship_Is_In_Stasis_(int16_t ship_idx) {
        return MOX::_combat_data[ship_idx].stasis_source_idx != UINT8_MAX;
    }
}

namespace CMBTFIR2 {
    int16_t __cdecl Get_Fleet_Engineer_Bonus_(int16_t player_idx) {
        return player_idx >= 0 && player_idx < MAX_PLAYERS
            ? engineer_bonus[player_idx]
            : 0;
    }

    void __cdecl Repair_Combat_Ship_(int16_t ship_idx, int16_t amount) {
        repair_count[ship_idx]++;
        repair_amount[ship_idx] = amount;
    }
}