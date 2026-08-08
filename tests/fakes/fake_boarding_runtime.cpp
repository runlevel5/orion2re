#include "pch.h"

#include "game/boarding.h"
#include "game/techdata.h"

namespace {
    s_combat_data combat_data[2];
    s_ship_data ships[2];
    s_player players[2];
    int16_t killed_officer;
    int16_t pooled_officer;
}

namespace TEST_BOARDING {
    void Reset_() {
        memset(combat_data, 0, sizeof(combat_data));
        memset(ships, 0, sizeof(ships));
        memset(players, 0, sizeof(players));
        for (s_combat_data& ship : combat_data) {
            ship.officer_idx = -1;
            ship.ship_idx = -1;
            ship.stasis_source_idx = UINT8_MAX;
            ship.black_hole_source_idx = UINT8_MAX;
        }
        for (s_ship_data& ship : ships) {
            ship.officer_index = -1;
        }
        memset(MOX::_ship_turn_order, -1, sizeof(MOX::_ship_turn_order));
        MOX::_ship_turn_order[0] = -10;
        MOX::_combat_attacker_player = 0;
        MOX::_combat_defender_player = 1;
        killed_officer = -1;
        pooled_officer = -1;
        BOARDING::_kill_excess_marines_after_capture = 0;
        BOARDING::_board_in_black_hole = 1;
    }

    int16_t Killed_Officer_() {
        return killed_officer;
    }

    int16_t Pooled_Officer_() {
        return pooled_officer;
    }
}

namespace MOX {
    s_combat_data* _combat_data = combat_data;
    s_ship_data* _ship = ships;
    s_player* _player = players;
    int16_t _ship_turn_order[210];
    int16_t _combat_attacker_player;
    int16_t _combat_defender_player;
} // namespace MOX

namespace OFFICER {
    void __cdecl Kill_Officer_(int16_t officer_idx) {
        killed_officer = officer_idx;
    }

    void __cdecl Move_Officer_To_Pool_(int16_t officer_idx) {
        pooled_officer = officer_idx;
    }
}

namespace COMBINIT {
    int16_t __cdecl Does_Combat_Ship_Have_Special_(int16_t, int16_t) {
        return 0;
    }
}

namespace CMBTFIRE {
    void __cdecl Destroy_Ship_(int16_t, int16_t, int16_t, int16_t) {
    }

    void __cdecl Retarget_Missiles_Aimed_At_(int16_t) {
    }
}

namespace COMBAT1 {
    void __cdecl Remove_Continuing_Effect_Specials_(int16_t) {
    }
}

namespace game_random {
    uint32_t __cdecl Random_(uint32_t) {
        return 0;
    }
}

namespace TECHDATA {
    s_tech_hull_data _hull_data[HULL_CLASS_COUNT];
}
