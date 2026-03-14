#include "pch.h"

namespace CMBTFIRE {
    // Suggested name: Does_Combat_Planet_Have_Defenses
    int16_t __cdecl Does_Combat_Planet_Have_Defenses_() {
        if (MOX::_cmbt_colony_id != -1) {
            // Pack colony defense buildings into a bitfield:
            // bit 3: Fighter Garrison
            // bit 2: Stellar Converter
            // bit 1: Ground Batteries
            // bit 0: Missile Base
            return (((int16_t)MOX::_colony[MOX::_cmbt_colony_id].buildings[BUILDING_FIGHTER_GARRISON] * 2 |
                     (int16_t)MOX::_colony[MOX::_cmbt_colony_id].buildings[BUILDING_STELLAR_CONVERTER]) * 2 |
                    (int16_t)MOX::_colony[MOX::_cmbt_colony_id].buildings[BUILDING_GROUND_BATTERIES]) * 2 |
                    (int16_t)MOX::_colony[MOX::_cmbt_colony_id].buildings[BUILDING_MISSILE_BASE];
        }
        return 0;
    }

    int16_t __cdecl Is_Combat_Ship_Dead_(int16_t ship_idx) {
        int16_t result = 0;

        if (ship_idx != 0) {
            s_combat_data* ship = &MOX::_combat_data[ship_idx];

            if (ship->structure_damage >= ship->structure_max) {
                result = 1;
            }
            else if (ship->drive_current_hits == 0 && ship->owner <= 9 && ship->ftl_type > 0) {
                result = 3;
            }

            if (result != 0) {
                if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 0x18 /* SPECIAL_STASIS_FIELD */)) {
                    result = 4;
                }
            }
        }

        return result;
    }
}