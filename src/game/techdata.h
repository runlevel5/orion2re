#pragma once
#include <cstdint>

namespace TECHDATA {
#pragma pack(push, 1)
    struct s_fuel_info {
        int16_t tech_index;
        int16_t ship_range;
        int16_t unknown_04;
        int16_t unknown_06;
        int16_t unknown_08;
    };
#pragma pack(pop)
    inline s_fuel_info* _fuel = reinterpret_cast<s_fuel_info*>(std::uintptr_t{ 0x0055575E });

    #pragma pack(push, 1)
    struct s_building_data {
        uint8_t filler[8];
        int32_t cost; // 0x8
        uint8_t filler2[37];
    };
    #pragma pack(pop)

    // _buildings[11] corresponds to Outpost
    inline s_building_data* _buildings = reinterpret_cast<s_building_data*>(std::uintptr_t{0x005542f0});
}