#pragma once
#include <cstdint>

namespace global {
    inline const char* s_Intruder_005576a0 =
        reinterpret_cast<const char*>(std::uintptr_t{0x005576A0});

    struct s_tech_drive_data {
        uint8_t combat_speed_modifier[7];
        uint8_t padding[39];
    };

    inline s_tech_drive_data* _drives =
        reinterpret_cast<s_tech_drive_data*>(std::uintptr_t{0x00555600});
}