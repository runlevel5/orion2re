#include "pch.h"

#include "game/cmbtmis.h"
#include "game/cmbtspec.h"
#include "game/combat1.h"

namespace {
    s_combat_data combat_data[4];
    s_missile missiles[300];
    int16_t cleanup_sources[4];
    int16_t cleanup_count;
    int16_t last_retargeted_missile;
}

namespace TEST_STASIS {
    void Reset_() {
        memset(combat_data, 0, sizeof(combat_data));
        memset(missiles, 0, sizeof(missiles));
        memset(cleanup_sources, 0xff, sizeof(cleanup_sources));
        cleanup_count = 0;
        last_retargeted_missile = -1;
    }

    int16_t Last_Cleanup_Source_() {
        return cleanup_count > 0 ? cleanup_sources[cleanup_count - 1] : -1;
    }

    int16_t Last_Retargeted_Missile_() {
        return last_retargeted_missile;
    }
}

namespace TEST_CRYSTAL_FLEET {
    void Reset_() {
        TEST_STASIS::Reset_();
        MOX::_combat_attacker_player = 0;
        MOX::_combat_defender_player = 11;
        MOX::_combat_total_ship_count = 3;
        MOX::_apply_damage_to_planet = 0;
        MOX::_super_fast_flag = 0;
    }

    int16_t Cleanup_Count_() {
        return cleanup_count;
    }

    int16_t Cleanup_Source_(int16_t index) {
        return cleanup_sources[index];
    }
}

namespace MOX {
    s_combat_data* _combat_data = combat_data;
    s_missile* _missile = missiles;
    int16_t _combat_attacker_player;
    int16_t _combat_defender_player;
    int16_t _combat_total_ship_count;
    uint16_t _apply_damage_to_planet;
    int16_t _super_fast_flag;
}

namespace CMBTMIS {
    void __cdecl Retarget_Missile_(int16_t missile_idx) {
        last_retargeted_missile = missile_idx;
    }

    void __cdecl Kill_Missile_(int16_t) {
    }
}

namespace COMBAT1 {
    void __cdecl Remove_Continuing_Effect_Specials_(int16_t source_idx) {
        cleanup_sources[cleanup_count++] = source_idx;
    }
}

namespace CMBTSPEC {
    void __cdecl Destroy_Ship_FX_(int16_t, int16_t, int16_t, int16_t) {
    }
}