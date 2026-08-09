#include "pch.h"

namespace {
    s_colony colonies[1];
    s_player players[1];
}

namespace TEST_TOWN_ZERO {
    void Reset_() {
        memset(colonies, 0, sizeof(colonies));
        memset(players, 0, sizeof(players));
        colonies[0].owner = 0;
        colonies[0].planet = 0;
    }
}

namespace MOX {
    s_colony* _colony = colonies;
    s_player* _player = players;
}

namespace ERICNET {
    void __cdecl Remove_Building_(int16_t colony_idx, int16_t building_idx) {
        MOX::_colony[colony_idx].buildings[building_idx] = 0;
    }
}

namespace game_random {
    uint32_t __cdecl Random_(uint32_t) {
        return 1;
    }
}