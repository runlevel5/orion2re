#pragma once

struct s_leader_data;

namespace spy {
    extern int8_t _lookup_table[8];

    int16_t __cdecl Tech_Spy_Bonus_(s_player* player);

    void __cdecl Compute_Spy_Bonuses_(s_player* player, int16_t* defense_bonus_out, int16_t* attack_bonus_out);

    void __cdecl Resolve_Spies_();

    extern int16_t _spy_bonuses[16];

    void __cdecl Resolve_Assassins_();

    void __cdecl Leader_Assassinates_(s_leader_data* leader);

    void __cdecl Allocate_AI_Spies_(s_player* player);

    void __cdecl Resolve_Player_Spies_(s_player* player);

    void __cdecl Bring_Home_Spies_Vs_(int16_t player_idx, int16_t enemy_idx);

    int32_t __cdecl N_Spies_Bonus_(int32_t spy_count);

    void __cdecl Steal_App_(s_player* attacker, s_player* victim, int8_t frame_flag);

    bool __cdecl Frame_Another_(int16_t player_idx_a, int16_t player_idx_b, int16_t* framed_player_idx, int32_t tech_app_id);

    void __cdecl Destroy_Random_Building_(int16_t framed_player_idx, int16_t victim_player_idx, int acting_player_idx);
}