#include "pch.h"

namespace {
    s_planet_data planets[MAX_PLANETS];
    s_ship_data ships[MAX_SHIPS];
}

namespace TEST_SHIPMOVE {
    void Reset_() {
        memset(planets, 0, sizeof(planets));
        memset(ships, 0, sizeof(ships));
    }
}

namespace MOX {
    s_planet_data* _planet = planets;
    s_ship_data* _ship = ships;
}