#include "pch.h"

namespace {
    s_leader_data leaders[2];
    s_player players[2];
    s_ship_data ships[2];
    int colony_officer;
}

namespace test_ground_commando_runtime {
    void Reset_() {
        memset(leaders, 0, sizeof(leaders));
        memset(players, 0, sizeof(players));
        memset(ships, 0, sizeof(ships));
        colony_officer = -1;
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
        const int16_t level_thresholds[] = {0, 60, 150, 300, 500, 1000};
        leaders[0].xp = level_thresholds[level];
    }
}

namespace MOX {
    s_leader_data* _leaders = leaders;
    s_player* _player = players;
    s_ship_data* _ship = ships;
}

namespace COLCALC {
    int __cdecl Colony_Officer_(s_colony*) {
        return colony_officer;
    }
}
