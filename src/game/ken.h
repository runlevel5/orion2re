#pragma once
#include <cstdint>

struct s_animation_header;

namespace KEN {
    void __cdecl Ken_Init_();

    s_animation_header* __cdecl Do_Get_Ship_Picture_Seg(int16_t player_idx, int16_t ship_type);
    s_animation_header* __cdecl Get_Ship_Picture_Seg_(int16_t ship_id, int16_t frame_index);

    void __cdecl Ken_Get_Text_Message_(int32_t msg_index, char* buffer, int16_t param_3);

    extern const char s_KENTEXT_LBX_005597c0[12]; /* "KENTEXT.LBX" */

    extern const char s_KENTEXT1_LBX_005597cc[13]; /* "KENTEXT1.LBX" */

    extern uint8_t _personalities[140];

    void __cdecl Init_NPC_Personalities_Objectives_Themes_();

    void __cdecl Ken_Init2_();

    void __cdecl Load_Player_Ship_Palette_(int16_t player_idx);

    uint16_t __cdecl Get_Weighted_Choice_(uint16_t* weights, int16_t count);

    void __cdecl Get_Derelict_Applications_(int16_t player_idx, uint16_t* app_array, int16_t* app_count);

    s_animation_header* __cdecl Get_Ship_Id_Picture_Seg_(int16_t ship_idx);
}