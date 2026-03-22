#pragma once

namespace spy {
    inline void __cdecl Compute_Spy_Bonuses_(s_player* player, int16_t* spy_bonus, int16_t* agent_bonus) {
        using Fn = void(__cdecl*)(s_player*, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004B8450});
        fn(player, spy_bonus, agent_bonus);
    }

    inline void __cdecl Resolve_Spies_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004b8be0 });
        fn();
    }

    inline void __cdecl Bring_Home_Spies_Vs_(int16_t target_player_idx, int16_t player_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b97c0});
        fn(target_player_idx, player_idx);
    }
}