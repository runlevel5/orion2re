#pragma once
#include <cstdint>

struct s_animation_header;
struct s_colony_combat_popup_data;
struct s_combat_bomb_result;
struct s_invasion_side_info;
struct s_potential_colony_info;

typedef struct s_colony_combat_popup_data s_colony_combat_popup_data, *Ps_colony_combat_popup_data;

struct s_colony_combat_popup_data {
    int16_t colony_idx;
    int8_t attacker_player_id;
    int16_t attacker_unit_counts[UNIT_COUNT];
    int8_t has_stellar_converter;
    int8_t defender_player_id;
    int16_t defender_unit_counts[UNIT_COUNT];
    int16_t n_pops;
    int16_t n_bldgs;
    int16_t best_shield_bldg;
    struct s_combat_bomb_result *bomb;
    struct s_invasion_side_info *attacker;
    struct s_invasion_side_info *defender;
    int32_t random_seed;
    int16_t bombardment_result;
    undefined field14_0x2d;
    undefined field15_0x2e;
    undefined field16_0x2f;
};

namespace MAINPUPS {
    extern char* _g_msg_ptr;

    extern s_animation_header* _anims[38];

    extern uint8_t _on_officer_screen_flag;

    extern int16_t _g_officer;

    extern int16_t _officer_gains_level_state;

    extern char* _g_str_ptr;

    extern int16_t _last_scanned_field;
    extern int16_t _g_state;
    extern int8_t _selection_state_flag;
    extern uint8_t _g_using_colony_ship;
    extern int16_t _g_max;
    extern int16_t _g_min;
    extern char* _g_msg;
    extern int16_t _g_var;
    extern int16_t _g_temp_tax_rate;
    extern int16_t _field_state;
    extern s_colors _main_scr_font_colors;

    extern uint8_t _g_special;
    extern int16_t _g_x;
    extern int16_t _g_y;
    extern int16_t _g_star_n;
    extern int16_t _g_player_n;
    extern int16_t _field_mode;
    extern int16_t _g_scanned_star;
    extern int16_t _g_old_scanned_star;
    extern uint8_t _no_flicker_between_mainpups_flag;
    extern int16_t _autofunction_ctr;
    extern int16_t _num_field_saves;
    extern s_fieldsav_data _stats_array[3];
    extern uint8_t _explode_flag;
    extern int16_t _g_refresh_mode;
    extern MOX2::action_func_t _eric_saved_autofunction2;
    extern MOX2::action_func_t _eric_saved_autofunction;

    extern MOX2::action_func_t _temp_af;

    extern s_colony_combat_popup_data* _g_data_ptr;

    void __cdecl Load_Tax_Rate_Popup_();
    void __cdecl Get_Picture_Draw_XY_(s_animation_header* anim);

    void __cdecl Load_Colony_Fields_();

    void __cdecl Set_G_Special_Value_(int16_t star_idx);

    void __cdecl Update_Selection_Fields_();

    void __cdecl Draw_Selection_Popup_();

    int16_t __cdecl Evaluate_Selection_Input_(int16_t* selected_field, int16_t* field_data, int16_t* refresh_flag);

    /**
     * Evaluates a system-display click for colony, outpost, troop-drop, or dragon selection popups.
     * @param mouse_x Current X-side field scan input.
     * @param mouse_y Current Y-side field scan input.
     * @param selection_confirmed Set to 1 when a valid target is chosen.
     */
    int16_t __cdecl Check_System_Display_Fields_Selection_(int16_t* mouse_x, int16_t* mouse_y, int16_t* selection_confirmed);

    int8_t __cdecl New_Colony_Confirmation_Box_(int16_t planet_idx);

    void __cdecl Load_SLIDER_Popup_();
    void __cdecl Load_Slider_Fields_();
    void __cdecl Add_Slider_Fields_(int16_t min_val, int16_t max_val, int16_t* value_ptr);

    /**
     * Sets up popup rendering state and centers the popup art over the selected star.
     * @param star_idx Star to focus in the system display, or -1 to use the current player's home star.
     * @param popup_anim Popup art used to compute the centered draw position.
     */
    void __cdecl Do_Popup_Setup_(int16_t star_idx, s_animation_header* popup_anim);

    void __cdecl Add_Tax_Rate_Fields_();
    void __cdecl Eric_Pop_Fields_();

