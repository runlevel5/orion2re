#include "pch.h"

#include "game/cmbtmis.h"
#include "game/combat1.h"

namespace {
    s_combat_data combat_data[2];
    s_missile missiles[300];
    int16_t last_cleanup_source;
    int16_t last_retargeted_missile;
}

namespace TEST_STASIS {
    void Reset_() {
        memset(combat_data, 0, sizeof(combat_data));
        memset(missiles, 0, sizeof(missiles));
        last_cleanup_source = -1;
        last_retargeted_missile = -1;
    }

    int16_t Last_Cleanup_Source_() {
        return last_cleanup_source;
    }

    int16_t Last_Retargeted_Missile_() {
        return last_retargeted_missile;
    }
}

namespace MOX {
    s_combat_data* _combat_data = combat_data;
    s_missile* _missile = missiles;
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
        last_cleanup_source = source_idx;
    }
}