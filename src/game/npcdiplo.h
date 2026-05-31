#pragma once

#include <cstdint>

namespace npcdiplo {
    static_assert(sizeof(s_npcdiplo_tech) == 5, "s_npcdiplo_tech size mismatch");

    extern s_npcdiplo_tech _human_tech[212];

    extern s_npcdiplo_tech _target_tech[212];

    int16_t __cdecl Get_Tech_Exchange_Reaction_(int16_t player_a, int16_t player_b, int16_t param_3);

    int __cdecl compare_tech_(const void* a, const void* b);

    int32_t __cdecl Get_Tech_Exchange_Expectation_(int16_t player_idx, int16_t target_idx);

    int __cdecl reverse_compare_tech_(const void* a, const void* b);

    void __cdecl Clear_Last_Proposal_Turn_(int16_t player_id_1, int16_t player_id_2);

    void __cdecl NPC_Proposal_Rejection_Accept_(int16_t player_idx, int16_t other_player_idx, int8_t accepted);

    bool __cdecl NPC_Good_Proposal_Check_(int16_t npc_idx, int16_t player_idx);

    void __cdecl NPC_Tech_Exchange_Check_(int16_t player_idx, int16_t target_player_idx);

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

    void __cdecl Get_Exchange_Tech_List_(int16_t player_a, int16_t player_b, int16_t* out_count, uint8_t* out_tech_index, uint8_t* out_human_tech_index);

    void __cdecl NPC_Tech_Exchange_();

    void __cdecl NPC_Treaty_Hatred_();

    void __cdecl Get_Differential_Tech_List_(
        int16_t player_idx,
        int16_t other_player_idx,
        uint32_t min_tech_value,
        uint32_t max_tech_value,
        uint8_t* tech_list_indices,
        int32_t* tech_list_values,
        int16_t* tech_count,
        int16_t sort_flag
    );

    void __cdecl NPC_Message_Rejection_Check_(int16_t player_idx, int16_t target_idx, int16_t* msg_type);
}