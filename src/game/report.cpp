#include "deps.h"

namespace REPORT {

    s_colony* __cdecl Player_Colony_Base_At_Star_(short player_idx, short star_idx) {
        for (int i = 0; i < 5; ++i) {
            short planet_idx = MOX::_star[star_idx].planet_index[i];
            
            if (planet_idx != -1) {
                short colony_idx = MOX::_planet[planet_idx].colony_index;
                
                if (colony_idx != -1) {
                    s_colony* colony = &MOX::_colony[colony_idx];
                    
                    if (colony->owner == player_idx && colony->buildings[11] > 0) {
                        return colony;
                    }
                }
            }
        }

        return nullptr;
    }

    s_ship_data* __cdecl Player_Ship_Type_At_Star_(short owner, short ship_type, short location, char include_just_built) {
        s_ship_data* ship = MOX::_ship;
        int num_ships = (int)MOX::_NUM_SHIPS;
        s_ship_data* end = &ship[num_ships];

        while (ship < end) {
            if ((short)(int8_t)ship->d.ship_type == ship_type &&
                (short)ship->owner == owner &&
                ship->location == location &&
                ship->status < 5) {

                if (include_just_built != 0 || ship->just_built == 0) {
                    return ship;
                }
            }
            ship++;
        }

        return nullptr;
    }

}