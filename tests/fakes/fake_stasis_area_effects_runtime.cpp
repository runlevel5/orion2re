#include "pch.h"

namespace {
    s_combat_data combat_data[2];
    s_qship qcombat_data[2];
    s_leader_data leaders[1];
    s_player players[MAX_PLAYERS];
}

namespace TEST_STASIS_AREA_EFFECTS {
    void Reset_() {
        memset(combat_data, 0, sizeof(combat_data));
        memset(qcombat_data, 0, sizeof(qcombat_data));
        memset(leaders, 0, sizeof(leaders));
        memset(players, 0, sizeof(players));

        for (s_combat_data& ship : combat_data) {
            ship.owner = 1;
            ship.officer_idx = -1;
            ship.stasis_source_idx = UINT8_MAX;
            ship.black_hole_source_idx = UINT8_MAX;
            ship.special_cooldown = UINT8_MAX;
        }

        MOX::_combat_total_ship_count = 2;
        MOX::_combat_attacker_count = 1;
        MOX::_combat_attacker_player = 0;
        MOX::_combat_defender_player = 1;
        COMBAT1::_continuous_bhg = 0;
        COMBINIT::_satellite_size_class_plus_one = 0;
    }

    void Set_Special_(int16_t ship_idx, int16_t special_id) {
        MOX::_combat_data[ship_idx].special_device_flags[special_id / 8] |=
            (uint8_t)(1u << (special_id % 8));
    }

    void Set_In_Stasis_(int16_t ship_idx, bool in_stasis) {
        MOX::_combat_data[ship_idx].stasis_source_idx = in_stasis ? 0 : UINT8_MAX;
    }
}

namespace MOX {
    s_combat_data* _combat_data = combat_data;
    s_qship* _qcombat_data = qcombat_data;
    int8_t _game_type;
    int16_t _combat_total_ship_count;
    int16_t _combat_attacker_count;
    int16_t _combat_attacker_player;
    int16_t _combat_defender_player;
    s_leader_data* _leaders = leaders;
    s_player* _player = players;
}

namespace COMBINIT {
    int16_t __cdecl Calc_Combat_Shield_Max_(int16_t) {
        return 0;
    }

    int16_t __cdecl Does_Combat_Ship_Have_Special_(int16_t ship_idx, int16_t special_id) {
        const uint8_t mask = (uint8_t)(1u << (special_id % 8));
        const s_combat_data& ship = MOX::_combat_data[ship_idx];
        return (ship.special_device_flags[special_id / 8] & mask) != 0 &&
               (ship.special_device_damage_flags[special_id / 8] & mask) == 0;
    }
}

namespace CMBTFIRE {
    int16_t __cdecl Is_Combat_Ship_Dead_(int16_t) {
        return 0;
    }

    uint16_t __cdecl Apply_Ship_Damage_(
        int16_t, int16_t*, int16_t*, int16_t, uint16_t, uint16_t,
        int16_t, int16_t, int16_t) {
        return 0;
    }
}

namespace COMBAT {
    void __cdecl Apply_Damage_To_Qship_(int16_t, int16_t) {}
}

namespace CMBTMOV1 {
    int8_t __cdecl Ship_Is_In_Stasis_(int16_t ship_idx) {
        return MOX::_combat_data[ship_idx].stasis_source_idx != UINT8_MAX;
    }

    bool __cdecl Ship_In_Black_Hole_(int16_t ship_idx) {
        return MOX::_combat_data[ship_idx].black_hole_source_idx != UINT8_MAX;
    }
}

namespace struct_ {
    int16_t __cdecl Test_Bit_Field_(void* bit_field, uint16_t bit_index) {
        const uint8_t* bytes = static_cast<const uint8_t*>(bit_field);
        return (bytes[bit_index / 8] >> (bit_index % 8)) & 1;
    }

    void __cdecl Set_Bit_Field_(void* bit_field, uint16_t bit_index) {
        uint8_t* bytes = static_cast<uint8_t*>(bit_field);
        bytes[bit_index / 8] |= static_cast<uint8_t>(1u << (bit_index % 8));
    }
}

namespace OFFICER {
    int16_t __cdecl Owned_Officer_Level_(int16_t) {
        return 0;
    }
}

namespace TECHDATA {
    s_tech_weapon_data _weapons[WEAPON_COUNT];
}