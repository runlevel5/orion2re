#include "pch.h"

namespace {
    s_player players[MAX_PLAYERS];
}

namespace TEST_COLCALC_BASE {
    void Reset_() {
        memset(players, 0, sizeof(players));
    }
}

namespace MOX {
    s_player* _player = players;
    int8_t _food_per_farmer_table[PLANET_CLIMATE_COUNT] = {};
}

namespace COLCALC {
    int16_t __cdecl Colony_Food2_Per_Farmer_(const s_colony* colony, int16_t, s_colony_job_production*) {
        return (uint8_t)colony->food2_per_farmer;
    }

    int16_t __cdecl Colony_Industry_Per_Worker_(const s_colony* colony, int16_t, s_colony_job_production*) {
        return (uint8_t)colony->industry_per_worker;
    }

    int16_t __cdecl Colony_Research_Per_Scientist_(const s_colony* colony, int16_t, s_colony_job_production*) {
        return (uint8_t)colony->research_per_scientist;
    }
}