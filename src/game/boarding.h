#pragma once
#include <cstdint>

namespace BOARDING {
    enum class e_boarding_filter : uint8_t {
        allowed,
        target_in_stasis,
        target_in_black_hole,
        attacker_in_black_hole,
    };

    inline uint8_t _telepathic_multiple_capture = 0;
    inline uint8_t _kill_excess_marines_after_capture = 0;
    inline uint8_t _board_in_black_hole = 1;

    int16_t __cdecl Captured_Marine_Count_(
        int16_t target_idx, int16_t surviving_marines);

    e_boarding_filter __cdecl Filter_Boarding_(
        int16_t attacker_idx, int16_t target_idx);
} // namespace BOARDING
