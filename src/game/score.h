#pragma once
#include <cstdint>

namespace SCORE {
    inline void __cdecl End_Of_Game_Hi_Score_(int16_t player_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004e4290})(player_idx);
    }

    inline void __cdecl Hall_Of_Fame_Screen_(int16_t value) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004e3880})(value);
    }

    inline int16_t __cdecl Do_Calculate_Hi_Score_(int16_t player_num) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004E2170})(player_num);
    }
}