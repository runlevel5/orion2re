#pragma once

namespace RACESEL {
    extern int16_t _custom_flag;
    extern int16_t _displayed_race;
    extern int16_t _old_race;
    extern int16_t _race_button_status[14];

    extern uint8_t* _race_specials[14];

    extern s_animation_header* _race_button[14];

    extern char _last_race_name[15];

    extern s_animation_header* _last_race_seg;

    extern uint8_t _color_flag[8];
    extern uint8_t _race_flag[14];

    extern const char s_patch13_lbx[12];
    extern const char s_lastrace_rac[13];
    extern const char s_RACESEL_LBX[12];
    extern const char s_rb_0055117C[3];
    extern const char s_empty_00551100[1];
    extern const char s_space_00551120[2];

    void __cdecl Convert_Custom_Race_Flags_(uint8_t* custom_race_flags);

    void __cdecl Reload_Race_Selection_Screen_();
    int8_t __cdecl Naming_Popup_(char* output_name, int16_t race_idx);
    int16_t __cdecl Flag_Screen_(uint8_t* color_ptr);
    int16_t __cdecl Racial_Option_Screen_(s_player* player);
    short Race_Selection_Screen_(s_player *plr);

    void __cdecl Set_Race_Select_Help_();
    void __cdecl Unrequest_Race_(uint8_t race_idx);
    void __cdecl Draw_Flag_Screen_();
    void __cdecl Set_Flag_Screen_Help_();

    extern char* _temp_ruler_name;

    void __cdecl Draw_Naming_Popup_();
    void __cdecl Message_Build_(int16_t* message_count, int16_t split_at_count, char* output);
    void __cdecl Draw_Race_Selection_Screen_();
}