#include "pch.h"

namespace {
    s_player players[MAX_PLAYERS];
    int32_t breakthrough_chance;
    int32_t research_cost;
    uint32_t random_value;
    int give_player_field_calls;
}

namespace TEST_COLCALC_BASE {
    void Reset_() {
        memset(players, 0, sizeof(players));
        breakthrough_chance = 0;
        research_cost = 0;
        random_value = 100;
        give_player_field_calls = 0;
        COLCALC::_keep_research_on_breakthrough = 0;
        COLCALC::_keep_research_on_finds = 0;
    }

    void Set_Breakthrough_(int32_t chance, uint32_t roll, int32_t cost) {
        breakthrough_chance = chance;
        random_value = roll;
        research_cost = cost;
    }

    int Give_Player_Field_Calls_() {
        return give_player_field_calls;
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

    int32_t __cdecl Chance_For_Research_Breakthrough_(s_player*) {
        return breakthrough_chance;
    }

    void __cdecl Give_Player_Field_(s_player*, int, char) {
        give_player_field_calls++;
    }

    int __cdecl Player_Research_Cost_(s_player*, int16_t) {
        return research_cost;
    }
}

namespace game_random {
    uint32_t __cdecl Random_(uint32_t) {
        return random_value;
    }
}