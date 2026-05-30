#pragma once

namespace SCIENCE {
    extern const char s_SR_R_x_SC_LBX_005512b0[14]; // "SR_R%x_SC.LBX"

    extern const char _spy_anim_fmt[14];
    extern const char _trooper_anim_fmt[14];
    extern const char _spy_anim_find_pat[13];
    extern const char _trooper_anim_find_pat[13];

    extern const char s_billtext_lbx_0055135c[13]; // "billtext.lbx"

    void __cdecl Show_Off_Stolen_Tech_(uint16_t tech_idx, int16_t other_idx);

    extern const char s__00551100[1];
    extern const char s__00551120[2];
    extern const char s__0055136c[2]; /* "\r" */

    extern s_colors _science_high_color;

    extern s_colors _science_normal_color;

    extern int16_t _bm_scale_x[5];

    extern int16_t _bm_scale_y[5];

    extern int16_t _bm_scaled_x1[5];

    extern int16_t _bm_scaled_y1[5];

    extern void* _science_mouse_bitmap;

    extern const char s_SCIENCE_LBX_00551290[12];
    void __cdecl Update_Science_Anims_(int unused, uint8_t draw_multi_anim, uint8_t draw_anim_2, uint8_t draw_anim_1);

    void __cdecl Draw_Science_Room_();

    void __cdecl Science_Room_(char* intro_animation_name, uint16_t discovery_count, int16_t* discovery_data, char* footer_text);

    void __cdecl Show_Off_Researched_Tech_(int16_t player_idx);

    void __cdecl Show_Off_Captured_Tech_(int16_t* captured_tech_ids, uint16_t tech_count, int16_t message_arg);
}