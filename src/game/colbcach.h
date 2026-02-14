#pragma once
#include <cstdint>

struct s_animation_header;

namespace COLBCACH {
    inline s_animation_header* __cdecl Cache_Load_Bldg_(uint16_t col, uint16_t row, int32_t bldg_id) {
        using Fn = s_animation_header*(__cdecl*)(uint16_t, uint16_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00424f30});
        return fn(col, row, bldg_id);
    }
}