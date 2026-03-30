#pragma once

namespace diplomac {
    inline int16_t& _diplomacy_proposal_system_bribe =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A778C});

    inline uint8_t& _diplomacy_proposal_tech_bribe1 =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A778E});

    inline int16_t& _diplomacy_proposal_gold_bribe =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A7790});

    inline uint8_t& _diplomacy_proposal_tech_bribe2 =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A7792});

    inline uint8_t* _unprovoked_message_mapping = reinterpret_cast<uint8_t*>(std::uintptr_t{0x00453734});

    inline int16_t* _starting_relations = reinterpret_cast<int16_t*>(std::uintptr_t{0x00556B40});

    inline int16_t (&_personality_peace_duration)[12] = 
        *reinterpret_cast<int16_t(*)[12]>(std::uintptr_t{0x00556CC8});

    inline int16_t* _personality_break_peace_treaty_chance = reinterpret_cast<int16_t*>(std::uintptr_t{0x00556CE0});

    void Init_Diplomatic_Relations_();

    void __cdecl Change_Relations_(int16_t amount, int16_t p_a, int16_t p_b, int16_t incident, int16_t system, int16_t value);

    void __cdecl Declare_War_(int16_t player_idx, int16_t target_idx);

    void __cdecl Declare_Peace_(int16_t p1, int16_t p2);

    int __cdecl Fleet_Comparison_(int16_t player_idx, int16_t opponent_idx);

    void __cdecl Break_Treaties_(int16_t initiator_idx, int16_t target_idx);

    void __cdecl Break_Trade_(int16_t p1_idx, int16_t p2_idx);

    void __cdecl Break_Research_(int16_t player1_idx, int16_t player2_idx);

    void __cdecl Break_Tribute_(int16_t p1_idx, int16_t p2_idx);

    void __cdecl Get_Break_Treaty_Message_(int16_t victim_idx, int16_t actor_idx, int16_t* msg_id);

    void __cdecl Get_Break_Trade_Message_(int16_t other_player_idx, int16_t this_player_idx, int16_t* out_message_id);

    void __cdecl Get_Break_Research_Message_(int16_t other_player_idx, int16_t this_player_idx, int16_t* out_msg_id);

    void __cdecl Get_Break_Tribute_Treaty_Message_(int16_t other_player_idx, int16_t acting_player_idx, int16_t* out_msg_id);

    void __cdecl Start_Trade_Treaty_(int16_t player_idx, int16_t target_player_idx);

    void __cdecl Start_Research_Treaty_(int16_t player_idx, int16_t other_idx);

    void __cdecl Modifier_Diplomacy_Adjustments_();

    void NoOp2();

    void Clear_Diplomacy_Messages_();

    void __cdecl Diplomacy_Growth_();

    void Determine_First_Contacts_();

    void Determine_Diplomacy_Messages_();

    void __cdecl Limit_Treaty_Modifiers_();

    void __cdecl Determine_Bad_Message_(int16_t current_player, int16_t target_player);

    void __cdecl Determine_Good_Message_(int16_t player_idx, int16_t other_player_idx, int incident);

    void __cdecl Peace_Proposal_(int16_t player_idx, int16_t target_idx);

    void __cdecl Required_Alliances_(int16_t player_idx, int16_t other_player_idx);

    void __cdecl NPC_Required_Alliances_(int16_t p1_idx, int16_t p2_idx);

    void __cdecl Unprovoked_Good_Messages_(int16_t player_idx, int16_t target_idx);

    void __cdecl End_Of_Turn_Diplomacy_Adjustments_();

    void __cdecl Reward_Check_(int16_t param_1, int16_t param_2, int16_t param_3, int16_t param_4);

    void __cdecl NPC_Sneak_Attacks_();

    int16_t __cdecl Sneak_Attack_Evaluations_(int16_t player_id, int16_t target_id, int16_t *out_severity, int16_t *out_msg_id, int16_t *unused);

    int16_t __cdecl Get_Player_Diplomacy_Personality_(int16_t player_idx, int16_t target_idx);

    int16_t __cdecl Get_War_Count_(int16_t player_idx, int16_t exclude_idx);

    void __cdecl Sneak_Attack_Or_Declare_War_(int16_t attacker_idx, int16_t target_idx, int8_t message_idx);

    int16_t __cdecl Total_War_Loss_Comparison_(int16_t attacker_idx, int16_t target_idx);

    inline int16_t __cdecl Get_Demands_(int16_t current_player, int16_t target_player, int demand_count, int p1, int p2, int p3, int p4) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int, int, int, int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004526D0});
        return fn(current_player, target_player, demand_count, p1, p2, p3, p4);
    }

    void __cdecl Set_Demands_(int16_t current_player, int16_t target_player);

    void __cdecl Start_Tribute_Treaty_(int16_t player_idx, int16_t target_idx, int16_t amount);

    int __cdecl Fleet_Strength_Comparison_(int16_t player_idx_1, int16_t player_idx_2);

    int __cdecl Tech_Strength_Comparison_(int16_t player_idx1, int16_t player_idx2);

    int __cdecl Maintenance_Strength_Comparison_(int16_t player_idx_1, int16_t player_idx_2);

    void __cdecl Start_Treaty_(int16_t p1, int16_t p2, int16_t treaty_type);

    void __cdecl Adjust_Diplomat_Modifiers_(int16_t player_idx, int16_t target_idx);

    void __cdecl Get_Declare_War_Message_(int16_t player_a_idx, int16_t player_b_idx, int16_t* msg_id);

    void __cdecl Diplomacy_Broken_Contacts_();

    void __cdecl Clear_Last_Proposal_Turn_(int16_t player_idx_1, int16_t player_idx_2);

    int16_t __cdecl Find_Worst_Modifier_(int16_t player_idx, int16_t target_idx);

    int8_t Valid_Treaty_Proposal_(int16_t player_idx, int16_t target_idx, int16_t treaty_type);

    inline int16_t __cdecl Check_Treaty_Proposal_(int16_t target_idx, int16_t proposer_idx, int16_t treaty_type, int16_t* response_message, int16_t* var1, int16_t* var2, int16_t* var3, int16_t* var4, int16_t* var5) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t, int16_t*, int16_t*, int16_t*, int16_t*, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004556F0});
        return fn(target_idx, proposer_idx, treaty_type, response_message, var1, var2, var3, var4, var5);
    }

    void __cdecl Get_Gift_Response_(int16_t plr_a, int16_t plr_b, int16_t gift_type, int16_t gift_data, int16_t *response_code);

    inline void __cdecl Get_Demand_Response_(int16_t param_1, int16_t param_2, int16_t demand_type, uint16_t treaty_val, int16_t* response, int16_t* message) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, uint16_t, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00456e10});
        fn(param_1, param_2, demand_type, treaty_val, response, message);
    }
}