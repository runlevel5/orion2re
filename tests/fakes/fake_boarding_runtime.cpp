#include "pch.h"

#include "game/boarding.h"
#include "game/techdata.h"

namespace {
    s_combat_data combat_data[2];
}

namespace TEST_BOARDING {
    void Reset_() {
        memset(combat_data, 0, sizeof(combat_data));
        for (s_combat_data& ship : combat_data) {
            ship.stasis_source_idx = UINT8_MAX;
            ship.black_hole_source_idx = UINT8_MAX;
        }
        BOARDING::_kill_excess_marines_after_capture = 0;
        BOARDING::_board_in_black_hole = 1;
    }
}

namespace MOX {
    s_combat_data* _combat_data = combat_data;
} // namespace MOX

namespace TECHDATA {
    s_tech_hull_data _hull_data[HULL_CLASS_COUNT];
}
