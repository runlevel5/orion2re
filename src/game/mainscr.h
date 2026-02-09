#pragma once
#include <cstdint>

namespace MAINSCR {
    inline int16_t& _zoom_in_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059613C});
    inline int16_t& _zoom_out_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596108});
    inline int16_t& _whole_screen_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596134});
    inline int16_t& _game_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059613E});
    inline int16_t& _next_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005961F2});
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
    inline void*& _temp_bitmap = *reinterpret_cast<void**>(std::uintptr_t{0x00596130});
    inline int16_t& _main_screen_scanned_star = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056CEB0});
    inline int8_t& _moving_a_stack = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0056CEB4});
    inline int8_t& _track_time_flag = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0056CEC0});
    inline int8_t& _clear_fleet_fields = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0056CEC4});
    inline int8_t& _mouse_btn_pressed = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0056CEC8});
    inline int8_t& _merging_relocations = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0056CED0});
    inline int8_t& _warn_about_cheaters = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0056CED4});
    inline int8_t& _zoom_box_increment_value = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0056CED8});
    inline int8_t& _officer_screen_return_screen = *reinterpret_cast<int8_t*>(std::uintptr_t{0x00596184});
    inline int8_t& _next_turn_hit = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0059618C});
    inline uint16_t& _zoom_box_x = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0059618E});
    inline uint16_t& _zoom_box_y = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00596192});
    inline int8_t& _fleet_ship_selection_status = *reinterpret_cast<int8_t*>(std::uintptr_t{0x005961FC});

    void* __cdecl Allocate_First_Screen_Seg_(int32_t p);

    bool __cdecl Is_Main_Screen_Palette_();

    inline uint8_t __cdecl Ship_Can_Be_Selected_(int16_t ship_idx) {
        using Fn = uint8_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004CD0F0});
        return fn(ship_idx);
    }

    void __cdecl Draw_Main_Screen_();

    void __cdecl Main_Screen_();

    inline void __cdecl Add_Fleet_Movement_Box_Fields_(int16_t node_ptr, int32_t flag) {
        using Fn = void(__cdecl*)(int16_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004039A0 });
        fn(node_ptr, flag);
    }

    inline uint8_t& _check_next_turn_exit_field = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0059613A});
    inline int16_t& _n_twinkling_stars = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056CEA8});
    inline int16_t& _main_screen_ship_icon_scanned =
            *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596188});
    inline uint8_t (&_ship_direction_line_red_colors)[8] = *reinterpret_cast<uint8_t(*)[8]>(std::uintptr_t{0x00559830});
    inline uint8_t (&_ship_direction_line_green_colors)[8] = *reinterpret_cast<uint8_t(*)[8]>(std::uintptr_t{0x00559838});
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

    bool __cdecl Print_Star_Name_To_Bitmap_(int16_t star_idx, char* str, void* out_bitmap_ptr);

    void __cdecl Get_Star_Name_(int16_t star_idx, int8_t is_selected, int8_t* param_3, char* str, void* param_5);

    inline uint8_t& _already_faded_into_main_screen = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596180});

    void __cdecl Add_Map_Fields_();

    void __cdecl Add_Moveable_Box_Fields_();

    void __cdecl Do_System_Popup_(int16_t system_id);

    inline void __cdecl Clear_Main_Screen_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004c4b00});
        fn();
    }

    inline void __cdecl Draw_Exploding_Plasma_Box_(int16_t x_start, int16_t y_start, uint16_t x_box, uint16_t y_box, uint16_t width, uint16_t height, int16_t flag) {
        using Fn = void(__cdecl*)(int16_t, int16_t, uint16_t, uint16_t, uint16_t, uint16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004c5220});
        fn(x_start, y_start, x_box, y_box, width, height, flag);
    }

    void __cdecl Set_Popup_Draw_XY_(int16_t popup_type, int16_t ship_icon_idx, int16_t* out_w, int16_t* out_h);

    void __cdecl Fade_Into_Main_Screen_();

    void __cdecl Print_Star_Names_();

    void __cdecl Draw_ETA_Destination_Line_();

    void __cdecl Draw_Diplomacy_Request_Lights_();

    inline void __cdecl Draw_Player_Done_Lights_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004C6B40})();
    }

    void __cdecl Draw_Main_Screen_Filled_(int16_t should_copy_to_back);

    void __cdecl Draw_A_Gate_Icon_(int16_t star_idx);

    void __cdecl Draw_Gate_Icons_();

    void __cdecl Draw_Nebulae_();

    void __cdecl Draw_Ship_Icons_(bool force_draw);

    void __cdecl Set_Post_Fitted_Font_Style_(uint8_t c1, uint8_t c2);

    void __cdecl Get_Research_Unit_String_(char* buffer);

    void __cdecl Print_Main_Screen_Data_();

    inline void __cdecl Draw_Main_Screen_Before_Stencil_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004C64B0});
        fn();
    }

    void __cdecl Popup_XY_(int16_t index, int16_t x, int16_t y, int16_t* out_w, int16_t* out_h);

    void __cdecl Set_Player_Cheated_Flag_(int16_t player_idx, int8_t cheated_this_turn);

    void __cdecl Give_Me_All_Tech_Advances_();

    inline uint16_t& _omniscience_cheat_alt = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00559860});
    inline int16_t& _all_tech_cheat = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00559864});
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

    void __cdecl Check_Hot_Keys_(int16_t key, int param_2);

    void __cdecl Add_Cheats_();

    void __cdecl Do_Colony_Screen_(int16_t colony_idx, int16_t* status_flag);

    int16_t __cdecl Current_Colony_();

    void __cdecl Check_Stars_XY_(int16_t x, int16_t y, int8_t flag);

    void __cdecl Check_Ships_XY_(int16_t mouse_x, int16_t mouse_y, int8_t selection_id);

    inline void __cdecl Scan_Stars_XY_(int16_t x, int16_t y) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004cc700});
        fn(x, y);
    }

    inline void __cdecl Scan_Ships_XY_(int16_t x, int16_t y) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004cc5b0});
        fn(x, y);
    }

    int16_t __cdecl Get_Mouse_Field_(int16_t type, int16_t* out_field, int16_t* out_index);

    void __cdecl Exit_Zooming_Mode_(int16_t* ptr);

    void __cdecl Do_Fleet_Popup_(int16_t ship_icon_id, int8_t check_overlap);

    inline int16_t __cdecl Mouse_Button_() {
        using Fn = int16_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x00527680})();
    }

    inline void __cdecl Set_Painted_Fleet_Fields_(int16_t field) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004CD040})(field);
    }

    void __cdecl Draw_Fleet_Box_Selected_Ship_Seg_();

    void __cdecl Draw_Scanned_Ship_Seg_();

    void __cdecl Print_Galaxy_Map_Scanned_Star_();

    void __cdecl Draw_Main_Screen_While_Moving_Boxes_();

    void __cdecl Check_Mouse_XY_Fields_();

    int16_t __cdecl Scan_System_Display_Fields_(int16_t mouse_input, int16_t input, int16_t *colony_btns, 
                                                int16_t *m_planet_idx, int16_t *planet_idx, 
                                                int16_t *m_ship_idx, int16_t *ship_idx, int16_t *selection);

    inline void __cdecl Scan_Moveable_Box_Fields(int16_t skip, int16_t key, int16_t scan, uint16_t* flag) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, uint16_t*);
        reinterpret_cast<Fn>(0x004CBED0)(skip, key, scan, flag);
    }

    inline void __cdecl Decrement_Zoom_Count_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x004F0840)();
    }

    inline int16_t __cdecl Increment_Zoom_Count_With_Limit_() {
        using Fn = int16_t(__cdecl*)();
        return reinterpret_cast<Fn>(0x004F0E50)();
    }

    inline void __cdecl Increment_Zoom_Count_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x004F0E20)();
    }

    inline int16_t __cdecl Get_Map_Scale_(int16_t zoom) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(0x004F0880)(zoom);
    }

    inline void __cdecl Draw_Main_Screen_For_Zooming_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x004C9C30)();
    }

    int16_t __cdecl Get_New_Paralax_XYs_(int16_t target_map_x, int16_t target_map_y, int16_t scale);

    void __cdecl Clear_Twinkling_Star_Data_();

    void __cdecl Change_Home_Star_Name_();

    void __cdecl Warn_About_Cheating_Players_();

    int16_t __cdecl Do_Reply_To_Diplomacy_Request_(int16_t requester_id);

    int16_t __cdecl Check_For_Diplomacy_Request_();

    inline void __cdecl Check_Function_Keys_(int16_t key, int16_t* flag) {
        using Fn = void(__cdecl*)(int16_t, int16_t*);
        reinterpret_cast<Fn>(0x004C8D00)(key, flag);
    }

    inline void __cdecl Do_Next_Turn_(int16_t* ptr, int8_t unk) {
        using Fn = void(__cdecl*)(int16_t*, int8_t);
        reinterpret_cast<Fn>(0x004C4E90)(ptr, unk);
    }

    int16_t __cdecl Get_Ship_Stack_For_Officers_Screen_();

    int16_t __cdecl Get_Star_Id_For_Officers_Screen_();

    void __cdecl Tax_Rate_Popup_();

    void __cdecl Scroll_Map_(int16_t x, int16_t y);

    void __cdecl Set_Reload_Fleet_Box_Palette_Flag_();

    inline void __cdecl Print_Highlighted_Star_Name_(short star_idx) {
        using Fn = void(__cdecl*)(short);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ca360});
        fn(star_idx);
    }

    void __cdecl Highlight_Star_(short star_idx);

    void __cdecl Redraw_Main_Screen_(uint8_t redraw_full);
}