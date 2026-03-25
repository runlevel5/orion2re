#pragma once

namespace settler {
    inline int16_t __cdecl N_Freighters_Player_Can_Scrap_(int16_t player_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(0x0043c0c0)(player_idx);
    }

    inline void __cdecl Move_Settlers_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0043be10 });
        fn();
    }
}