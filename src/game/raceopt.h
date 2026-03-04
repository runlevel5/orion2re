#pragma once

namespace raceopt {
    void __cdecl Setup_Evolutionary_Upgrade_(short player_idx);

    inline void __cdecl Evolutionary_Mutation_Screen_(int8_t* traits) {
        using Fn = void(__cdecl*)(int8_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0043A270});
        fn(traits);
    }
}