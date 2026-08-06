#include "pch.h"

#include "game/boarding.h"

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
        BOARDING::_board_in_black_hole = 1;
    }
}

namespace MOX {
    s_combat_data* _combat_data = combat_data;
} // namespace MOX
