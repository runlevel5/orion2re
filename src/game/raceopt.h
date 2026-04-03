#pragma once

namespace raceopt {
    void __cdecl Setup_Evolutionary_Upgrade_(short player_idx);

    inline void __cdecl Evolutionary_Mutation_Screen_(int8_t* traits) {
        using Fn = void(__cdecl*)(int8_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0043A270});
        fn(traits);
    }

    inline int16_t __cdecl Player_Score_Modifier_(int16_t player_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00437990});
        return fn(player_idx);
    }
}