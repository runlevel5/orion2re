#pragma once
#include <cstdint>

namespace MAINSCR {
    inline int16_t& _zoom_in_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059613C});
    inline int16_t& _zoom_out_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596108});
    inline int16_t& _whole_screen_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596134});
    inline int16_t& _game_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059613E});
    inline int16_t& _next_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005961F2});
    inline int16_t& _design_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596182});
    inline int16_t& _colonies_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059619C});
    inline int16_t& _planets_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005961A8});
    inline int16_t& _fleets_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596194});
    inline int16_t& _leaders_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059618A});
    inline int16_t& _races_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059610A});
    inline int16_t& _info_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005961F0});
    inline int16_t& _treasury_window_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596190});
    inline int16_t& _command_points_window_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596138});
    inline int16_t& _food_window_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005961FA});
    inline int16_t& _freighters_window_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005961FE});
    inline int16_t& _research_window_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596186});
    inline int16_t& _first_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596198});
    inline uint8_t& _debug_enabled = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596136});
    inline int16_t& _debug_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005961F4});
    inline int16_t& _exit_game_hot_key = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059619A});
    inline int16_t& _main_screen_grid_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596E78});
    inline int16_t& _main_screen_grid_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596D52});
    inline s_animation_header*& _temp_bitmap = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596130});
    inline int16_t& _main_screen_scanned_star = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056CEB0});
    inline int8_t& _moving_a_stack = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0056CEB4});
    inline int8_t& _track_time_flag = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0056CEC0});
    inline int8_t& _clear_fleet_fields = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0056CEC4});
    inline int8_t& _mouse_btn_pressed = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0056CEC8});
    inline int8_t& _merging_relocations = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0056CED0});
    inline int8_t& _warn_about_cheaters = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0056CED4});
    inline uint8_t& _zoom_box_counter = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x0056CEA4 });
    inline uint8_t& _zoom_box_color_increment = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x0056CEAC });
    inline uint8_t& _zoom_box_increment_value = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x0056CED8 });
    inline int8_t& _officer_screen_return_screen = *reinterpret_cast<int8_t*>(std::uintptr_t{ 0x00596184 });
    inline int8_t& _next_turn_hit = *reinterpret_cast<int8_t*>(std::uintptr_t{ 0x0059618C });
    inline int16_t& _zoom_box_x = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0059618E });
    inline int16_t& _zoom_box_y = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00596192 });
    inline int8_t& _fleet_ship_selection_status = *reinterpret_cast<int8_t*>(std::uintptr_t{0x005961FC});
    inline int16_t& _fleet_icon_chosen = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005961F6});

    inline s_hotseat_system_box_state (&_hotseat_system_box)[8] =
            *reinterpret_cast<s_hotseat_system_box_state(*)[8]>(std::uintptr_t{0x00596140});

    inline uint8_t (&_hotseat_map_scale)[8] =
            *reinterpret_cast<uint8_t (*)[8]>(std::uintptr_t{0x005961A0});

    inline s_hotseat_fleet_box_state (&_hotseat_fleet_box)[8] =
            *reinterpret_cast<s_hotseat_fleet_box_state(*)[8]>(std::uintptr_t{0x005961B0});

    inline int16_t& _hotseat_fleet_box_stack_ship_id(int32_t index) {
        return *reinterpret_cast<int16_t*>(
            std::uintptr_t{0x005961B6} + static_cast<std::uintptr_t>(index * sizeof(s_hotseat_fleet_box_state)));
    }

    inline int16_t& _moveable_box_0_id =
            *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597862});

    inline int16_t& _moveable_box_0_x =
            *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597868});

    inline int16_t& _moveable_box_0_y =
            *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059786A});

    inline int16_t& _moveable_box_0_drag_field =
            *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059786E});

    inline int16_t& _moveable_box_0_close_btn =
            *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597870});

    inline int16_t& _moveable_box_0_window_field_id =
            *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597872});

    inline int16_t& _moveable_box_0_content_offset_x =
            *reinterpret_cast<int16_t*>(std::uintptr_t{0x00597878});

    inline int16_t& _moveable_box_0_content_offset_y =
            *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059787A});

    inline int8_t& _star_name_highlight_direction =
        *reinterpret_cast<int8_t*>(std::uintptr_t{0x00559840});

    inline uint8_t& _star_name_highlight_value =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00559844});

    void* __cdecl Allocate_First_Screen_Seg_(int32_t p);

    bool __cdecl Is_Main_Screen_Palette_();

    bool __cdecl Ship_Can_Be_Selected_(int16_t ship_idx);

    void __cdecl Draw_Main_Screen_();

    void __cdecl Main_Screen_();

    inline uint8_t& _check_next_turn_exit_field = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0059613A});
    inline uint8_t* _twinkling_stars = reinterpret_cast<uint8_t*>(std::uintptr_t{0x00565460});
    inline int16_t& _n_twinkling_stars = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056CEA8});
    inline int16_t& _main_screen_ship_icon_scanned =
            *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596188});
    inline s_colors& _ship_direction_line_green_colors = *reinterpret_cast<s_colors*>(std::uintptr_t{0x00559830});
    inline s_colors& _ship_direction_line_red_colors = *reinterpret_cast<s_colors*>(std::uintptr_t{0x00559838});
    inline uint8_t& _fltpop_field_selection_status = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056CECC});
    inline int8_t (&_diplo_lights_anim_frame)[8] = *reinterpret_cast<int8_t(*)[8]>(std::uintptr_t{0x0056CEB8});
    inline s_animation_header* (&_player_lights_seg)[8] = *reinterpret_cast<s_animation_header*(*)[8]>(std::uintptr_t{0x00596110});

    void __cdecl Get_PointXY_Draw_Coords_(int16_t x, int16_t y, int16_t* out_x, int16_t* out_y);

    s_animation_header* __cdecl Get_Star_Picture_Seg_(int8_t spectral_class, int8_t pict_type, int8_t size);

    void __cdecl Get_Star_Draw_Coords_(int16_t star_idx, int16_t* out_x, int16_t* out_y);

    bool __cdecl Star_On_Screen_(int16_t star_idx);

    void __cdecl Draw_Stars_();

    void __cdecl Draw_A_Star_(int16_t star_idx);

    void __cdecl Update_Star_Owners_();

    void __cdecl Draw_Mini_Main_Screen_();

    void __cdecl Draw_Mini_Main_Offscreen_();

    void __cdecl Update_Ship_Icons_In_Between_Turns_();

    void __cdecl Draw_Wormhole_Links_();

    void __cdecl Draw_Paralax_();

    void __cdecl Draw_Relocation_Links_();

    void __cdecl Draw_Black_Holes_();

    void __cdecl Draw_Main_Main_Screen_();

    void __cdecl Print_Star_Name(int16_t star_id, int8_t simple_draw, char* star_name, void* buffer);

    bool __cdecl Print_Star_Name_To_Bitmap_(int16_t star_idx, char* str, s_animation_header** out_bitmap_ptr);

    void __cdecl Get_Star_Name_(int16_t star_idx, int8_t is_selected, int8_t* param_3, char* str, s_animation_header** param_5);

    inline uint8_t& _already_faded_into_main_screen = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596180});
    inline uint8_t& _in_auto_next_turn_mode = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005961F8});

    void __cdecl Add_Map_Fields_();

    void __cdecl Add_Star_Fields_();

    void __cdecl Add_Moveable_Box_Fields_();

    void __cdecl Do_System_Popup_(int16_t system_id);

    void __cdecl Clear_Main_Screen_Fields_();

    void __cdecl Draw_Exploding_Plasma_Box_(int16_t x_start, int16_t y_start, int16_t x_box, int16_t y_box, int16_t width, int16_t height, int16_t flag);

    void __cdecl Set_Popup_Draw_XY_(int16_t popup_type, int16_t ship_icon_idx, int16_t* out_w, int16_t* out_h);

    void __cdecl Fade_Into_Main_Screen_();

    void __cdecl Print_Star_Names_();

    void __cdecl Draw_ETA_Destination_Line_();

    void __cdecl Draw_Diplomacy_Request_Lights_();

    void __cdecl Draw_Player_Done_Lights_();

    void __cdecl Draw_Game_Turn_Buttons_();

    void __cdecl Draw_Main_Screen_Filled_(int16_t should_copy_to_back);

    void __cdecl Draw_A_Gate_Icon_(int16_t star_idx);

    void __cdecl Draw_Gate_Icons_();

    void __cdecl Draw_Nebulae_();

    void __cdecl Draw_Ship_Icons_(bool force_draw);

    void __cdecl Set_Post_Fitted_Font_Style_(uint8_t c1, uint8_t c2);

    void __cdecl Get_Research_Unit_String_(char* buffer);

    void __cdecl Print_Main_Screen_Data_();

    void __cdecl Draw_Main_Screen_Before_Stencil_();

    void __cdecl Popup_XY_(int16_t index, int16_t x, int16_t y, int16_t* out_w, int16_t* out_h);

    void __cdecl Set_Player_Cheated_Flag_(int16_t player_idx, int8_t cheated_this_turn);

    void __cdecl Give_Me_All_Tech_Advances_();

    inline uint16_t& _omniscience_cheat_alt = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00559860});
    inline int16_t& _all_tech_cheat = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00559864});
    inline int16_t& _leader_cheat = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00559868});
    inline int16_t& _more_money_cheat = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055986C});
    inline int16_t& _get_tech_cheat = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00559870});
    inline int16_t& _omniscience_cheat = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00559874});
    inline int16_t& _all_ai_cheat = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00559878});
    inline int16_t& _hi_score_cheat = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00559880});
    inline int16_t& _last_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056CEDC});

    inline const char* const s_EINSTEIN_005597e8 = reinterpret_cast<const char*>(std::uintptr_t{0x005597E8});
    inline const char* const s_MOOLA_005597f8 = reinterpret_cast<const char*>(std::uintptr_t{0x005597F8});
    inline const char* const s_MENLO_00559800 = reinterpret_cast<const char*>(std::uintptr_t{0x00559800});
    inline const char* const s_ISEEALL_00559808 = reinterpret_cast<const char*>(std::uintptr_t{0x00559808});
    inline const char* const s_SCORE_00559820 = reinterpret_cast<const char*>(std::uintptr_t{0x00559820});
    inline const char* s_Current_score_for_s_is_d = reinterpret_cast<const char*>(std::uintptr_t{0x00559908});
    inline const char* const s_fmt_star_len_00559984 = reinterpret_cast<const char*>(0x00559984);
    inline const char* s_ship_icon_idx_fmt = reinterpret_cast<const char*>(std::uintptr_t{0x00559974});

    void __cdecl Check_Hot_Keys_(int16_t key, int param_2);

    void __cdecl Add_Cheats_();

    void __cdecl Do_Colony_Screen_(int16_t colony_idx, int16_t* status_flag);

    void __cdecl Check_Stars_XY_(int16_t x, int16_t y, int8_t flag);

    void __cdecl Check_Ships_XY_(int16_t mouse_x, int16_t mouse_y, int8_t selection_id);

    void __cdecl Scan_Stars_XY_(int16_t mouse_x, int16_t mouse_y);

    void __cdecl Scan_Ships_XY_(int16_t mouse_x, int16_t mouse_y);

    int16_t __cdecl Get_Mouse_Field_(int16_t type, int16_t* out_field, int16_t* out_index);

    void __cdecl Exit_Zooming_Mode_(int16_t* ptr);

    void __cdecl Do_Fleet_Popup_(int16_t ship_icon_id, int8_t check_overlap);

    void __cdecl Set_Painted_Fleet_Fields_(int16_t field_to_select);

    void __cdecl Draw_Fleet_Box_Selected_Ship_Seg_();

    void __cdecl Draw_Scanned_Ship_Seg_();

    void __cdecl Print_Galaxy_Map_Scanned_Star_();

    void __cdecl Draw_Main_Screen_While_Moving_Boxes_();

    void __cdecl Check_Mouse_XY_Fields_();

    int16_t __cdecl Scan_System_Display_Fields_(int16_t mouse_input, int16_t input, int16_t *colony_btns, 
                                                int16_t *m_planet_idx, int16_t *planet_idx, 
                                                int16_t *m_ship_idx, int16_t *ship_idx, int16_t *selection);

    void __cdecl Draw_Main_Screen_For_Zooming_();

    int16_t __cdecl Get_New_Paralax_XYs_(int16_t target_map_x, int16_t target_map_y, int16_t scale);

    void __cdecl Clear_Twinkling_Star_Data_();

    void __cdecl Change_Home_Star_Name_();

    void __cdecl Warn_About_Cheating_Players_();

    int16_t __cdecl Do_Reply_To_Diplomacy_Request_(int16_t requester_id);

    int16_t __cdecl Check_For_Diplomacy_Request_();

    void __cdecl Check_Function_Keys_(int16_t key, int16_t* flag);

    void __cdecl Toggle_Game_Option_(int32_t option_idx);

    void __cdecl Do_Next_Turn_(int16_t* exit_flag, int8_t auto_mode);

    int16_t __cdecl Get_Ship_Stack_For_Officers_Screen_();

    int16_t __cdecl Get_Star_Id_For_Officers_Screen_();

    void __cdecl Tax_Rate_Popup_();

    void __cdecl Scroll_Map_(int16_t x, int16_t y);

    void __cdecl Set_Reload_Fleet_Box_Palette_Flag_();

    void __cdecl Print_Highlighted_Star_Name_(int16_t star_idx);

    void __cdecl Highlight_Star_(short star_idx);

    void __cdecl Redraw_Main_Screen_(uint8_t redraw_full);

    void __cdecl Load_Reply_To_Diplomacy_Request_Popup_();

    void __cdecl Add_Reply_To_Diplomacy_Request_Fields_(
        int16_t* accept_field,
        int16_t* invoke_field,
        int16_t* wait_field,
        int16_t* dismiss_field
    );

    void __cdecl Draw_Reply_To_Diplomacy_Request_();

    void __cdecl Main_Receive_Message_(uint32_t from_player_id, int16_t param_2, uint16_t msg_type, uint32_t len, void* data);

    int16_t __cdecl Scan_Galaxy_Map_Fields_(int16_t field_id_1, int16_t field_id_2, int16_t* star_idx_ptr_1, int16_t* star_idx_ptr_2);
    int __cdecl Zoom_Box_Width_(int16_t width, int16_t zoom_factor);
    int32_t __cdecl Zoom_Box_Height_(int16_t height, int16_t divisor);
    void __cdecl Cycle_Ship_Icons_(int16_t* ptr, int16_t* out_val, int16_t dir);
    void __cdecl Cycle_Star_Icons_(int16_t* p_star_idx, int16_t player_idx, int16_t direction);

    void __cdecl Center_Map_(int16_t x, int16_t y);

    inline s_fleet_box& _fleet_box = *reinterpret_cast<s_fleet_box*>(std::uintptr_t{0x005977C0});

    int16_t __cdecl Scan_Fleet_Movement_Box_Fields_(int16_t field_id_a, int16_t field_id_b, int16_t* out_fleet_icon_idx, int16_t* out_loop_counter);

    void __cdecl Scan_Moveable_Box_Fields(int16_t param_1, int16_t param_2, int32_t param_3, int16_t* param_4);

    void __cdecl Get_Star_Name_Info_(int16_t star_idx, int16_t* out_x, int16_t* out_y, int16_t* out_width);

    void __cdecl Mini_Main_Screen_Text_Box_(char* text_1, char* text_2);

    void __cdecl Wrapper_For_Normal_System_Fields_();

    void __cdecl Save_Hotseat_Map_Info_();
}