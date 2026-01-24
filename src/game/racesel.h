#pragma once
#include <cstdint>

namespace RACESEL {
    inline void**& _race_specials = *reinterpret_cast<void***>(std::uintptr_t{ 0x005A5730 });
}