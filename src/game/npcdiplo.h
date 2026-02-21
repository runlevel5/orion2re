#pragma once

#include <cstdint>

namespace npcdiplo {
    inline void __cdecl Clear_Last_Proposal_Turn_(int16_t player_idx1, int16_t player_idx2) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ec4c0});
        fn(player_idx1, player_idx2);
    }

    void __cdecl NPC_Diplomacy_();

    inline void __cdecl NPC_Diplomacy_Buildup_Penalties_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ea9a0});
        fn();
    }

    inline void __cdecl NPC_Threats_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e9fb0});
        fn();
    }

    inline void __cdecl NPC_To_NPC_Treaty_Negotiations_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ea1f0});
        fn();
    }

    inline void __cdecl NoOp3() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004eb7c0});
        fn();
    }

    inline void __cdecl NPC_Declarations_Of_War_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004eab20});
        fn();
    }

    inline void __cdecl NPC_Peace_Negotiations_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004eb500});
        fn();
    }

    inline void __cdecl NPC_Tech_Exchange_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004eb7d0});
        fn();
    }

    inline void __cdecl NPC_Treaty_Hatred_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004EC7B0});
        fn();
    }
}