    void __cdecl Load_New_Officer_Popup_For_Officer_Screen_();

    void __cdecl Load_New_Officer_Popup_();

    void __cdecl Update_Random_New_Officer_Fields_();

    void __cdecl Draw_Random_New_Officer_Popup_();

    extern int16_t _mp_fields[26];


    void __cdecl Eric_Push_Fields_();

    void __cdecl Add_Selection_Fields_();

    void __cdecl Draw_Tax_Rate_Popup_();

    void __cdecl Draw_Slider_Popup_();

    void __cdecl Evaluate_Slider_Input_(int16_t* input, int16_t* scanned_field, int16_t* done);

    void __cdecl Mainpups_Tested_Execute_Autofunction_();

    void __cdecl Leader_Technology_String_(int16_t player_idx, int16_t leader_idx, char* output_buf);

    void __cdecl Evaluate_Tax_Rate_Input_(int16_t* input_val, int16_t* scan_val, int16_t* exit_flag);

    void __cdecl Do_Popup_Cleanup_();

    void __cdecl Load_Colony_Combat_Popup_();

    void __cdecl Draw_Colony_Combat_Popup_();

    /**
     * Rebuilds the colony-combat popup fields for the current mode and transient combat state.
     * @param popup_mode Popup mode selector used by the combat popup loop.
     * @param combat_state_ptr Current bombing/invasion substate used to choose available actions.
     */
    void __cdecl Update_Colony_Combat_Fields_(int16_t popup_mode, int16_t* combat_state_ptr);

    /**
     * Adds the actionable colony-combat buttons for the current popup mode and combat state.
     * @param popup_mode Popup mode selector used by the combat popup loop.
     * @param combat_state_ptr Current bombing/invasion substate used to choose available actions.
     */
    void __cdecl Add_Colony_Combat_Fields_(int16_t popup_mode, int16_t* combat_state_ptr);

    void __cdecl Setup_To_Apply_Bomb_Hits_(s_combat_bomb_result* result, int16_t* p_bomb_hits_remaining, int16_t* p_bio_hits_remaining);

    void __cdecl Apply_Next_Bomb_Hit_(s_combat_bomb_result *bomb, int16_t *state, int16_t *bomb_hits, int16_t *bio_hits);

    int16_t __cdecl Evaluate_Colony_Combat_Input_(int16_t* field_id_ptr, int16_t* unused_input, int16_t* close_popup_flag, int16_t* selection_idx_ptr);

    void __cdecl Load_Combat_Colony_Picture_To_Screen_Seg_(int16_t colony_idx);

    void __cdecl Load_System_Discovery_Popup_();

    void __cdecl Clear_System_Special_Slots_();

    void __cdecl Update_Discovery_Fields_();

    void __cdecl Draw_Discovery_Popup_();

    void __cdecl Do_Evaluate_Discovery_Input_(int16_t* mouse_x, int16_t* mouse_y);

    void __cdecl Evaluate_Discovery_Input_(int16_t* input_1, int16_t* input_2);

    void __cdecl Marooned_Officer_Fails_Popup_(int16_t star_idx, int16_t officer_idx);

    void __cdecl Marooned_Officer_Joins_Popup_(int16_t star_idx, int16_t officer_idx);

    void __cdecl Build_Generic_Planet_Or_Colony_Description_String_Formatted_(int16_t planet_idx, char* buffer);

    int16_t __cdecl Dragon_New_Star_Popup_(int16_t player_idx, int16_t star_idx);

    void __cdecl Load_Dragon_New_Star_Popup_();

    void __cdecl Draw_Dragon_New_Star_Popup_();

    void __cdecl Update_Dragon_New_Star_Fields_();

    int16_t __cdecl Evaluate_Dragon_New_Star_Input_(int16_t* field_idx_ptr, int16_t* mouse_pos_ptr, int16_t* status_ptr, int16_t target_star_val);

    int16_t __cdecl Defense_Colony_Selection_Popup_(int16_t star_idx, int16_t player_idx);

    void __cdecl Load_Colony_Defense_Selection_Popup_();

    void __cdecl Draw_Defense_Selection_Popup_();

    void __cdecl Update_Colony_Defense_Fields_();

