#pragma once

#include <cstdint>

namespace npcdiplo {
    void __cdecl Clear_Last_Proposal_Turn_(int16_t player_id_1, int16_t player_id_2);

    inline void __cdecl NPC_Proposal_Rejection_Accept_(int16_t a, int16_t b, int32_t status) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ec6a0});
        fn(a, b, status);
    }

    inline int16_t __cdecl NPC_Good_Proposal_Check_(int16_t player_idx, int16_t target_idx) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ec330});
        return fn(player_idx, target_idx);
    }

    inline void __cdecl NPC_Tech_Exchange_Check_(int16_t player_idx, int16_t target_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ec180});
        fn(player_idx, target_idx);
    }

    void __cdecl NPC_Diplomacy_();

    void __cdecl NPC_Diplomacy_Buildup_Penalties_();

    void __cdecl NPC_Threats_();

    void __cdecl NPC_To_NPC_Treaty_Negotiations_();

    void __cdecl Honor_Alliances_(int16_t player_idx, int16_t ally_idx);

    void __cdecl Eliminate_NPC_Treaty_Conflicts_();

    void __cdecl NoOp3();

    void __cdecl NPC_Declarations_Of_War_();

    void __cdecl NPC_Peace_Negotiations_();

    int16_t __cdecl NPC_Surrender_Check_(int16_t player_idx);

    inline void __cdecl Get_Exchange_Tech_List_(int16_t player_num, int16_t ambassador_idx, int16_t* count, uint8_t* target_list, int16_t* exchange_list) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t*, uint8_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004ebbf0 });
        fn(player_num, ambassador_idx, count, target_list, exchange_list);
    }

    void __cdecl NPC_Tech_Exchange_();

    void __cdecl NPC_Treaty_Hatred_();

    inline void __cdecl Get_Differential_Tech_List_(int16_t player_a, int16_t player_b, int32_t mode, int32_t max_value, uint8_t* tech_list, int32_t* tech_cost_list, int16_t* n_techs, int32_t sort_flag) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int32_t, int32_t, uint8_t*, int32_t*, int16_t*, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ebfa0});
        fn(player_a, player_b, mode, max_value, tech_list, tech_cost_list, n_techs, sort_flag);
    }
}