#pragma once

namespace CMBTDRW1 {
    inline const char* s_combat_lbx = reinterpret_cast<const char*>(0x00551244);
    inline uint8_t (&_green)[16] = *reinterpret_cast<uint8_t(*)[16]>(std::uintptr_t{0x0056CEF0});
    inline MOX2::action_func_t& _k_save = *reinterpret_cast<MOX2::action_func_t*>(std::uintptr_t{0x005960FC});

    inline int16_t& _g_max = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056CEE0});
    inline int16_t& _g_min = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056CEE4});
    inline char*& _g_msg = *reinterpret_cast<char**>(std::uintptr_t{0x0056CEE8});
    inline int16_t& _g_var = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056CF00});

    inline s_animation_header*& _board_capture_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005960F4});
    inline s_animation_header*& _board_raid_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005960F8});
    inline s_animation_header*& _board_left_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596100});
    inline s_animation_header*& _board_right_button_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596104});

    inline const char* s_B_005560e4 = reinterpret_cast<const char*>(std::uintptr_t{0x005560E4}); // "B"
    inline const char* s_L_005560e8 = reinterpret_cast<const char*>(std::uintptr_t{0x005560E8}); // "L"
    inline const char* s_S_005569cc = reinterpret_cast<const char*>(std::uintptr_t{0x005569CC}); // "S"
    inline const char* s_D_005569dc = reinterpret_cast<const char*>(std::uintptr_t{0x005569DC}); // "D"
    inline const char* s_A_00557ed4 = reinterpret_cast<const char*>(std::uintptr_t{0x00557ED4}); // "A"
    inline const char* s_I_0055801c = reinterpret_cast<const char*>(std::uintptr_t{0x0055801C}); // "I"
    inline const char* s_R_0055824c = reinterpret_cast<const char*>(std::uintptr_t{0x0055824C}); // "R"
    inline const char* s_F_005598f4 = reinterpret_cast<const char*>(std::uintptr_t{0x005598F4}); // "F"

    void __cdecl Detailed_View_Ship_(int16_t ship_idx);

    void __cdecl Draw_Board_Popup_();

    void __cdecl Draw_Internals_Display_(int16_t x_off, int16_t y_off);
    void __cdecl Full_Draw_Combat_Screen_();
    void __cdecl Draw_Legal_Boxes_();
    void __cdecl Draw_Ship_Status_(int16_t param_1, int16_t param_2, int16_t param_3);
    void __cdecl Draw_Grey_Damage_Bars_(int16_t x, int16_t y, int16_t ship_idx);
    void __cdecl Draw_Crew_Level_Stars_(int16_t ship_idx, int16_t x, int16_t y);
    void __cdecl Draw_Reduced_Combat_Map_();

    void __cdecl Draw_Reduced_Map_Box_();

    void __cdecl Draw_Fake_Combat_Buttons_();

    void __cdecl Draw_Option_Panel_Lights_();

    void __cdecl Draw_Blip2_(int16_t x, int16_t y, int16_t intensity_a, int16_t intensity_b);

    void __cdecl Draw_Blip3_(int16_t x, int16_t y, int16_t p3, int16_t p4);

    void __cdecl Draw_Blip4_(int16_t x, int16_t y, int16_t base_color, int16_t color_offset);

    void __cdecl Draw_Blip5_(int16_t x, int16_t y, int16_t color_base_a, int16_t color_base_b);

    void __cdecl Draw_Shield_Arcs_();

    void __cdecl Draw_Weapon_Status_Display_(int16_t param_1, int16_t param_2);

    void __cdecl Draw_Special_Status_Display_();

    void __cdecl Draw_Main_Combat_Screen_();

    void __cdecl Refresh_Combat_Screen_Full_();

    void __cdecl Draw_Super_Fast_Screen_();

    void __cdecl Move_Options_Background_();

    void __cdecl Display_Combat_View_Ship_(int16_t ship_idx);

    void __cdecl Scroll_Draw_Combat_Screen_();

    void __cdecl Create_Ship_Darken_Map_();

    int16_t __cdecl Ship_Completely_On_Screen_(int16_t ship_idx);

    int16_t __cdecl Relative_Bearing_XY_(int16_t ship_idx, int16_t target_x, int16_t target_y);

    int16_t __cdecl Relative_Bearing_(int16_t ship_a_idx, int16_t ship_b_idx);

    int16_t __cdecl Ship_On_Screen_(int16_t ship_idx);

    void __cdecl Draw_Ship_(int16_t param_1, int16_t param_2, int16_t param_3);

    void __cdecl Get_Draw_Ship_Offsets_(int16_t ship_idx, int16_t* out_x, int16_t* out_y);

    void __cdecl Draw_Ship_To_Bitmap_(int16_t ship_idx, int16_t layer, int16_t plasma_mode, int16_t plasma_progress);

    void __cdecl Draw_Retreating_Ship_(int16_t ship_idx);

    void __cdecl Get_Ship_Center_Draw_XY_(int16_t combat_idx, int16_t* x_out, int16_t* y_out);

    void __cdecl Get_Ship_Actual_Size_(int16_t ship_id, int16_t* width, int16_t* height);

    uint8_t __cdecl Get_Weapon_Arc_(int16_t bearing);

    int16_t __cdecl Board_Popup_(int16_t min_val, int16_t max_val, char* msg, int16_t* out_val);

    void __cdecl Plasma_Darken_(s_animation_header* bitmap, int16_t progress, int16_t mode);

    void __cdecl Draw_Bombs_(int16_t attacker_idx, int16_t target_idx, int16_t weapon_id, int16_t damage);

    void __cdecl Combat_Draw_View_Ship_();

    void __cdecl Remap_Draw_Ship_Status_(int16_t x, int16_t y);

    char* __cdecl Crew_Quality_String_(int16_t crew_quality);
}