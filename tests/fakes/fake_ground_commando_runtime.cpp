#include "pch.h"

namespace {
    s_leader_data leaders[2];
    s_player players[2];
    s_colony colonies[1];
    s_ship_data ships[2];
    int colony_officer;
}

namespace test_ground_commando_runtime {
    void Reset_() {
        memset(leaders, 0, sizeof(leaders));
        memset(players, 0, sizeof(players));
        memset(colonies, 0, sizeof(colonies));
        memset(ships, 0, sizeof(ships));
        colony_officer = -1;
    }

    s_colony* Colony_() {
        return &colonies[0];
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
        int leader_idx = colony_officer >= 0 ? colony_officer : 0;
        leaders[leader_idx].xp = level_thresholds[level];
    }
}

namespace MOX {
    s_leader_data* _leaders = leaders;
    s_player* _player = players;
    s_colony* _colony = colonies;
    s_ship_data* _ship = ships;
    s_settings _settings = {};
}

namespace TECHDATA {
    s_tech_armor_data _armor[ARMOR_COUNT] = {};
    s_tech_data_rifle _rifles[6] = {};
    s_tech_data_personal_shield _personal_shields[2] = {};
}

namespace aitech {
    int32_t __cdecl Player_Best_Armor_(s_player*) {
        return 0;
    }

    int32_t __cdecl Player_Best_Rifle_(s_player*) {
        return 0;
    }

    int32_t __cdecl Player_Best_Personal_Shield_(s_player*) {
        return 0;
    }
}

namespace COLCALC {
    int __cdecl Colony_Officer_(s_colony*) {
        return colony_officer;
    }
}
