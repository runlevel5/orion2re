#pragma once
#include <cstdint>

namespace TECHDATA {
#pragma pack(push, 1)
    struct s_fuel_info {
        int16_t ship_range;
        int16_t unknown_02;
        int16_t unknown_04;
        int16_t unknown_06;
        int16_t unknown_08;
    };
#pragma pack(pop)
    inline s_fuel_info* _fuel = reinterpret_cast<s_f
}