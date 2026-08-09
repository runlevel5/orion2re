#include "pch.h"

namespace {
    s_ship_data ships[3];
    s_star_data stars[2];
    s_planet_data planets[1];
}

namespace TEST_FLEETPOP {
    void Reset_() {
        memset(ships, 0, sizeof(ships));
        memset(stars, 0xff, sizeof(stars));
        memset(planets, 0, sizeof(planets));
        memset(MOX::_ship_node, 0xff, sizeof(MOX::_ship_node));
        memset(MOX::_ship_stack_start, 0xff, sizeof(MOX::_ship_stack_start));

        MOX::_PLAYER_NUM = 0;
        MOX::_fleet_box_ship_stack = 0;
        MOX::_ship_stack_start[0] = 0;
        MOX::_ship_node[0].ship_idx = 0;
        MOX::_ship_node[0].next_node = -1;
        MOX::_ship[0].owner = 0;
        MOX::_ship[0].d.ship_type = SHIP_TYPE_OUTPOST;
        MOX::_star[1].colonize_player = 0;
        MOX::_star[1].planet_index[0] = 0;
        MOX::_planet[0].colony_index = -1;
    }
}

namespace MOX {
    int16_t _PLAYER_NUM;
    s_ship_data* _ship = ships;
    s_star_data* _star = stars;
    s_planet_data* _planet = planets;
    s_ship_node _ship_node[MAX_SHIPS];
    int16_t _ship_stack_start[MAX_SHIPS];
    int16_t _fleet_box_ship_stack;
}

namespace HACCESS {
    int16_t __cdecl Ship_Owner_(int16_t ship_idx) {
        return MOX::_ship[ship_idx].owner;
    }
}

namespace HAROLD {
    int8_t __cdecl Planet_Has_Outpost_(int16_t) {
        return 0;
    }
}