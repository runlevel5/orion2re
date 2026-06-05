#pragma once

namespace GENDRAW {
    void __cdecl Message_Box_(const char* text);

    void __cdecl Help_(const char* message);

    int16_t __cdecl Confirmation_Box_(const char* text);

    int16_t __cdecl Do_Confirmation_Box_(const char* msg);

    void __cdecl Warning_Box_(const char* message);

    extern s_colors _message_text_colors;
    extern uint8_t _warning_box;
    extern int16_t _message_box_y;
    extern s_animation_header* _box_bitmap;
    extern int16_t _animation_frame;
    extern int16_t _direction;
    extern int16_t _first_field;
    extern s_animation_header* _image_seg[4];

    extern s_fieldsav_data _message_field_save;

    extern MOX2::action_func_t _gendraw_af;

    extern int16_t _save_refresh_mode;

    extern int16_t _old_draw_fields_flag;

    extern int16_t _old_help_list_active;

    void __cdecl Draw_Confirm_Box_();

    void __cdecl Message_Box_Startup_();

    void __cdecl _Print_1_Or_More_Formatted_Lines_Centered_(int16_t x, int16_t y, int16_t width, const char* text, s_animation_header* bitmap);

    void __cdecl Do_Print_1_Or_More_Formatted_Lines_Centered_(int16_t x, int16_t y, int16_t width, const char* text);

    void __cdecl Draw_Animating_Message_Box_();

    void __cdecl Message_Box_Cleanup_();

    void __cdecl Message_Box_Exploding_(const char* text);

    void __cdecl Message_Box_Exploding_Star_(const char* text, int16_t star_idx);
}