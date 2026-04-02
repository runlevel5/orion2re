#pragma once
#include <cstdint>

namespace _antarans {
    inline int32_t& invasion_stardate =
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x0059BA5D});

    inline uint8_t& attack_notified_mask =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0059BA61});
}