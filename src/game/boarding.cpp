#include "pch.h"

#include "game/boarding.h"

namespace BOARDING {
    e_boarding_filter __cdecl Filter_Boarding_(
        int16_t attacker_idx, int16_t target_idx) {
        const s_combat_data& attacker = MOX::_combat_data[attacker_idx];
        const s_combat_data& target = MOX::_combat_data[target_idx];

        if (target.stasis_source_idx != UINT8_MAX) {
            return e_boarding_filter::target_in_stasis;
        }
        if (_board_in_black_hole == 0) {
            if (target.black_hole_source_idx != UINT8_MAX) {
                return e_boarding_filter::target_in_black_hole;
            }
            if (attacker.black_hole_source_idx != UINT8_MAX) {
                return e_boarding_filter::attacker_in_black_hole;
            }
        }
        return e_boarding_filter::allowed;
    }
} // namespace BOARDING
