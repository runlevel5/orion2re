#include "pch.h"

namespace {
    s_colony colonies[1];
    s_planet_data planets[1];
    s_player players[1];
}

namespace TEST_COLONY_POP_GROWTH {
    void Reset_() {
        memset(colonies, 0, sizeof(colonies));
        memset(planets, 0, sizeof(planets));
        memset(players, 0, sizeof(players));
        COLCALC::_housing_creates_no_farmers = 0;
    }
}

namespace MOX {
    int16_t _NUM_COLONIES = 1;
    int16_t _NUM_PLAYERS = 1;
    s_colony* _colony = colonies;
    s_planet_data* _planet = planets;
    s_player* _player = players;
    int8_t _planet_max_population[PLANET_SIZE_COUNT] = {5, 10, 15, 20, 25};
}

namespace EVENTS {
    int16_t __cdecl Event_Check_Space_Anomaly_(int16_t) {
        return 0;
    }

    int16_t __cdecl Event_Check_Colony_Researching_(int16_t) {
        return 0;
    }
}

namespace MSG_ {
    void __cdecl Add_Msg_(s_MSG, int16_t) {
    }
}

namespace RUSS {
    void __cdecl Shuffle_Sint_(int16_t*, int16_t) {
    }

    int16_t __cdecl Get_Weighted_Choice_Int_(uint32_t*, int16_t) {
        return 1;
    }
}

namespace game_random {
    uint32_t __cdecl Random_(uint32_t) {
        return 1;
    }
}