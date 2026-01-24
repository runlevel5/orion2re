#pragma once
#include <cstdint>

namespace TECHINIT {
    inline void __cdecl Init_Player_Tech_(int16_t player_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x004fd390 })(player_idx);
    }
}