#pragma once

namespace COLBOMB {
    inline void __cdecl Colony_Bombing_Screen_(s_combat_bomb_result* bomb) {
        using Fn = void(__cdecl*)(s_combat_bomb_result*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044b710});
        fn(bomb);
    }
}