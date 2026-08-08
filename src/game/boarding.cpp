#include "pch.h"

#include "game/boarding.h"

namespace BOARDING {
    int16_t __cdecl Captured_Marine_Count_(
        int16_t target_idx, int16_t surviving_marines) {
        if (_kill_excess_marines_after_capture == 0) {
            return surviving_marines;
        }

        s_combat_data* target = &MOX::_combat_data[target_idx];
        int16_t hull_class = target->size_class;
        if (target->ship_idx == -1) {
            hull_class += HULL_STAR_BASE - HULL_CRUISER;
        }

        int16_t capacity = TECHDATA::_hull_data[hull_class].marines;
        if (struct_::Test_Bit_Field_(
                target->special_device_flags, SPECIAL_TROOP_PODS) &&
            !struct_::Test_Bit_Field_(
                target->special_device_damage_flags, SPECIAL_TROOP_PODS)) {
            capacity *= 2;
        }

        return surviving_marines > capacity ? capacity : surviving_marines;
    }

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
