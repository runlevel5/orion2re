#include "pch.h"

namespace {
    s_colony colonies[1];
}

namespace TEST_COLONY_REPLICATORS {
    void Reset_() {
        memset(colonies, 0, sizeof(colonies));
        memset(TECHDATA::_buildings, 0, sizeof(TECHDATA::_buildings));
        TECHDATA::_buildings[BUILDING_AUTOMATED_FACTORY].maintenance = 2;
        COLCALC::_replicator_food_pp = 2;
        COLCALC::_replicator_food_bc = 1;
    }
}

namespace MOX {
    s_colony* _colony = colonies;
}

namespace TECHDATA {
    s_tech_building_data _buildings[BUILDING_COUNT];
}

namespace EVENTS {
    int16_t __cdecl Event_Check_Space_Anomaly_(int16_t) {
        return 0;
    }
}