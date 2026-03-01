#pragma once
#include <cstdint>

namespace paragrph {
    inline uint16_t& _exclusion_count = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055BD24});
    inline int16_t* _exclusion_area_y1 = reinterpret_cast<int16_t*>(std::uintptr_t{0x0057E3E0});
    inline int16_t* _exclusion_area_y2 = reinterpret_cast<int16_t*>(std::uintptr_t{0x0057E3E8});
    inline int16_t* _exclusion_area_x1 = reinterpret_cast<int16_t*>(std::uintptr_t{0x0057E3F0});
    inline int16_t* _exclusion_area_x2 = reinterpret_cast<int16_t*>(std::uintptr_t{0x0057E3F8});
}