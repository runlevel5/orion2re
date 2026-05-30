#pragma once

struct s_animation_header;
struct s_invasion_side_info;

namespace NEWPUP {
    extern MOX2::action_func_t _saved_autofunction;

    extern s_animation_header* _anims;

    void __cdecl Print_Data_(char* str, int16_t value, int16_t* y_pos, s_invasion_side_info* side_info);

    void __cdecl Print_Best_Summary_(s_invasion_side_info* side_info, int16_t* y_pos);

    void __cdecl Print_Race_Summary_(s_invasion_side_info* side_info, int16_t* summary_buffer);

    void __cdecl Print_Troop_Totals_(int16_t x_base, int16_t unused_param_2, s_invasion_side_info* invasion_side);

    void __cdecl Draw_Troop_Popup_();

    void __cdecl Reset_Fields_();

    extern const char s__00551100[];
    extern const char s_0_0055110c[3];
    extern const char s_1_00551110[3];

    void __cdecl Troop_Popup_();
}