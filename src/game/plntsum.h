#pragma once

namespace PLNTSUM {
    inline const char* s__00551100 = reinterpret_cast<const char*>(std::uintptr_t{0x00551100}); // ""
    inline const char* s_plus = reinterpret_cast<const char*>(std::uintptr_t{0x00551118}); // "+"
    inline const char* s_minus = reinterpret_cast<const char*>(std::uintptr_t{0x0055111C}); // "-"
    inline const char* s_space = reinterpret_cast<const char*>(std::uintptr_t{0x00551120}); // " "
    inline const char* s_C = reinterpret_cast<const char*>(std::uintptr_t{0x00552420}); // "C"
    inline const char* s_O = reinterpret_cast<const char*>(std::uintptr_t{0x00556AFC}); // "O"
    inline const char* s_1 = reinterpret_cast<const char*>(std::uintptr_t{0x00556B10}); // "1"
    inline const char* s_2 = reinterpret_cast<const char*>(std::uintptr_t{0x00556B0C}); // "2"
    inline const char* s_3 = reinterpret_cast<const char*>(std::uintptr_t{0x00556B08}); // "3"
    inline const char* s_4 = reinterpret_cast<const char*>(std::uintptr_t{0x00556B04}); // "4"
    inline const char* s_5 = reinterpret_cast<const char*>(std::uintptr_t{0x00556B00}); // "5"

