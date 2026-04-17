#pragma once
#include <cstdint>

namespace EVENTS {
    inline uint8_t& _current_event = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A6450});
    inline s_event_data (&_event_data)[36] = *reinterpret_cast<s_event_data(*)[36]>(std::uintptr_t{0x005A6460});
    inline void*& _jim_saved_autofunction = *reinterpret_cast<void**>(std::uintptr_t{0x005A65B0});
    inline uint32_t& _last_event_year = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x005A65A8});
    inline int16_t& _event_delay_count = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A65AE});
    inline int8_t (&_event_good_array)[36] =
        *reinterpret_cast<int8_t(*)[36]>(std::uintptr_t{0x00557508});

    int16_t __cdecl Get_Antaran_Victim_();
    void Init_Events_();

    int16_t __cdecl Event_Check_Comet_(int16_t target_id);

    int16_t __cdecl Event_Check_Space_Anomaly_(int16_t colony_idx);
    int16_t __cdecl Event_Check_Space_Anomaly_At_Star_(int16_t star_idx);

    int16_t __cdecl Event_Check_Population_Boom_(int16_t colony_idx);
    int16_t __cdecl Event_Check_Plague_(int16_t colony_idx);

    int16_t __cdecl Event_Check_Pirate_Activity_(uint8_t player_idx);
    int16_t __cdecl Event_Check_Colony_Researching_(int16_t colony_idx);

    int16_t __cdecl Event_Check_Hyperspace_Flux_();

    void __cdecl Event_Surrender_(int8_t player_idx, int16_t target_id);

    int16_t __cdecl Colony_Has_Event_(int16_t colony_idx);

    bool __cdecl Player_Has_Bad_Event_(int16_t player_idx);

    void __cdecl Determine_Event_();

    void __cdecl Event_Twiddle_();

    void __cdecl Update_Lucky_Players_Events_();

    int16_t __cdecl Determine_Lucky_Players_Events_();

    int16_t __cdecl Get_Event_Climate_Change_Colony_(int16_t player_idx);

    int16_t __cdecl Pick_Random_Player_In_Contact_(int16_t player_idx);

    int16_t __cdecl Pick_Random_Ship_(int16_t player_idx);

    int16_t __cdecl Get_Event_Industrial_Accident_Colony_(int16_t player_idx);

    int16_t __cdecl Get_Event_Mineral_Depletion_Colony_(int16_t player_idx);
    int16_t __cdecl Get_Event_Mineral_Deposit_Colony_(int16_t player_idx);

    int16_t __cdecl Pick_Random_Colony_No_Capitol_(int16_t player_idx);

    int16_t __cdecl Pick_Random_Colony_No_Outpost_(int16_t player_idx);

    int16_t __cdecl Pick_Random_Star_(int16_t player_idx);

    int16_t __cdecl Star_Has_Event_(int16_t star_idx);

    int16_t __cdecl Event_Pirate_Activity_Strength_(int16_t star_idx);

    int16_t __cdecl Pick_Completely_Random_Star_();

    void __cdecl Event_Check_Planet_Warning_();

    void __cdecl Event_Check_General_Status_();

    inline int8_t* s__00557509 =
        reinterpret_cast<int8_t*>(std::uintptr_t{0x00557509});

    int16_t __cdecl Ship_Size_Points_At_Star_(int16_t star_idx);

    void __cdecl Allocate_Comet_Hit_(int16_t colony_idx);

    void __cdecl Allocate_Earthquake_Hit_(int16_t colony_idx, int16_t* pop_killed, int16_t* buildings_destroyed_count);

    int16_t __cdecl Colony_Research_Points_(int16_t colony_idx);

    void __cdecl Allocate_Industrial_Accident_Hit_(int16_t colony_idx);

    int16_t __cdecl Get_Event_Victim_(int8_t type);

    void __cdecl Event_Orion_Discovered_(int8_t player_idx);
    void __cdecl Event_Race_Eliminated_(int8_t player_idx);
    int16_t __cdecl Event_Check_Warp_Beast_();

    void __cdecl Kill_All_Events_At_Colony_(int16_t colony_idx);

    bool __cdecl Check_For_Event_Message_();

    void __cdecl Check_For_Event_();

    inline void __cdecl Setup_Next_Event_(int16_t event_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00474F60});
        fn(event_idx);
    }

    inline void __cdecl Reset_Events_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00473E50});
        fn();
    }

    void __cdecl Event_Monster_Update_(int16_t param_1, int16_t param_2, int16_t param_3);
}