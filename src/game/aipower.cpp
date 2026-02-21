#include "deps.h"
#include "aipower.h"

namespace aipower {
    int16_t __cdecl Get_Best_Ship_Shield_Value_(int16_t player_idx) {
        if (player_idx >= 8) {
            switch (player_idx) {
                case 8:
                    return 6;
                case 9:
                    return 8;
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                    return 0;
                default:
                    // Assembly at 0x00509451 returns uninitialized value from stack
                    int16_t uninitialized_ret = 0;
                    return uninitialized_ret;
            }
        }

        int16_t shield_idx = INITSHIP::Best_Ship_Shield_(player_idx);
        return TECHDATA::_shields[shield_idx].points_blocked;
    }

    /**
     * Calculates the combat strength of an attacking ship against a target ship by first 
     * determining the target's best shield value and then calculating strength vs that shield.
     */
    int16_t __cdecl Ship_Strength_Vs_Ships_(int16_t attacker_idx, int16_t target_idx) {
        int16_t shield_value = Get_Best_Ship_Shield_Value_(target_idx);
        return Ship_Strength_Vs_Shield_(attacker_idx, target_idx, shield_value);
    }
}