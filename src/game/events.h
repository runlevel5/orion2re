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

    /** Advances event processing and queues any reportable events for display. */
    void __cdecl Check_For_Event_();

    /** Initializes the event-report screen state, music, and drawing hooks. */
    void __cdecl Start_Main_Event_();

    extern s_colors _event_normal_colors;

    extern s_mouse_list _event_blank_mouse;

    /** Fades in the event screen and primes its animation and mouse buffers. */
    void __cdecl Event_Fade_In_();

    /** Draws the current event illustration and formatted report text. */
    void __cdecl Draw_Event_Screen_();

    /**
     * Builds the localized message text for a specific event report variant.
     * @param colony_idx Colony index used by planet-name substitution tokens.
     * @param val1 First numeric or lookup substitution value.
     * @param val2 Second numeric substitution value.
     * @param out_str Destination buffer that receives the formatted message.
     */
    void __cdecl Get_Event_Message_(
        int16_t event_type,
        int16_t event_subtype,
        int16_t colony_idx,
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

    /** Runs the event-report input loop until the player dismisses the screen. */
    void __cdecl Drive_Event_Screen_();

    /** Loads the event illustration for the specified event. */
    void __cdecl Load_Event_Picture_(int16_t event_id);

    /** Prepares and presents the next pending event report. */
    void __cdecl Setup_Next_Event_(int16_t event_id);

    /** Reinitializes the event table after a full cleanup checkpoint. */
    void __cdecl Reset_Events_();

    /**
     * Updates monster-event state after a monster move or resolution step.
     * @param monster_type Nonplayer monster ship type used to select the event slot.
     * @param target_id Target index for a location update, or -1 for a payload-only update.
     * @param player_idx_or_amount Player index for target updates, or amount for payload updates.
     */
    void __cdecl Event_Monster_Update_(int16_t monster_type, int16_t target_id, int16_t player_idx_or_amount);
}