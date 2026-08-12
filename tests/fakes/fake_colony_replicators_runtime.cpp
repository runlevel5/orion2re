#include "pch.h"

namespace {
    s_colony colonies[1];
    s_player players[MAX_PLAYERS];
    s_ship_data ships[1];
    s_leader_data leaders[MAX_LEADERS];
    s_planet_data planets[1];
    s_star_data stars[1];
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

namespace TEST_PLAYER_STATS {
    void Reset_() {
        memset(players, 0, sizeof(players));
        memset(ships, 0, sizeof(ships));
        memset(leaders, 0, sizeof(leaders));
        memset(planets, 0, sizeof(planets));
        memset(stars, 0, sizeof(stars));
        memset(&MOX::_settings, 0, sizeof(MOX::_settings));
        for (s_leader_data& leader : leaders) {
            leader.status = -1;
        }
        MOX::_NUM_PLAYERS = 1;
        MOX::_NUM_COLONIES = 0;
        MOX::_NUM_SHIPS = 1;
        const uint8_t defaults[SHIP_SIZE_COUNT] = {1, 2, 4, 8, 16, 32};
        memcpy(COLCALC::scout_lab_research_bonus, defaults, sizeof(defaults));
    }
}

namespace MOX {
    s_colony* _colony = colonies;
    s_player* _player = players;
    s_ship_data* _ship = ships;
    s_leader_data* _leaders = leaders;
    s_planet_data* _planet = planets;
    s_star_data* _star = stars;
    s_settings _settings = {};
    int16_t _NUM_PLAYERS = 0;
    int16_t _NUM_COLONIES = 0;
    int16_t _NUM_SHIPS = 0;
}

namespace TECHDATA {
    s_tech_building_data _buildings[BUILDING_COUNT];
    s_tech_hull_data _hull_data[HULL_CLASS_COUNT];
    s_tech_app_data _technology_applications[TECH_APP_COUNT];
}

namespace EVENTS {
    int16_t __cdecl Event_Check_Space_Anomaly_(int16_t) {
        return 0;
    }

    int16_t __cdecl Event_Check_Space_Anomaly_At_Star_(int16_t) {
        return 0;
    }

    int16_t __cdecl Event_Check_Colony_Researching_(int16_t) {
        return 0;
    }
}

namespace struct_ {
    int16_t __cdecl Test_Bit_Field_(void* bit_field, uint16_t bit_index) {
        uint8_t* bytes = (uint8_t*)bit_field;
        return (bytes[bit_index >> 3] >> (bit_index & 7)) & 1;
    }
}

namespace OFFICER {
    int16_t __cdecl Owned_Officer_Level_(int16_t) {
        return 0;
    }

    int16_t __cdecl Officer_Maintenance_(int16_t, int16_t) {
        return 0;
    }
}

namespace BILL {
    int16_t __cdecl Get_Their_Spy_Number_(int16_t, int16_t) {
        return 0;
    }
}

namespace STRBUILD {
    void Sprintf(s_builder*, const char*, ...) {
    }
}