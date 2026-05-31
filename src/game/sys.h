#pragma once

namespace SYS {
    extern void* _sys_data_buffer;
    extern const char s_fmt_star_display[7]; // "%d> %s"
    extern const char s_1[3];
    extern const char s_percent_d_nl[6]; // "%d%%\n"
    extern const char s_0s_1[7];
    extern const char s_0s[5];
    extern const char s_m25_17[9];
    extern const char s_0s_1_dash[10];
    extern const char s_m25_27[9];
    extern const char s_0s_1d_nl[10];
    extern const char s_0s_10_nl[9];
    extern const char s_d_nl[4]; // "%d\n"
    extern const char s_m25_30_0s[13];

    extern uint8_t _draw_sun_before_planet;
    extern int8_t _skip_draw_system_planets;
    extern const char s__00551120[2];
    extern uint8_t _skip_draw_system_display;
    extern s_animation_header* _system_display_sun_seg;
    extern void* _sys_disp_bitmap;

    void __cdecl Clear_System_Display_Loaded_Flag_();

    void __cdecl Clear_System_Display_Ship_Fields_();

    bool __cdecl Need_To_Reload_System_Display_(int16_t star_id);

    void __cdecl Set_System_Display_Loaded_Flag_(int16_t star_idx);

    void __cdecl Load_New_System_Display_(int16_t star_idx);

    void __cdecl Init_System_Display_(int16_t star_id);

    void __cdecl Potential_Colony_Info_Popup_(int16_t planet_idx);

    void __cdecl Get_Rotating_Planet_Segment_(s_animation_header** out_ptr, int16_t planet_idx, int16_t arg3, int32_t arg4);

    extern int8_t _remap_gray_percentages[10];

    extern uint8_t _remap_gray_mask_palette[10][256];

    void __cdecl Calculate_Remap_Gray_Palettes_();

    int __cdecl qsort_sys_disp_(const void* a, const void* b);

    bool __cdecl Ok_To_View_System_(int16_t star_idx, int16_t player_idx);

    void __cdecl N_Ships_By_Type_At_Location_(int16_t x, int16_t y, int16_t target_owner, int16_t *total_count, int16_t *bases_count, int16_t *colony_ships_count, int16_t *transports_count, int16_t *ship_sizes);

    void __cdecl Hide_System_Display_(int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, int16_t x_offset, int16_t y_offset, char mode);

    void __cdecl Draw_System_Display_Border_(int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, int16_t x_offset, int16_t y_offset, int16_t mode);

    void __cdecl Draw_System_Special_Popup_(int16_t x, int16_t y, int16_t special_idx, int16_t anim_idx);

    void __cdecl Draw_Sun_Seg_(int16_t star_idx, int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, int16_t y_offset, int16_t x_offset);

    void __cdecl Draw_System_Display_Planets_(int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, int16_t y_offset, int16_t x_offset);

    void __cdecl Draw_Planet_With_Pierced_Ring_(int16_t planet_idx, int16_t sys_disp_idx, int16_t x_offset_1, int16_t y_offset_1, int16_t x_offset_2, int16_t y_offset_2, int16_t y_offset_3, int16_t x_offset_3);

    void __cdecl Remap_Draw_With_Mask_(int16_t x, int16_t y, s_animation_header* anim_hdr, s_animation_header* mask_hdr);

    extern s_animation_header* _mask_header;

    void __cdecl Remap_Draw_Animated_Sprite_Mask_(
        int16_t x,
        int16_t y,
        const uint8_t* sprite_data,
        const int8_t* mask_data);

    void __cdecl Remap_Clipped_Draw_Animated_Sprite_Mask_(int16_t x, int16_t y, const uint8_t* encoded_sprite, const int8_t* mask_bitmap);

    void __cdecl Draw_Colony_Marker_(int16_t x, int16_t y, int16_t owner);

    void __cdecl Draw_Outpost_Marker_(int16_t x, int16_t y, int16_t owner);

    void __cdecl Draw_System_Highlight_(int16_t planet_idx, int16_t rect_w, int16_t rect_h, int16_t x_offset, int16_t y_offset);

    void __cdecl Print_Planet_Info_Short_(int16_t planet_idx, int16_t* box_width, int16_t* box_height);

    int16_t __cdecl Print_Special_Building_Data_(
            int16_t colony_idx,
            int16_t x,
            int16_t* line_pos,
            int16_t* max_text_width,
            int16_t* total_height,
            int16_t line_spacing,
            bool draw);

    void __cdecl Do_Print_Planet_Info_(
            int16_t left,
            int16_t top,
            int16_t inner_left,
            int16_t inner_top,
            int16_t planet_idx,
            int16_t text_left,
            int16_t text_top,
            int16_t* max_width,
            int16_t* total_height,
            bool draw);

    void __cdecl Print_Planet_Info_(int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7);

    void __cdecl Draw_System_Ship_Icons_(int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, int16_t y_offset, int16_t x_offset);

    void __cdecl Measure_System_Ship_Data_(int16_t x, int16_t y, int16_t w, int16_t h, int16_t p5, int16_t* out_w, int16_t* out_h);

    void __cdecl Do_Print_System_Ship_Data_(
        int16_t base_x,
        int16_t base_y,
        int16_t x_offset,
        int16_t y_offset,
        int16_t ship_node_num,
        int16_t* width_out,
        int16_t* height_out,
        bool draw_text
    );

    void __cdecl Print_System_Ship_Data_(int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, int16_t ship_idx);

    void __cdecl Build_System_Popup_Ships_(int16_t star_idx);

    void __cdecl Get_System_Display_Ship_Icon_XY_(
            int32_t unused,
            int16_t display_slot,
            int16_t* x,
            int16_t* y,
            int16_t* layout_state);

    s_animation_header* __cdecl Buffer_Reload_System_Ships_(int16_t display_idx);

    void __cdecl Get_Gate_Icon_XY_(int16_t x, int16_t y, int16_t off_x, int16_t off_y, int16_t unused, int16_t* out_x, int16_t* out_y);

    void __cdecl Draw_Gate_Icons_(int16_t star_idx, int16_t x, int16_t y, int16_t width, int16_t height, int32_t unknown);

    void __cdecl Print_Gate_Info_(
            int16_t star_idx,
            uint16_t gate_type,
            int16_t x_base,
            int16_t y_base,
            int16_t x_offset,
            int16_t y_offset,
            int16_t x_extra,
            int16_t y_extra);

    void __cdecl Print_System_Display_Box_Title_(int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, const char* title_text);

    void __cdecl Print_Empty_System_Data_(int16_t star_idx);

    extern char _star_description_string[256];

    void __cdecl Draw_System_Display_Popup_(int16_t star_idx, int8_t draw_border, const char* title_text, int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, int16_t x_offset, int16_t y_offset);

    void __cdecl Wrapper_For_Normal_System_Popup_();

    void __cdecl Add_System_Display_Popup_Fields_(
        int16_t star_idx,
        int16_t base_x,
        int16_t base_y,
        int16_t popup_offset_x,
        int16_t popup_offset_y,
        int16_t orbit_offset_x,
        int16_t orbit_offset_y,
        int16_t* out_popup_hidden_field,
        int16_t* out_popup_title_button,
        int16_t* out_system_field);

    void __cdecl Wrapper_For_Informational_System_Fields_(int16_t star_idx, int16_t x, int16_t y, int16_t arg4, int16_t arg5, int16_t* arg6);
}