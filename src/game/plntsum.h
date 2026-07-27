#pragma once

struct s_animation_header;

typedef struct s_plntsum_screen s_plntsum_screen, *Ps_plntsum_screen;

struct s_plntsum_screen {
    struct s_animation_header *pics[14];
    int16_t buttons[10];
    int16_t fields[8];
    int16_t colony_icons[8];
    int16_t output_icons[8];
    int16_t up_button;
    int16_t dn_button;
    int16_t esc_button;
    int16_t planet_icons[MAX_STARS];
    bool refresh_fields;
    int16_t scanned_input;
};

namespace PLNTSUM {
    extern int16_t _plntscrn_delta_y[9];
    extern int16_t _sort_choice;
    extern int16_t _filter_out_adverse_gravity_planets;
    extern int16_t _filter_out_hostile_environment_planets;
    extern int16_t _filter_out_mineral_scarcity_planets;
    extern int16_t _filter_out_enemy_controlled_planets;
    extern int16_t _filter_out_planets_out_of_range;

    extern s_plntsum_screen* _screen;

    extern s_colors _hi_neutral_colors;
    extern s_colors _red_colors;
    extern s_colors _green_colors;

    extern s_plntsum_planet* _planet_summary;

    extern s_plntsum_ship* _ship_summary;

    extern int16_t _n_ships_in_summary;
    extern int8_t _can_reach_star[MAX_STARS];

    extern uint8_t _explored_bitmap[BITMAP(MAX_PLANETS)];

    void __cdecl Reset_Explored_Planets_();

    extern uint8_t _found_outpost_ship;

    extern uint8_t _draw_colony_button_status;

    extern int16_t _send_choice;

    extern uint8_t _found_colony_ship;

    extern uint8_t _draw_outpost_button_status;

    extern int16_t _plntsum_scroll_bar;

    extern uint32_t _planet_summary_reserved_counter;

    extern uint8_t _planet_summary_refresh_marker;

    void __cdecl Add_Planet_Summary_Bar_Field_();

    void __cdecl Add_Colony_Ship_Icon_Field_(int16_t* field_id, int16_t row_index);

    void __cdecl Add_Outpost_Ship_Icon_Field_(int16_t* field_id, int16_t row_index);

    void __cdecl Add_PlntSum_Galaxy_Map_Fields_(int16_t* fields_array);

    void __cdecl Add_Plntsum_Fields_();

    void __cdecl Planet_Summary_Screen_();

    bool __cdecl Send_Colony_Button_Selectable_();
    bool __cdecl Send_Outpost_Button_Selectable_();
    bool __cdecl Summary_Found_Colony_Ship_(int16_t ship_summary_idx);
    bool __cdecl Summary_Found_Outpost_Ship_(int16_t ship_summary_idx);
    bool __cdecl Planet_Is_Outpost_Planet_(int16_t planet_idx);
    bool __cdecl Planet_Has_Players_Colony_(int16_t planet_idx);
    bool __cdecl Planet_Has_Been_Visited_(int16_t planet_index);
    bool __cdecl Planet_In_Range_Of_A_Colony_(int16_t planet_idx);

    int16_t __cdecl Productivity_Penalty_(int16_t player_idx, int16_t planet_gravity);

    int16_t __cdecl First_Planet_In_Summary_List_(int16_t star_index);

    int16_t __cdecl Summary_Planet_Index_(int16_t star_index);

    int16_t __cdecl Get_ETA_To_Star_(int16_t star_idx);

    void __cdecl Set_Planet_Explored_Flags_();

    bool __cdecl Planet_Has_Adverse_Gravity_(int16_t planet_idx, int16_t player_idx);
    bool __cdecl Planet_Is_Enemy_Controlled_(int16_t planet_idx, int16_t player_idx);
    bool __cdecl Planet_Has_Hostile_Environment_(int16_t planet_idx);

