#include "pch.h"

static s_colony colonies[2];
static s_planet_data planets[2];
static s_player players[2];
static s_star_data stars[2];
static int16_t pop_limits[10];

namespace TEST_SETTLER {
    void Reset_() {
        memset(colonies, 0, sizeof(colonies));
        memset(planets, 0, sizeof(planets));
        memset(players, 0, sizeof(players));
        memset(stars, 0, sizeof(stars));
        memset(pop_limits, 0, sizeof(pop_limits));
        MOX::_NUM_PLAYERS = 1;
    }

    void Set_Pop_Limit_(int32_t race_idx, int16_t pop_limit) {
        pop_limits[race_idx] = pop_limit;
    }
}

namespace MOX {
    int16_t _NUM_PLAYERS;
    s_colony* _colony = colonies;
    s_planet_data* _planet = planets;
    s_player* _player = players;
    s_star_data* _star = stars;
}

namespace COLCALC {
    int16_t __cdecl Colony_Race_Pop_Limit_(s_colony*, int16_t race_idx) {
        return pop_limits[race_idx];
    }
}

namespace EVENTS {
    int16_t __cdecl Event_Check_Plague_(int16_t) {
        return 0;
    }

    int16_t __cdecl Event_Check_Space_Anomaly_(int16_t) {
        return 0;
    }

    int16_t __cdecl Event_Check_Space_Anomaly_At_Star_(int16_t) {
        return 0;
    }
}

namespace GEO {
    int32_t __cdecl Parsecs_Between_Stars_(int16_t, int16_t) {
        return 0;
    }
}

namespace MSG_ {
    void __cdecl Add_Msg_(s_MSG, int16_t) {
    }
}