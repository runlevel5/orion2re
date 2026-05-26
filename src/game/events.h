#pragma once
#include <cstdint>

struct s_colors;
struct s_mouse_list;
struct s_animation_header;

namespace EVENTS {
    inline uint8_t& _current_event = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A6450});
    inline s_animation_header*& _event_display_seg =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A6454});
    inline s_event_data (&_event_data)[36] = *reinterpret_cast<s_event_data(*)[36]>(std::uintptr_t{0x005A6460});
    inline MOX2::action_func_t& _jim_saved_autofunction = *reinterpret_cast<MOX2::action_func_t*>(std::uintptr_t{0x005A65B0});
    inline uint32_t& _last_event_year = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x005A65A8});
    inline int16_t& _event_delay_count = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A65AE});
    inline int8_t (&_event_good_array)[36] =
        *reinterpret_cast<int8_t(*)[36]>(std::uintptr_t{0x00557508});

    int16_t __cdecl Get_Antaran_Victim_();
    void __cdecl Init_Events_();
    int32_t __cdecl Event_Get_Fleet_Strength_(int16_t player_idx);

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

    int16_t __cdecl Pick_Random_Colony_No_Outpost_(int8_t player_idx);

    int16_t __cdecl Pick_Random_Star_(int16_t player_idx);

    int16_t __cdecl Star_Has_Event_(int16_t star_idx);

    int16_t __cdecl Event_Pirate_Activity_Strength_(int16_t star_idx);

    int16_t __cdecl Pick_Completely_Random_Star_();

    void __cdecl Event_Check_Planet_Warning_();

    void __cdecl Event_Check_General_Status_();

    inline uint8_t& _event_status_flag =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A65AC});

    inline uint8_t*& _event_mouse_bitmap =
        *reinterpret_cast<uint8_t**>(std::uintptr_t{0x005A65A4});

    inline char* _event_message =
        reinterpret_cast<char*>(std::uintptr_t{0x005A65C0});

    inline int8_t* s__00557509 =
        reinterpret_cast<int8_t*>(std::uintptr_t{0x00557509});

    inline char* s__00551100 = reinterpret_cast<char*>(std::uintptr_t{0x00551100});

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

    void __cdecl Start_Main_Event_();

    inline s_colors& _event_normal_colors =
        *reinterpret_cast<s_colors*>(std::uintptr_t{0x00557500});

    inline s_mouse_list& _event_blank_mouse =
        *reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x00557530});

    void __cdecl Event_Fade_In_();

    void __cdecl Draw_Event_Screen_();

    inline char* s_ian_00550000 = reinterpret_cast<char*>(std::uintptr_t{0x00550000}); // "german-austrian"
    inline char* s_s_005510f4 = reinterpret_cast<char*>(std::uintptr_t{0x005510F4}); // "s"
    inline char* s_0_0055110c = reinterpret_cast<char*>(std::uintptr_t{0x0055110C}); // " 0"
    inline char* s_jimtext2_lbx_00552340 = reinterpret_cast<char*>(std::uintptr_t{0x00552340}); // "jimtext2.lbx"
    inline char* s__005569f8 = reinterpret_cast<char*>(std::uintptr_t{0x005569F8}); // "\n"
    inline char* s_X_d__d___s_00557548 = reinterpret_cast<char*>(std::uintptr_t{0x00557548}); // " X%d.%d. %s"
    inline char* s_EVENTMSP_LBX_00557554 = reinterpret_cast<char*>(std::uintptr_t{0x00557554}); // "EVENTMSP.LBX"
    inline char* s_EVENTMSI_LBX_00557564 = reinterpret_cast<char*>(std::uintptr_t{0x00557564}); // "EVENTMSI.LBX"
    inline char* s_EVENTMSS_LBX_00557574 = reinterpret_cast<char*>(std::uintptr_t{0x00557574}); // "EVENTMSS.LBX"
    inline char* s_EVENTMSF_LBX_00557584 = reinterpret_cast<char*>(std::uintptr_t{0x00557584}); // "EVENTMSF.LBX"
    inline char* s_EVENTMSG_LBX_00557594 = reinterpret_cast<char*>(std::uintptr_t{0x00557594}); // "EVENTMSG.LBX"
    inline char* s_EVENTMSE_LBX_005575a4 = reinterpret_cast<char*>(std::uintptr_t{0x005575A4}); // "EVENTMSE.LBX"

    inline const char* s_EVENTS_LBX_0055753c = reinterpret_cast<const char*>(std::uintptr_t{0x0055753C});

    void __cdecl Get_Event_Message_(
        int16_t event_type,
        int16_t event_subtype,
        int16_t planet_idx,
        int16_t val1,
        int16_t val2,
        int16_t player_idx1,
        int16_t tech_field_idx1,
        int16_t player_idx2,
        int16_t leader_idx,
        int16_t player_idx3,
        int16_t star_idx,
        int16_t tech_app_idx1,
        int16_t tech_app_idx2,
        char* out_str);

    void __cdecl Drive_Event_Screen_();

    void __cdecl Load_Event_Picture_(int16_t event_id);

    void __cdecl Setup_Next_Event_(int16_t event_id);

    void __cdecl Reset_Events_();

    void __cdecl Event_Monster_Update_(int16_t param_1, int16_t param_2, int16_t param_3);
}