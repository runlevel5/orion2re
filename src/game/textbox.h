#pragma once
#include <cstdint>

#pragma pack(push, 1)
struct s_help_record {
    char title[84];
    char anim_lbx[10];
    uint32_t anim_info;
    uint8_t unknown_98;
    uint32_t next_help_idx;
    char body[1300];
};
#pragma pack(pop)

namespace TEXTBOX {
    extern int16_t _bottom_y;
    extern uint8_t* _textbox_seg;
    extern s_animation_header* tmp_block;
    extern uint16_t _field_save_ix;
    extern const char s_textbox_lbx[12];
    extern const char (&s_textbox_lbx_00557444)[12];
    extern const char s_SPA_HELP_LBX[16];
    extern const char s_ITA_HELP_LBX[16];
    extern const char s_FRE_HELP_LBX[16];
    extern const char s_GER_HELP_LBX[16];
    extern const char s_HELP_LBX[9];
    extern s_fieldsav_data _textbox_field_save[2];
    extern uint8_t _reload_saved_block;
    extern int16_t _box_x;
    extern int16_t _box_y;
    extern uint16_t _old_draw_fields_flag;
    extern MOX2::action_func_t _textbox_af;
    extern uint16_t _old_help_list_active;
    extern const char s__00551120[2];
    extern int32_t _base_colors[3];

    void __cdecl Get_Help_Lbx_Name_(char* help_lbx_name);

    void __cdecl Get_Mox_Text_Box_Base_Colors_(int16_t color_set, uint8_t *out_color_a, uint8_t *out_color_b);

    void __cdecl Text_Box_Startup_(int32_t size);

    void __cdecl Box_Centered_On_XY_(int16_t* x_ptr, int16_t* y_ptr, int16_t height);

    void __cdecl Text_Box_Get_Input_(int16_t ticks);

    void __cdecl Text_Box_Cleanup_();

    void __cdecl Do_Text_Box_(const char* title, const char* text, int16_t input_flag);

    void __cdecl Text_Box_(const char* title, const char* text);

    void __cdecl Draw_Help_Entry_(int16_t help_id);

    void __cdecl Timed_Text_Box_(const char* title, const char* message, int16_t duration);

}