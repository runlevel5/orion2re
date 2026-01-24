#pragma once
#include "deps.h"

namespace HACCESS {
    inline void __cdecl Set_Colony_Auto_Select_For_Star_(int16_t star_idx, int16_t player_num, int16_t* out_val) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049AF30});
        fn(star_idx, player_num, out_val);
    }
}