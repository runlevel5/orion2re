#include "pch.h"

namespace {
    s_colony colonies[2];
    s_planet_data planet_storage[2];
    s_player players[1];
    s_star_data stars[1];
    s_ship_data ships[1];
    int32_t destroy_colony_reason;
}

namespace TEST_COLONY_POP_GROWTH {
    void Reset_() {
        memset(colonies, 0, sizeof(colonies));
        memset(planet_storage, 0, sizeof(planet_storage));
        memset(players, 0, sizeof(players));
        memset(stars, 0, sizeof(stars));
        memset(ships, 0, sizeof(ships));
        memset(TECHDATA::_buildings, 0, sizeof(TECHDATA::_buildings));
        planet_storage[0].colony_index = 1;
        colonies[1].buildings[BUILDING_CAPITOL] = 1;
        TECHDATA::_buildings[BUILDING_CAPITOL].tech_app = TECH_APP_CAPITOL;
        destroy_colony_reason = -1;
        COLCALC::_housing_creates_no_farmers = 0;
        COLCALC::_planet_extinction = 0;
        COLCALC::_zero_colony_lost = 0;
    }

    int32_t Destroy_Colony_Reason_() {
        return destroy_colony_reason;
    }
}

namespace MOX {
    int16_t _NUM_COLONIES = 1;
    int16_t _NUM_PLAYERS = 1;
    s_colony* _colony = colonies;
    s_planet_data* _planet = &planet_storage[1];
    s_player* _player = players;
    s_star_data* _star = stars;
    s_ship_data* _ship = ships;
    int8_t _planet_max_population[PLANET_SIZE_COUNT] = {5, 10, 15, 20, 25};
}

namespace TECHDATA {
    s_tech_building_data _buildings[BUILDING_COUNT] = {};
    s_tech_field_data _technology_fields[TECH_FIELD_COUNT] = {};
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

namespace BOMB {
    void __cdecl Destroy_Colony_(s_colony*, int32_t capturing_player) {
        destroy_colony_reason = capturing_player;
    }
}

namespace ERICNET {
    void __cdecl Remove_Building_(int16_t, int16_t) {
    }
}

namespace game_random {
    uint32_t __cdecl Random_(uint32_t) {
        return 1;
    }
}