#pragma once

namespace MSG_ {
    extern uint8_t _first_msg[8];
    extern uint8_t _n_msgs[8];
    extern s_MSG _msgs[100];
    extern uint8_t _max_msgs[8];

    void __cdecl Allocate_Msg_Slots_();
    int16_t __cdecl N_Msgs_For_Player_(int16_t player_idx);

    void __cdecl Add_Msg_(s_MSG msg, int16_t plr_idx);

    int16_t __cdecl First_Msg_For_Player_(int16_t player_idx);

    char* __cdecl Load_Msg_Strings_(char* native_msg);

    void __cdecl Add_Ongoing_Event_Msgs_();

    void __cdecl Add_Peace_Ended_Msg_(int player_i, int player_j);

    void __cdecl Add_Peace_Msg_(int32_t player_a_idx, int32_t player_b_idx);

    void __cdecl Add_Diplomacy_Msg_(int16_t plr_a, int16_t plr_b, int16_t diplomacy_idx);

    extern char* _msg_artifacts_fmt;
    extern char* _msg_colony_your_rebellion_lost_fmt;
    extern char* _msg_colony_starvation_food_fmt;
    extern char* _msg_colony_1_rebel_won_fmt;
    extern char* _msg_colony_assimilation_fmt;
    extern char* _msg_scrap_leader_fmt;
    extern char* _msg_colony_genocide_fmt;
    extern char* _msg_hyperspace_beast_hungry_fmt;
    extern char* _msg_lost_ship_fmt;
    extern char* _s_and_s_declare_war_fmt;
    extern char* _msg_colony_built_freighter_fmt;
    extern char* _msg_lost_attacking_spy_fmt;
    extern char* _msg_1_ship_killed_retreating_fmt;
    extern char* _msg_colony_built_next_queue_fmt;
    extern char* _lost_colony_reason_strings[4];
    extern char* _msg_lost_defending_spies_fmt;
    extern char* _s_and_s_declare_alliance_fmt;
    extern char* _msg_colony_d_rebels_won_fmt;
    extern char* _msg_hyperspace_flux_fmt;
    extern char* _msg_financial_fmt;
    extern char* _msg_lost_colony_leader_fmt;
    extern char* _msg_colony_built_android_farmer_fmt;
    extern char* _native_msg;
    extern char* _msg_scrap_ship_hyperspace_fmt;
    extern char* _msg_offensive_spy_fmt;
    extern char* _msg_colony_built_android_scientist_fmt;
    extern char* _msg_lost_defending_spy_fmt;
    extern char* _msg_april1_fmt;
    extern char* _msg_lost_colony_fmt;
    extern char* _msg_colony_boom_fmt;
    extern char* _msg_defensive_spy_fmt;
    extern char* _msg_eel_vanished_fmt;
    extern char* _msg_colony_built_ship_fmt;
    extern char* _leader_fate_strings[3];
    extern char* _msg_colony_built_auto_build_fmt;
    extern char* _msg_colony_android_killed_organic_fmt;
    extern char* _msg_colony_settlers_arrived_blockaded_fmt;
    extern char* _msg_colony_comet_fmt;
    extern char* _msg_colony_d_settlers_arrived_fmt;
    extern char* _msg_colony_starvation_ind_fmt;
    extern char* _msg_d_ships_killed_retreating_fmt;
    extern char* _msg_failed_retreat_leader_fmt;
    extern char* _msg_colony_or_outpost[2];
    extern char* _msg_colony_1_settler_arrived_over_limit_fmt;
    extern char* _msg_spy_assassinated_fmt;
    extern char* _msg_spy_assassinated_victim_fmt;
    extern char* _msg_colony_no_room_for_android_fmt;
    extern char* _failed_retreat_reason_strings[3];
    extern char* _msg_eel_spawned_fmt;
    extern char* _msg_scrap_ship_bc_fmt;
    extern char* _msg_colony_d_rebels_lost_fmt;
    extern char* _msg_colony_scrap_building_fmt;
    extern char* _msg_hyperspace_beast_ate_other_ship_fmt;
    extern char* _msg_colony_built_android_worker_fmt;
    extern char* _msg_colony_built_refit_fmt;
    extern char* _msg_artemis_leader_fmt;
    extern char* _msg_colony_your_rebellion_won_fmt;
    extern char* _msg_colony_plague_fmt;
    extern char* _msg_anomaly_fmt;
    extern char* _msg_diplomacy_strings[5];
    extern char* _msg_colony_starvation_both_fmt;
    extern char* _msg_colony_built_spy_fmt;
    extern char* _msg_colony_built_building_fmt;
    extern char* _msg_artemis_fmt;
    extern char* _msg_colony_1_rebel_lost_fmt;
    extern char* _msg_hyperspace_beast_ate_your_ship_fmt;
    extern char* _msg_colony_1_settler_arrived_fmt;
    extern char* _msg_colony_d_settlers_arrived_over_limit_fmt;

    void __cdecl sprintf_msg_(char* param_1, s_MSG* param_2);

    void __cdecl Add_Msg_All_(s_MSG msg);

    int8_t __cdecl Goto_Msg_Colony_(s_MSG* msg);

    void __cdecl Add_Nonaggression_Msg_(int16_t plr_a, int16_t plr_b);

    void __cdecl Add_Alliance_Msg_(uint32_t player_a_idx, uint32_t player_b_idx);

    void __cdecl Add_War_Msg_(int32_t player_a, int32_t player_b);
}