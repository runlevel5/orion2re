#pragma once

#include <cstdint>

namespace aimove {
    inline void __cdecl Move_AI_(int32_t plr_idx) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00461540 });
        fn(plr_idx);
    }

    inline void __cdecl Move_NPCs_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00465910 });
        fn();
    }

    void __cdecl Move_All_AI_();

    inline void __cdecl Player_Threatens_Player_(int16_t p1, int16_t p2, int* out_planet_id, int* out_threat_val, int* out_extra) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int*, int*, int*);
        reinterpret_cast<Fn>(std::uintptr_t{0x00465f70})(p1, p2, out_planet_id, out_threat_val, out_extra);
    }

    inline int16_t __cdecl Best_Uncolonized_Undestined_Planet_(int16_t player_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00461000});
        return fn(player_idx);
    }

    inline void __cdecl Find_Opportunity_Attack_(int16_t player_idx, int16_t target_idx, int16_t threshold, int16_t* out_planet, int16_t* out_value, int32_t* out_ratio) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t*, int16_t*, int32_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00465E00 });
        fn(player_idx, target_idx, threshold, out_planet, out_value, out_ratio);
    }
}