#pragma once

#include <cstdint>

namespace diplodef {
    inline int16_t& _next_council_meeting_turn = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059600C});
    inline int16_t& _COUNCIL_FLAG = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059600E});
    inline int16_t& _council_winner = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596010});
}