#pragma once
#include <cstdint>

namespace CMBTSPEC {
    inline void __cdecl Draw_Cloak_(int16_t ship_idx, int32_t cloak_type) {
        using Fn = void(__cdecl*)(int16_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004A6960});
        fn(ship_idx, cloak_type);
    }
}