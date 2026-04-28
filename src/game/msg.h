#pragma once

namespace MSG_ {
    inline uint8_t (&_first_msg)[8] = *reinterpret_cast<uint8_t(*)[8]>(std::uintptr_t{0x005A66F0});
    inline uint8_t (&_n_msgs)[8] = *reinterpret_cast<uint8_t(*)[8]>(std::uintptr_t{0x005A66F8});
    inline s_MSG (&_msgs)[100] = *reinterpret_cast<s_MSG(*)[100]>(std::uintptr_t{0x005A6700});
    inline uint8_t (&_max_msgs)[8] = *reinterpret_cast<uint8_t(*)[8]>(std::uintptr_t{0x005A6E08});

    void __cdecl Allocate_Msg_Slots_();
    int16_t __cdecl N_Msgs_For_Player_(int16_t player_idx);

    void __cdecl Add_Msg_(s_MSG msg, int16_t plr_idx);

    int16_t __cdecl First_Msg_For_Player_(int16_t player_idx);

    inline char* __cdecl Load_Msg_Strings_(char* data) {
        using Fn = char*(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00472040});
        return fn(data);
    }

    void __cdecl Add_Ongoing_Event_Msgs_();

    void __cdecl Add_Peace_Ended_Msg_(int player_i, int player_j);

    void __cdecl Add_Peace_Msg_(int32_t player_a_idx, int32_t player_b_idx);

    void __cdecl Add_Diplomacy_Msg_(int16_t plr_a, int16_t plr_b, int16_t diplomacy_idx);

    inline char*& _msg_artifacts_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565EF0});
    inline char*& _msg_colony_your_rebellion_lost_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565EF4});
    inline char*& _msg_colony_starvation_food_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565EF8});
    inline char*& _msg_colony_1_rebel_won_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565EFC});
    inline char*& _msg_colony_assimilation_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F00});
    inline char*& _msg_scrap_leader_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F04});
    inline char*& _msg_colony_genocide_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F08});
    inline char*& _msg_hyperspace_beast_hungry_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F0C});
    inline char*& _msg_lost_ship_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F10});
    inline char*& _s_and_s_declare_war_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F14});
    inline char*& _msg_colony_built_freighter_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F18});
    inline char*& _msg_lost_attacking_spy_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F1C});
    inline char*& _msg_1_ship_killed_retreating_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F20});
    inline char*& _msg_colony_built_next_queue_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F24});
    inline char** _lost_colony_reason_strings = reinterpret_cast<char**>(std::uintptr_t{0x00565F28});
    inline char*& _msg_lost_defending_spies_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F38});
    inline char*& _s_and_s_declare_alliance_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F3C});
    inline char*& _msg_colony_d_rebels_won_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F40});
    inline char*& _msg_hyperspace_flux_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F44});
    inline char*& _msg_financial_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F48});
    inline char*& _msg_lost_colony_leader_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F4C});
    inline char*& _msg_colony_built_android_farmer_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F50});
    inline char*& _native_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00565F54});
    inline char*& _msg_scrap_ship_hyperspace_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F58});
    inline char*& _msg_offensive_spy_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F5C});
    inline char*& _msg_colony_built_android_scientist_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F60});
    inline char*& _msg_lost_defending_spy_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F64});
    inline char*& _msg_april1_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F68});
    inline char*& _msg_lost_colony_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F6C});
    inline char*& _msg_colony_boom_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F70});
    inline char*& _msg_defensive_spy_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F74});
    inline char*& _msg_eel_vanished_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F78});
    inline char*& _msg_colony_built_ship_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F7C});
    inline char** _leader_fate_strings = reinterpret_cast<char**>(std::uintptr_t{0x00565F80});
    inline char*& _msg_colony_built_auto_build_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F8C});
    inline char*& _msg_colony_android_killed_organic_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F90});
    inline char*& _msg_colony_settlers_arrived_blockaded_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F94});
    inline char*& _msg_colony_comet_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F98});
    inline char*& _msg_colony_d_settlers_arrived_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565F9C});
    inline char*& _msg_d_ships_killed_retreating_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565FA4});
    inline char*& _msg_failed_retreat_leader_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565FA8});
    inline char** _msg_colony_or_outpost = reinterpret_cast<char**>(std::uintptr_t{0x00565FB0});
    inline char*& _msg_colony_1_settler_arrived_over_limit_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565FB8});
    inline char*& _msg_spy_assassinated_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565FBC});
    inline char*& _msg_spy_assassinated_victim_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565FC0});
    inline char*& _msg_colony_no_room_for_android_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565FC4});
    inline char** _failed_retreat_reason_strings = reinterpret_cast<char**>(std::uintptr_t{0x00565FC8});
    inline char*& _msg_eel_spawned_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565FD4});
    inline char*& _msg_scrap_ship_bc_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565FD8});
    inline char*& _msg_colony_d_rebels_lost_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565FDC});
    inline char*& _msg_colony_scrap_building_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565FE0});
    inline char*& _msg_hyperspace_beast_ate_other_ship_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565FE4});
    inline char*& _msg_colony_built_android_worker_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565FE8});
    inline char*& _msg_colony_built_refit_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565FEC});
    inline char*& _msg_artemis_leader_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565FF0});
    inline char*& _msg_colony_your_rebellion_won_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565FF4});
    inline char*& _msg_colony_plague_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565FF8});
    inline char*& _msg_anomaly_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00565FFC});
    inline char** _msg_diplomacy_strings = reinterpret_cast<char**>(std::uintptr_t{0x00566000});
    inline char*& _msg_colony_starvation_ind_fmt = *reinterpret_cast<char**>(std::uintptr_t{ 0x00565fa0 });
    inline char*& _msg_colony_starvation_both_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00566014});
    inline char*& _msg_colony_built_spy_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00566018});
    inline char*& _msg_colony_built_building_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x0056601C});
    inline char*& _msg_artemis_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00566020});
    inline char*& _msg_colony_1_rebel_lost_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00566024});
    inline char*& _msg_hyperspace_beast_ate_your_ship_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00566028});
    inline char*& _msg_colony_1_settler_arrived_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x0056602C});
    inline char*& _msg_colony_d_settlers_arrived_over_limit_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x00566030});

    void __cdecl sprintf_msg_(char* param_1, s_MSG* param_2);

    void __cdecl Add_Msg_All_(s_MSG msg);

    int8_t __cdecl Goto_Msg_Colony_(s_MSG* msg);

    void __cdecl Add_Nonaggression_Msg_(uint8_t plr_a, uint8_t plr_b);

    void __cdecl Add_Alliance_Msg_(uint32_t player_a_idx, uint32_t player_b_idx);

    void __cdecl Add_War_Msg_(int32_t player_a, int32_t player_b);
}