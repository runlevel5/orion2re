#pragma once
#include <cstdint>

namespace BOARDING {
    enum class e_boarding_filter : uint8_t {
        allowed,
        target_in_stasis,
        target_in_black_hole,
        attacker_in_black_hole,
    };

    inline uint8_t _board_in_black_hole = 1;

    e_boarding_filter __cdecl Filter_Boarding_(
        int16_t attacker_idx, int16_t target_idx);
} // namespace BOARDING
