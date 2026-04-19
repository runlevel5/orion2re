#pragma once
#include <cstdint>

namespace TECHINIT {
    inline void __cdecl Init_Tech_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004fd020});
        fn();
    }
    int16_t __cdecl Tech_Is_Legal_For_Player_(int16_t player_idx, int16_t tech_id);

    void __cdecl Init_Player_Tech_(int16_t player_idx);
}