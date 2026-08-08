#include "pch.h"

namespace {
    s_combat_data combat_data[2];
    s_leader_data leaders[1];
}

namespace TEST_STASIS_AREA_EFFECTS {
    void Reset_() {
        memset(combat_data, 0, sizeof(combat_data));
        memset(leaders, 0, sizeof(leaders));

        for (s_combat_data& ship : combat_data) {
            ship.owner = 1;
            ship.officer_idx = -1;
            ship.stasis_source_idx = UINT8_MAX;
        }

        MOX::_combat_total_ship_count = 2;
        MOX::_combat_attacker_player = 0;
        MOX::_combat_defender_player = 1;
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
    int16_t _combat_total_ship_count;
    int16_t _combat_attacker_player;
    int16_t _combat_defender_player;
    s_leader_data* _leaders = leaders;
}

namespace COMBINIT {
    int16_t __cdecl Does_Combat_Ship_Have_Special_(int16_t ship_idx, int16_t special_id) {
        const uint8_t mask = (uint8_t)(1u << (special_id % 8));
        const s_combat_data& ship = MOX::_combat_data[ship_idx];
        return (ship.special_device_flags[special_id / 8] & mask) != 0 &&
               (ship.special_device_damage_flags[special_id / 8] & mask) == 0;
    }
}

namespace CMBTMOV1 {
    int8_t __cdecl Ship_Is_In_Stasis_(int16_t ship_idx) {
        return MOX::_combat_data[ship_idx].stasis_source_idx != UINT8_MAX;
    }
}

namespace OFFICER {
    int16_t __cdecl Owned_Officer_Level_(int16_t) {
        return 0;
    }
}