#pragma once
#include <cstdint>

namespace global {
    inline const char* s_Intruder_005576a0 =
        reinterpret_cast<const char*>(std::uintptr_t{0x005576A0});

    inline s_tech_drive_data* _drives =
        reinterpret_cast<s_tech_drive_data*>(std::uintptr_t{0x005555E0});

    inline uint8_t (&_max_farms_by_size)[10] = *reinterpret_cast<uint8_t(*)[10]>(std::uintptr_t{0x00559660});
    inline uint8_t (&_max_pop_by_size)[10] = *reinterpret_cast<uint8_t(*)[10]>(std::uintptr_t{0x00559670});
}