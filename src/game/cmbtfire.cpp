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
}