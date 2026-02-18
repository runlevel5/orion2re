#pragma once

#include <cstdint>

namespace npcdiplo {
    inline void __cdecl Clear_Last_Proposal_Turn_(int16_t player_idx1, int16_t player_idx2) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ec4c0});
        fn(player_idx1, player_idx2);
    }

    inline void __cdecl NPC_Diplomacy_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004e9f80 });
        fn();
    }
}