    inline int16_t (&_plntscrn_delta_y)[9] = *reinterpret_cast<int16_t(*)[9]>(std::uintptr_t{0x00556A78});
    inline int16_t& _sort_choice = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556ACC});
    inline int16_t& _filter_out_adverse_gravity_planets = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556AD0});
    inline int16_t& _filter_out_hostile_environment_planets = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556AD4});
    inline int16_t& _filter_out_mineral_scarcity_planets = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556AD8});
    inline int16_t& _filter_out_enemy_controlled_planets = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556ADC});
    inline int16_t& _filter_out_planets_out_of_range = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556AE0});

    inline s_plntsum_screen*& _screen = *reinterpret_cast<s_plntsum_screen**>(std::uintptr_t{0x0056582C});

    inline s_colors& _hi_neutral_colors = *reinterpret_cast<s_colors*>(std::uintptr_t{0x00556A60});
    inline s_colors& _red_colors = *reinterpret_cast<s_colors*>(std::uintptr_t{0x00556A68});
    inline s_colors& _green_colors = *reinterpret_cast<s_colors*>(std::uintptr_t{0x00556A70});

    inline s_plntsum_planet*& _planet_summary =
        *reinterpret_cast<s_plntsum_planet**>(std::uintptr_t{0x005657E8});

    inline s_plntsum_ship*& _ship_summary =
        *reinterpret_cast<s_plntsum_ship**>(std::uintptr_t{0x005658A0});

    inline int16_t& _n_ships_in_summary = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565898});
    extern int8_t _can_reach_star[72];

    inline uint8_t (&_explored_bitmap)[45] =
        *reinterpret_cast<uint8_t(*)[45]>(std::uintptr_t{0x005657F8});

    void __cdecl Reset_Explored_Planets_();

    inline uint8_t& _found_outpost_ship =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00565828});

    inline uint8_t& _draw_colony_button_status =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00565890});

    inline int16_t& _send_choice =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565894});

    inline uint8_t& _found_colony_ship =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005658AC});

    inline uint8_t& _draw_outpost_button_status =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005658C4});

    inline uint16_t& _plntsum_scroll_bar =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00565844});

    inline uint32_t& DAT_0056583c = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0056583C});

    inline uint8_t& DAT_00565840 = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00565840});

    void __cdecl Add_Planet_Summary_Bar_Field_();

    void __cdecl Add_Colony_Ship_Icon_Field_(int16_t* field_id, int16_t row_index);

    void __cdecl Add_Outpost_Ship_Icon_Field_(int16_t* field_id, int16_t row);

    void __cdecl Add_PlntSum_Galaxy_Map_Fields_(int16_t* fields_array);

    void __cdecl Add_Plntsum_Fields_();

    void __cdecl Planet_Summary_Screen_();

    bool __cdecl Send_Colony_Button_Selectable_();
    bool __cdecl Send_Outpost_Button_Selectable_();
    bool __cdecl Summary_Found_Colony_Ship_(short ship_index);
    bool __cdecl Summary_Found_Outpost_Ship_(short index);
    bool __cdecl Planet_Is_Outpost_Planet_(short planet_idx);
    bool __cdecl Planet_Has_Players_Colony_(int16_t planet_idx);
    bool __cdecl Planet_Has_Been_Visited_(int16_t planet_index);
    bool __cdecl Planet_In_Range_Of_A_Colony_(short planet_index);

    int16_t __cdecl Productivity_Penalty_(int16_t player_idx, int16_t planet_gravity);

    int16_t __cdecl First_Planet_In_Summary_List_(int16_t star_index);

    int16_t __cdecl Summary_Planet_Index_(int16_t star_index);

    int16_t __cdecl Get_ETA_To_Star_(int32_t star_idx);

    void __cdecl Set_Planet_Explored_Flags_();

    bool __cdecl Planet_Has_Adverse_Gravity_(int16_t planet_idx, int16_t player_idx);
    bool __cdecl Planet_Is_Enemy_Controlled_(int16_t planet_idx, int16_t player_idx);
    bool __cdecl Planet_Has_Hostile_Environment_(short planet_idx);

    bool __cdecl Planet_Is_Mineral_Scarce_(short planet_idx);
    int16_t __cdecl Colony_Ship_Auto_Colonization_(s_ship_data* ship);
    void __cdecl Set_Auto_Colonize_Flag_(s_ship_data* ship, int16_t flag);
    bool __cdecl Ship_Is_Not_Colonizing_(s_ship_data* ship);

    int16_t __cdecl Planet_Summary_List_Id_(int16_t planet_idx);

    void __cdecl Set_Colony_Ship_To_Star_Distances_(int16_t summary_idx, int16_t ship_idx);

    void __cdecl Set_Outpost_Ship_To_Star_Distances_(int16_t summary_idx, int16_t ship_idx);

    void __cdecl Update_Ship_Etas_();

    inline int16_t& _last_ship_moved =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x005658C8});

    inline char*& PTR_s_PLNTSUM_LBX_00556AC0 =
        *reinterpret_cast<char**>(std::uintptr_t{0x00556AC0});

    inline s_animation_header*& _send_colony_ship_button_seg =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00556AC4});

    inline s_animation_header*& _send_outpost_ship_button_seg =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00556AC8});

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

    inline bool& _can_reach_scanned_planet =
        *reinterpret_cast<bool*>(std::uintptr_t{0x00565830});

    inline int8_t& _box_field_scanned =
        *reinterpret_cast<int8_t*>(std::uintptr_t{0x005658BC});

    void __cdecl Scan_Summary_Box_Fields_(int16_t input_field_id, int16_t clicked_field_id, int16_t* last_input_field_id_ptr);

    void __cdecl Wrapper_For_Planet_Summary_Galaxy_Map_();

    void __cdecl Draw_Nearest_Ship_Directional_Line_(int16_t star_idx);

    void __cdecl Get_Colony_Outpost_Marker_XY_(int16_t summary_idx, int16_t* out_x, int16_t* out_y);

    void __cdecl Print_Colony_Outpost_Marker_(short x, short y, short index, int8_t colony, int8_t outpost);

    inline bool& _recalc_first_planet =
        *reinterpret_cast<bool*>(std::uintptr_t{0x005658C0});

    inline uint8_t& _fade_flag =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056589C});

    inline uint8_t& DAT_005658a4 = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005658A4});

    inline uint8_t& _skip_draw_planets =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005658A8});

    void __cdecl Fill_Summary_Screen_Scroll_Bar_();

    inline s_animation_header**& _p_rotating_planet =
        *reinterpret_cast<s_animation_header***>(std::uintptr_t{ 0x00565838 });

    void __cdecl Draw_Rotating_Planets_();

    s_animation_header* __cdecl Load_Planet_Segment_(int16_t param_1);

    void __cdecl Summary_Screen_Print_Data_();

    void __cdecl Draw_Colony_ETA_(int16_t planet_sum_idx, int16_t scroll_idx);
    void __cdecl Draw_Outpost_ETA_(int16_t planet_sum_idx, int16_t scroll_idx);

    void __cdecl Player_Colors_(int16_t player_idx, s_colors* colors_out, int8_t type, uint8_t* base_color_out);

    void __cdecl Colony_Owner_Colors_(int16_t colony_idx, s_colors* colors, char param_3);

    void __cdecl Summary_Screen_Print_Scanned_();

    void __cdecl Draw_Mini_Planet_Summary_Screen_();
}