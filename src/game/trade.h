#pragma once
#include <cstdint>

struct s_player;

namespace TRADE {
    void __cdecl Start_Trade_Agreement_(s_player* player_a, int16_t player_b_idx);

    void __cdecl Start_Research_Agreement_(s_player* player_a, int16_t player_b_idx);

    void __cdecl Process_Trade_Agreement_(s_player* plr, int16_t target_plr_idx);

    void __cdecl Process_Research_Agreement_(s_player* plr, int16_t target_plr_idx);

    void __cdecl Compute_N_Trade_And_Research_Pops_();

    void __cdecl Process_Trade_And_Research_Agreements_();
    int16_t __cdecl Base_Trade_Agreement_Goal_(s_player* player, int16_t player_idx);
    int16_t __cdecl Trade_Agreement_Goal_(s_player* player, int16_t player_idx);
    int32_t __cdecl Get_Combined_Research_Points_(s_player* plr, int16_t target_plr_idx);
    int32_t __cdecl Research_Agreement_Goal_(s_player* plr, int16_t target_plr_idx);
}