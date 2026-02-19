#pragma once

namespace aimove {
    inline void __cdecl Move_All_AI_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004658a0 });
        fn();
    }

    inline void __cdecl Find_Opportunity_Attack_(int16_t player_idx, int16_t target_idx, int16_t threshold, int16_t* out_planet, int16_t* out_value, int32_t* out_ratio) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t*, int16_t*, int32_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00465E00 });
        fn(player_idx, target_idx, threshold, out_planet, out_value, out_ratio);
    }
}