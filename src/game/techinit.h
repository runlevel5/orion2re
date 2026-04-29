#pragma once
#include <cstdint>

namespace TECHINIT {
    void __cdecl Init_Tech_();
    inline void __cdecl Load_Tech_Names_() {
        using Fn = void (__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004FCC40});
        fn();
    }
    int16_t __cdecl Tech_Is_Legal_For_Player_(int16_t player_idx, int16_t tech_id);

    void __cdecl Init_Player_Tech_(int16_t player_idx);
}