#pragma once

namespace GENDRAW {
    void __cdecl Message_Box_(const char* text);

    void __cdecl Help_(const char* message);

    int16_t __cdecl Confirmation_Box_(char* text);

    int16_t __cdecl Do_Confirmation_Box_(char* msg);

    void __cdecl Warning_Box_(const char* message);

    inline s_colors& _message_text_colors = *reinterpret_cast<s_colors*>(std::uintptr_t{0x00557640});
    inline uint8_t& _warning_box = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00566068});
    inline int16_t& _message_box_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056603C});
    inline uint8_t*& _box_bitmap = *reinterpret_cast<uint8_t**>(std::uintptr_t{0x00566040});
    inline int16_t& _animation_frame = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056606C});
    inline int16_t& _direction = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00566074});
    inline int16_t& _first_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00566064});
    inline s_animation_header* (&_image_seg)[4] = *reinterpret_cast<s_animation_header*(*)[4]>(std::uintptr_t{0x00566078});

    inline s_fieldsav_data& _message_field_save =
        *reinterpret_cast<s_fieldsav_data*>(std::uintptr_t{0x00566050});

    inline MOX2::action_func_t& _gendraw_af =
        *reinterpret_cast<MOX2::action_func_t*>(std::uintptr_t{0x00566044});

    inline int16_t& _save_refresh_mode =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00566038});

    inline int16_t& _old_draw_fields_flag =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00566048});

    inline int16_t& _old_help_list_active =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00566070});

    void __cdecl Draw_Confirm_Box_();

    void __cdecl Message_Box_Startup_();

    void __cdecl _Print_1_Or_More_Formatted_Lines_Centered_(int16_t x, int16_t y, int16_t width, char* text, s_animation_header* bitmap);

    void __cdecl Do_Print_1_Or_More_Formatted_Lines_Centered_(int16_t x, int16_t y, int16_t width, char* text);

    void __cdecl Draw_Animating_Message_Box_();

    void __cdecl Message_Box_Cleanup_();

    void __cdecl Message_Box_Exploding_(char* text);

    void __cdecl Message_Box_Exploding_Star_(const char* text, int16_t star_idx);
}