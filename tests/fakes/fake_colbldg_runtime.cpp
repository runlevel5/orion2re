#include "pch.h"

namespace {
    s_colony colonies[3];
    s_planet_data planets[5];
    s_star_data stars[1];
}

namespace TEST_COLBLDG {
    void Reset_() {
        memset(colonies, 0, sizeof(colonies));
        memset(planets, 0, sizeof(planets));
        memset(stars, 0xff, sizeof(stars));
        COLONY::_colony_handle = 0;
    }
}

namespace MOX {
    s_colony* _colony = colonies;
    s_planet_data* _planet = planets;
    s_star_data* _star = stars;
}

namespace COLONY {
    int16_t _colony_handle;
}