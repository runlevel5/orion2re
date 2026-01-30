#pragma once
#include <cstdint>

namespace animate {
    inline uint16_t& _clipping_flag = *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x0055B0E0 });

    void __cdecl Set_Animation_Frame_(void* seg, int32_t frame);
    void __cdecl Reset_Animation_Frame_(void* seg);
    int16_t __cdecl Get_Animation_Frame_Count_(void* seg);

    inline void __cdecl Draw_(int32_t x, int32_t y, void* resource) {
        using Fn = void(__cdecl*)(int32_t, int32_t, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00524490});
        fn(x, y, resource);
    }
}