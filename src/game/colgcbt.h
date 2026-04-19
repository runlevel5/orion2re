#pragma once
#include <cstdint>

namespace COLGCBT {
    inline s_mouse_list &_mouse_list_blank = *reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x005583A8});

    inline void __cdecl Colony_Combat_Screen_(int info_ptr, int defender_info, int32_t ground_strength, int16_t colony_idx) {
        using Fn = void(__cdecl*)(int, int, int32_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004a2830});
        fn(info_ptr, defender_info, ground_strength, colony_idx);
    }
}