    int16_t __cdecl Evaluate_Defense_Selection_Input_(int16_t* field_idx, int16_t* mouse_status, int16_t* selection_made);

    /**
     * Evaluates defense-selection clicks on the system display and returns the chosen colony or hostile owner.
     * @param mouse_x Current X-side field scan input.
     * @param mouse_y Current Y-side field scan input.
     * @param selection_made Set to 1 when a valid defense target is chosen.
     */
    int16_t __cdecl Check_System_Display_Fields_Defense_Selection_(int16_t* mouse_x, int16_t* mouse_y, int16_t* selection_made);

    void __cdecl Player_Attacked_Popup_(int16_t attacker_idx, int16_t defender_idx, int16_t star_idx, int16_t colony_idx);

    int16_t __cdecl Slider_Popup_(int16_t min, int16_t max, int16_t var, char* msg, int16_t param_5);

    void __cdecl Clear_New_Officer_Slots_();

    void __cdecl Add_Officer_Gains_Level_Fields_();

    void __cdecl Load_Officer_Gains_Level_Popup_();

    void __cdecl Update_Officer_Gains_Level_Fields_();

    void __cdecl Draw_Officer_Gains_Level_Popup_();

    int16_t __cdecl New_Colony_Selection_Popup_(int16_t star_idx, uint8_t using_colony_ship);
    int16_t __cdecl New_Outpost_Selection_Popup_(int16_t star_id);

    int16_t __cdecl Drop_Troops_Selection_Popup_(int16_t target_idx);

    bool __cdecl Random_New_Officer_Popup_(int16_t player_idx);

    void __cdecl New_System_Discovery_Popup_(int16_t star_idx, uint8_t special_flag);

    void __cdecl Officer_Gains_Level_Popup_(int16_t star_handle, int16_t officer_idx);

    const char* __cdecl System_Special_Desc_(int16_t desc_id);

    s_animation_header* __cdecl System_Special_Anim_(int16_t star_idx, int16_t anim_type);

    /**
     * Draws a titled system-special popup with the supplied animation and descriptive text.
     * @param x Left screen coordinate for the popup.
     * @param y Top screen coordinate for the popup.
     * @param anim Special-case illustration to draw inside the popup.
     * @param title Title text shown at the top of the popup.
     * @param text Body text shown beside the illustration.
     */
    void __cdecl Draw_Special_Info_Popup_(int16_t x, int16_t y, s_animation_header* anim, const char* title, const char* text);

    int16_t __cdecl Get_Colony_Best_Shield_Bldg_(int16_t colony_idx);

    int16_t __cdecl Colony_Selection_Popup_(
        s_invasion_side_info* attacker,
        s_invasion_side_info* defender,
        int32_t random_seed,
        int16_t colony_idx,
        int16_t bombardment_result,
        int8_t has_stellar_converter
    );

    void __cdecl Update_CCP_Display_Data_Invasion_(
        s_colony_combat_popup_data* ccp_data,
        s_invasion_side_info* attacker,
        s_invasion_side_info* defender,
        int32_t random_seed,
        int16_t colony_idx,
        int16_t bombardment_result,
        int8_t has_stellar_converter
    );

    void __cdecl Update_CCP_Display_Data_Bombing_(
        s_colony_combat_popup_data* ccp_data,
        s_combat_bomb_result* bomb,
        s_invasion_side_info* attacker,
        s_invasion_side_info* defender,
        int32_t random_seed,
        int16_t colony_idx,
        int16_t bombing_result,
        int8_t has_stellar_converter
    );

    /**
     * Runs the colony-combat popup loop until the user closes it or selects an action.
     * @param popup_data Display state used by the popup while it is active.
     * @param popup_mode Combat mode selector used to choose invasion or bombing behavior.
     */
    int16_t __cdecl Colony_Combat_Popup_(s_colony_combat_popup_data* popup_data, int16_t popup_mode);

    int16_t __cdecl Colony_Bombing_Popup_(
        s_combat_bomb_result* bomb,
        s_invasion_side_info* attacker,
        s_invasion_side_info* defender,
        int32_t random_seed,
        int16_t colony_idx,
        int16_t bombing_result,
        int8_t has_stellar_converter);

    int8_t __cdecl Player_Has_Colony_Or_Outpost_At_Star_(int16_t player_idx, int16_t star_idx);
}