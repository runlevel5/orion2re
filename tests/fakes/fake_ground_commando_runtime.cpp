#include "pch.h"

namespace {
    s_leader_data leaders[2];
    s_ship_data ships[2];
    int colony_officer;
    int16_t officer_level;
}

namespace test_ground_commando_runtime {
    void Reset_() {
        memset(leaders, 0, sizeof(leaders));
        memset(ships, 0, sizeof(ships));
        colony_officer = -1;
        officer_level = 0;
    }

    s_leader_data* Leader_(int index) {
        return &leaders[index];
    }

    s_ship_data* Ship_(int index) {
        return &ships[index];
    }

    void Set_Colony_Officer_(int leader_idx) {
        colony_officer = leader_idx;
    }

    void Set_Officer_Level_(int16_t level) {
        officer_level = level;
    }
}

namespace MOX {
    s_leader_data* _leaders = leaders;
    s_ship_data* _ship = ships;
}

namespace COLCALC {
    int __cdecl Colony_Officer_(s_colony*) {
        return colony_officer;
    }
}

namespace OFFICER {
    int16_t __cdecl Owned_Officer_Level_(int16_t) {
        return officer_level;
    }
}