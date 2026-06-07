#pragma once
#include <cstdint>

struct s_animation_header;
struct s_colors;
struct s_drop_area;
struct s_help_box;
struct s_help_topic_record;
struct s_list_item;
struct s_list_state;
struct s_tech_review_achievement;

namespace INFO {
    extern s_tech_review_achievement* _review_list[4];

    extern s_colors _info_normal_color;

    extern s_colors _info_high_color;

    extern int16_t _tech_rev_field[8];

    extern s_drop_area _tech_review_drops[4];

    extern uint16_t _list_sizes[4];

    extern s_list_state _tech_review_list_header;

    extern int16_t _tech_review_tab_index;

    extern void* _g_tech_image_seg;

    extern int16_t _g_last_list_field;

    extern uint16_t _g_selected_app_id;

    extern s_list_state* _g_header_p;

    extern s_animation_header* _g_bitmap1_p;

    extern s_animation_header* _g_bitmap2_p;

    extern int16_t _g_prior_choice;

    extern s_list_item* _g_selected_list_p;

    extern uint8_t _g_list_update_needed;

    extern int16_t _g_first_list_field;

    extern s_colors _info_red_color;

    extern uint8_t _sub_scr_field[10];

    extern uint8_t _g_reference_screen_type;

    extern uint8_t _g_turn_message_ix;

    extern int16_t _g_reference_ix;

    extern char (*_g_how_to_str_)[48];

    extern int16_t _g_first_how_to_field;

    extern int16_t _g_first_category_field;

    extern char (*_g_category_str_p)[48];

    extern uint8_t _race_statistics_page;

    extern s_drop_area _turn_summary_drop;

    extern s_drop_area _reference_drops[4];

    extern s_list_state _g_turn_header;

    extern s_list_state _g_category_header;

    extern uint16_t _g_exit_field;

    extern s_list_item* _g_selected_msg_p;

    extern s_drop_area _how_to_drops[2];
    extern s_drop_area _category_drops[3];
    extern s_drop_area _race_stats_drops[4];
    extern int16_t _race_bitmap_x[4];
    extern int16_t _race_bitmap_y[4];
    extern int16_t _race_statistics_specials_help_count;
    extern s_help_box _race_statistics_specials_help[50];

    /** Returns the currently selected player-info tab index. */
    int16_t __cdecl Get_Plyr_Info_Btns_();

    /** Fills the output array with normalized maintenance percentages for the active player. */
    int16_t __cdecl Initialize_Maintenance_Values_(int32_t* values);

    /** Builds the maintenance chart bar and label bitmaps from the supplied values. */
    void __cdecl Build_Maint_Chart_Bitmaps_(s_animation_header** sprites, s_animation_header** bars, s_animation_header** labels, int32_t* values);

    extern s_colors _info_dark_color;

    extern s_colors* _maintenance_bar_colors[7];

    extern s_colors* PTR__orange_text_colors[8];

    extern s_colors _hist_palettes[MAX_PLAYERS];

    extern s_colors* _hist_graph_palettes[MAX_PLAYERS];

    /** Recolors bitmap pixels whose indices fall within the supplied palette range. */
    void __cdecl Colorize_Bitmap_(s_animation_header* anim, s_colors* color_table, uint8_t range_start, uint8_t range_end);

    extern s_drop_area _history_graph_drops[2];
    extern int16_t _graph_button_x[4];

    extern uint8_t _graph_offsets[10];

    /** Draws the maintenance income chart and summary text for the active player. */
    void __cdecl Draw_Maint_Income_Chart_(s_animation_header** chart_bars, s_animation_header** chart_labels, int16_t visible_bar_count);

    /** Runs the history-graph tab until the caller's exit threshold is reached. */
    int16_t __cdecl History_Subscreen_(s_animation_header** lbx_images, int16_t exit_threshold);

    void __cdecl Set_Up_Player_Colors_(s_animation_header** anim_ptrs);

    int32_t __cdecl Smooth_History_Data_(int32_t* data, int16_t start_idx, int16_t end_idx);

    void __cdecl Get_Plyr_Hist_Btns_(int16_t* btns);

    void __cdecl Set_Plyr_Hist_Btns_(int16_t* btns_state);

    void __cdecl Draw_History_Subscreen_();

    void __cdecl Draw_Drop_Area_(s_drop_area* area);

    void __cdecl Draw_Multiple_Drop_Areas_(s_drop_area* areas, uint16_t count);

    void __cdecl Display_Graphed_Players_Names_();

    int16_t __cdecl Draw_The_History_Graph_();

    void __cdecl Draw_Histories_(int16_t offset_x);

    void __cdecl Introduce_Subscreen_();

    void __cdecl Draw_Tech_Review_Subscreen_();

    void __cdecl Init_Tech_Review_List_(s_list_state* state, int16_t review_idx);

    /** Runs the tech-review tab for the supplied player until the exit threshold is reached. */
    int16_t __cdecl _Tech_Review_Subscreen_(s_animation_header** anims, int16_t exit_threshold, int16_t player_idx);

    int16_t __cdecl Tech_Review_Subscreen_(s_animation_header** headers, int16_t exit_threshold);

    /** Runs the race-statistics tab until the caller's exit threshold is reached. */
    int16_t __cdecl Race_Stats_Subscreen_(s_animation_header** anims, int16_t exit_threshold);

    void __cdecl Print_Player_Specials_To_Bitmap_(int16_t x, int16_t y, int16_t player_idx, s_animation_header* bitmap);

    void __cdecl Race_Statistics_Specials_Help_(int16_t x, int16_t y, uint8_t player_idx);

    void __cdecl Draw_Race_Stats_Subscreen_();

    void __cdecl Draw_Dummy_Turn_Summary_Subscreen_();

    void __cdecl Draw_Turn_Summary_Subscreen_();

    void __cdecl _Init_Turn_Summary_List_(s_list_state* state, int16_t num_items, int16_t font_style, int16_t font_lf);

    void __cdecl Init_Turn_Summary_List_(s_list_state* state, int16_t val);

    int16_t __cdecl Turn_Summary_Subscreen_(s_animation_header** anim_data, int16_t min_field);

    /** Displays the reference index view and records which sub-view should open next. */
    int16_t __cdecl Reference_Main_Subscreen_(s_animation_header** unused_anims, int16_t exit_code);

    void __cdecl Draw_Reference_Main_Subscreen_();

    int16_t __cdecl Reference_How_To_Subscreen_(s_animation_header** anims, int16_t exit_code);

    void __cdecl Draw_Reference_Category_Subscreen_();

    int16_t __cdecl Reference_Category_Subscreen_(s_animation_header** anims, int16_t unused_exit_code);

    /** Dispatches to the active reference sub-view. */
    int16_t __cdecl Reference_Subscreen_(s_animation_header** anims, int16_t exit_threshold);

    void __cdecl Info_Screen_();
}