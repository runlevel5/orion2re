#pragma once

namespace CMBTDRW1 {
    extern const char s_combat_lbx[11];
    extern uint8_t _green[16];
    extern MOX2::action_func_t _k_save;

    extern int16_t _g_max;
    extern int16_t _g_min;
    extern char* _g_msg;
    extern int16_t _g_var;

    extern s_animation_header* _board_capture_button_seg;
    extern s_animation_header* _board_raid_button_seg;
    extern s_animation_header* _board_left_button_seg;
    extern s_animation_header* _board_right_button_seg;

    void __cdecl Detailed_View_Ship_(int16_t ship_idx);

    void __cdecl Draw_Board_Popup_();

    void __cdecl Draw_Internals_Display_(int16_t x_off, int16_t y_off);
    void __cdecl Full_Draw_Combat_Screen_();
    /** Highlights reachable combat-grid cells for the currently selected ship. */
    void __cdecl Draw_Legal_Boxes_();
    /** Draws the compact status panel for the given ship or planet at the requested panel position. */
    void __cdecl Draw_Ship_Status_(int16_t x, int16_t y, int16_t ship_idx);
    void __cdecl Draw_Grey_Damage_Bars_(int16_t x, int16_t y, int16_t ship_idx);
    void __cdecl Draw_Crew_Level_Stars_(int16_t ship_idx, int16_t x, int16_t y);
    void __cdecl Draw_Reduced_Combat_Map_();

    void __cdecl Draw_Reduced_Map_Box_();

    void __cdecl Draw_Fake_Combat_Buttons_();

    void __cdecl Draw_Option_Panel_Lights_();

    void __cdecl Draw_Blip2_(int16_t x, int16_t y, int16_t intensity_a, int16_t intensity_b);

    void __cdecl Draw_Blip3_(int16_t x, int16_t y, int16_t base_color, int16_t color_offset);

    void __cdecl Draw_Blip4_(int16_t x, int16_t y, int16_t base_color, int16_t color_offset);

    void __cdecl Draw_Blip5_(int16_t x, int16_t y, int16_t color_base_a, int16_t color_base_b);

    /** Draws directional shield-arc labels for the currently selected ship. */
    void __cdecl Draw_Shield_Arcs_();

    /** Draws the current weapon list, highlighting legality against the hovered ship or missile target. */
    void __cdecl Draw_Weapon_Status_Display_(int16_t target_ship_idx, int16_t target_missile_idx);

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

    /** Draws a combat ship or planet sprite at the supplied screen position. */
    void __cdecl Draw_Ship_(int16_t ship_idx, int16_t draw_x, int16_t draw_y);

    void __cdecl Get_Draw_Ship_Offsets_(int16_t ship_idx, int16_t* out_x, int16_t* out_y);

    void __cdecl Draw_Ship_To_Bitmap_(int16_t ship_idx, int16_t layer, int16_t plasma_mode, int16_t plasma_progress);

    void __cdecl Draw_Retreating_Ship_(int16_t ship_idx);

    void __cdecl Get_Ship_Center_Draw_XY_(int16_t combat_idx, int16_t* x_out, int16_t* y_out);

    void __cdecl Get_Ship_Actual_Size_(int16_t ship_id, int16_t* width, int16_t* height);

    uint8_t __cdecl Get_Weapon_Arc_(int16_t bearing);

    int16_t __cdecl Board_Popup_(int16_t min_val, int16_t max_val, char* msg, int16_t* out_val);

    /** Applies the animated plasma darkening mask to a ship bitmap for the requested effect mode. */
    void __cdecl Plasma_Darken_(s_animation_header* bitmap, int16_t progress, int16_t mode);

    void __cdecl Draw_Bombs_(int16_t attacker_idx, int16_t target_idx, int16_t weapon_id, int16_t damage);

    void __cdecl Combat_Draw_View_Ship_();

    void __cdecl Remap_Draw_Ship_Status_(int16_t x, int16_t y);

    char* __cdecl Crew_Quality_String_(int16_t crew_quality);
}