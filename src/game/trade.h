#pragma once
#include <cstdint>

struct s_player;

namespace TRADE {
    inline void __cdecl Process_Trade_Agreement_(s_player* player, int16_t other_player_idx) {
        using Fn = void(__cdecl*)(s_player*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00466430});
        fn(player, other_player_idx);
    }

    inline void __cdecl Process_Research_Agreement_(s_player* player, int16_t other_player_idx) {
        using Fn = void(__cdecl*)(s_player*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00466600});
        fn(player, other_player_idx);
    }

    inline void __cdecl Compute_N_Trade_And_Research_Pops_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004662f0});
        fn();
    }

    void __cdecl Process_Trade_And_Research_Agreements_();
    int16_t __cdecl Base_Trade_Agreement_Goal_(s_player* player, int16_t player_idx);
    int16_t __cdecl Trade_Agreement_Goal_(s_player* player, int16_t player_idx);
}