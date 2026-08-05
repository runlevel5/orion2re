#include "pch.h"

namespace {
    s_colony colonies[MAX_COLONIES];
    s_planet_data planets[MAX_PLANETS];
    s_star_data stars[MAX_STARS];
    s_ship_data ships[MAX_SHIPS];
    s_player players[MAX_PLAYERS];
}

namespace TEST_COLONIZE {
    void Reset_() {
        memset(colonies, 0, sizeof(colonies));
        memset(planets, 0, sizeof(planets));
        memset(stars, 0xff, sizeof(stars));
        memset(ships, 0, sizeof(ships));
        memset(players, 0, sizeof(players));
        MOX::_NUM_STARS = 1;
        MOX::_NUM_COLONIES = 1;
        MOX::_NUM_SHIPS = 2;
        MOX::_NUM_PLAYERS = 1;
        MOX::_stardate = 0;
    }
}

namespace MOX {
    int16_t _NUM_STARS;
    int16_t _NUM_COLONIES;
    int16_t _NUM_SHIPS;
    int16_t _NUM_PLAYERS;
    int32_t _stardate;
    s_colony* _colony = colonies;
    s_planet_data* _planet = planets;
    s_star_data* _star = stars;
    s_ship_data* _ship = ships;
    s_player* _player = players;
}

namespace AIDATA {
    int32_t __cdecl Uncolonized_Planet_Worth_To_Player_(s_planet_data*, s_player*) {
        return 0;
    }
}

namespace COLCALC {
    void __cdecl Colony_Calculation_(int16_t) {
    }

    void __cdecl Compute_Star_Colony_Stuff_(s_star_data*) {
    }
}

namespace ERICNET {
    void __cdecl Remove_Building_(int16_t, int16_t) {
    }
}

namespace INITGAME {
    void __cdecl Init_Colony_(int16_t, int16_t, uint8_t) {
    }
}

namespace MAINSCR {
    void __cdecl Update_Ship_Icons_In_Between_Turns_() {
    }
}

namespace MSG_ {
    void __cdecl Add_Msg_(s_MSG, int16_t) {
    }
}

namespace REPORT {
    s_colony* __cdecl Player_Colony_Base_At_Star_(int16_t, int16_t) {
        return nullptr;
    }

    s_ship_data* __cdecl Player_Ship_Type_At_Star_(int16_t, int16_t, int16_t, int8_t) {
        return nullptr;
    }
}

namespace SHIPS {
    void __cdecl Kill_Ship_(int16_t) {
    }
}

namespace TECHDATA {
    s_tech_building_data _buildings[BUILDING_COUNT];
}