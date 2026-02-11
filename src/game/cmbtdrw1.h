#pragma once
#include <cstdint>

namespace CMBTDRW1 {
    inline void __cdecl Detailed_View_Ship_(int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004D3C10 });
        fn(ship_idx);
    }
}