    bool __cdecl Planet_Is_Mineral_Scarce_(int16_t planet_idx);
    int16_t __cdecl Colony_Ship_Auto_Colonization_(s_ship_data* ship);
    void __cdecl Set_Auto_Colonize_Flag_(s_ship_data* ship, int16_t flag);
    bool __cdecl Ship_Is_Not_Colonizing_(s_ship_data* ship);

    int16_t __cdecl Planet_Summary_List_Id_(int16_t planet_idx);

    void __cdecl Set_Colony_Ship_To_Star_Distances_(int16_t summary_idx, int16_t ship_idx);

    void __cdecl Set_Outpost_Ship_To_Star_Distances_(int16_t summary_idx, int16_t ship_idx);

    void __cdecl Update_Ship_Etas_();

    extern int16_t _last_ship_moved;

    extern const char* PTR_s_PLNTSUM_LBX_00556AC0;

    extern s_animation_header* _send_colony_ship_button_seg;

    extern s_animation_header* _send_outpost_ship_button_seg;

    void __cdecl Load_Player_Icon_();

    void __cdecl Load_Planet_Summary_Pictures_();

    void __cdecl Load_Planet_Pictures_();

    void __cdecl Allocate_Space_For_Summary_Screen_();

    void __cdecl Check_For_Colony_And_Outpost_Ships_();

    void __cdecl Determine_Button_Status_();

    void __cdecl Build_Ship_Summary_();

    int __cdecl qsort_by_climate_(const void* a, const void* b);
    int __cdecl qsort_by_minerals_(const void* a, const void* b);
    int __cdecl qsort_by_max_population_(const void* a, const void* b);

    void __cdecl Sort_Planet_Summary_();
    void __cdecl Filter_Explored_Planets_(int16_t* summary_count);
    int16_t __cdecl Planet_Summary_Id_(int16_t explored_id);

    int16_t __cdecl Nearest_Ship_Id_(int16_t index);

    void __cdecl Send_Nearest_Ship_(int16_t explored_idx);

    void __cdecl Reset_Ship_To_Star_Distances_();

    void __cdecl Undo_Send_Order_(int16_t planet_idx, int16_t order_type);

    extern bool _can_reach_scanned_planet;

    extern int8_t _box_field_scanned;

    void __cdecl Scan_Summary_Box_Fields_(int16_t input_field_id, int16_t clicked_field_id, int16_t* last_input_field_id_ptr);

    void __cdecl Wrapper_For_Planet_Summary_Galaxy_Map_();

    void __cdecl Draw_Nearest_Ship_Directional_Line_(int16_t star_idx);

    void __cdecl Get_Colony_Outpost_Marker_XY_(int16_t summary_idx, int16_t* out_x, int16_t* out_y);

    void __cdecl Print_Colony_Outpost_Marker_(int16_t x, int16_t y, int16_t ship_summary_idx, int8_t colony, int8_t outpost);

    extern bool _recalc_first_planet;

    extern uint8_t _fade_flag;

    extern uint8_t _planet_summary_state_marker;

    extern uint8_t _skip_draw_planets;

    void __cdecl Fill_Summary_Screen_Scroll_Bar_();

    extern s_animation_header** _p_rotating_planet;

    void __cdecl Draw_Rotating_Planets_();

    s_animation_header* __cdecl Load_Planet_Segment_(int16_t planet_anim_idx);

    void __cdecl Summary_Screen_Print_Data_();

    void __cdecl Draw_Colony_ETA_(int16_t planet_sum_idx, int16_t scroll_idx);
    void __cdecl Draw_Outpost_ETA_(int16_t planet_sum_idx, int16_t scroll_idx);

    void __cdecl Player_Colors_(int16_t player_idx, s_colors* colors_out, int8_t type, uint8_t* base_color_out);

    void __cdecl Colony_Owner_Colors_(int16_t colony_idx, s_colors* colors, int8_t highlight_type);

    void __cdecl Summary_Screen_Print_Scanned_();

    void __cdecl Draw_Mini_Planet_Summary_Screen_();
}