#pragma once
#include <cstdint>

namespace TECHINIT {
    inline int __cdecl Tech_Is_Legal_For_Player_(int16_t player_idx, int16_t tech_app_idx) {
        using Fn = int(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004FD2B0 });
        return fn(player_idx, tech_app_idx);
    }

    void __cdecl Init_Player_Tech_(int16_t player_idx);
}