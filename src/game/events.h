#pragma once
#include <cstdint>

struct s_colors;
struct s_mouse_list;
struct s_animation_header;

namespace EVENTS {
    extern uint8_t _current_event;
    extern s_animation_header* _event_display_seg;
    extern s_event_data _event_data[36];
    extern MOX2::action_func_t _jim_saved_autofunction;
    extern uint32_t _last_event_year;
    extern int16_t _event_delay_count;
    extern int8_t _event_good_array[36];

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

    extern uint8_t _event_status_flag;

    extern uint8_t* _event_mouse_bitmap;

    extern char _event_message[300];

    extern const char s__00551100[];

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

    extern s_colors _event_normal_colors;

    extern s_mouse_list _event_blank_mouse;

    void __cdecl Event_Fade_In_();

    void __cdecl Draw_Event_Screen_();
    extern const char s_s_005510f4[];
    extern const char s_0_0055110c[];
    extern const char s_jimtext2_lbx_00552340[];
    extern const char s_X_d__d___s_00557548[];
    extern const char s_EVENTMSP_LBX_00557554[];
    extern const char s_EVENTMSI_LBX_00557564[];
    extern const char s_EVENTMSS_LBX_00557574[];
    extern const char s_EVENTMSF_LBX_00557584[];
    extern const char s_EVENTMSG_LBX_00557594[];
    extern const char s_EVENTMSE_LBX_005575a4[];

    extern const char s_EVENTS_LBX_0055753c[];

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