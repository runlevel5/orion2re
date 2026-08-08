#include "pch.h"

namespace {
    s_colony colonies[1];
    s_planet_data planets[1];
    s_star_data stars[1];
    s_ship_data ships[1];
    s_player players[MAX_PLAYERS];
}

namespace test::fakes {
    int32_t last_played_sound = -1;
}

namespace MOX {
    int16_t _NUM_COLONIES = 1;
    s_colony* _colony = colonies;
    s_planet_data* _planet = planets;
    s_star_data* _star = stars;
    s_ship_data* _ship = ships;
    s_player* _player = players;
    int16_t _super_fast_flag;
    s_settings _settings = {};
    int8_t _food_per_farmer_table[PLANET_CLIMATE_COUNT];
    uint8_t _planet_max_farms[PLANET_SIZE_COUNT];
    int8_t _planet_max_population[PLANET_SIZE_COUNT];
}

namespace sound {
    int32_t __cdecl Play_Sound_(int32_t resource_id) {
        test::fakes::last_played_sound = resource_id;
        return resource_id;
    }
}

namespace TECHDATA {
    s_tech_building_data _buildings[BUILDING_COUNT];
}

namespace game_random {
    uint32_t __cdecl Random_(uint32_t) {
        return 1;
    }
}

namespace COLBLDG {
    void __cdecl Delete_From_Colony_Queue_(int16_t, int16_t) {}
    void __cdecl Delete_Prod_From_Colony_Queue_(int16_t, int16_t) {}
}

namespace COLCALC {
    void __cdecl Compute_Star_Colony_Stuff_(s_star_data*) {}
    void __cdecl Colony_Environmental_Stuff_(s_colony*) {}
    void __cdecl Colony_Calculation_(int16_t) {}
}

namespace invasion {
    int __cdecl Colony_Tank_Barracks_Limit_(s_colony*) {
        return 0;
    }

    int16_t __cdecl Colony_Infantry_Barracks_Limit_(s_colony*) {
        return 0;
    }

    void __cdecl Assign_New_Home_Planet_(int8_t, int16_t) {}
    void __cdecl Enforce_Population_Limits_At_Colony_(s_colony*) {}
}

namespace SHIPMOVE {
    void __cdecl Update_Ship_ETAs_(int16_t) {}
}