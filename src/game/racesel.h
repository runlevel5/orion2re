#pragma once
#include <cstdint>

namespace RACESEL {
    inline void**& _race_specials = *reinterpret_cast<void***>(std::uintptr_t{ 0x005A5730 });
    inline uint8_t* _color_flag = reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A56E8});
    inline uint8_t* _race_flag = reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A5770});
}