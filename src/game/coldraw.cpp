#include "pch.h"

namespace COLDRAW {

void __cdecl Draw_Colony_Prod_(int16_t colony_handle, int32_t selection, int32_t p3, int16_t y_offset, int32_t p5, int32_t p6) {
    Draw_Colony_Prod_Both_(colony_handle, (int16_t)selection, (int16_t)p3, y_offset, (int16_t)p5, (int16_t)p6, 0);
}

void __cdecl Draw_Info_Morale_(int16_t param_1, int16_t param_2, int16_t param_3, int32_t param_4) {
    COLDRAW::Draw_Info_Morale_Both_(param_1, param_2, param_3, param_4, 0);
}

void __cdecl Draw_Colony_PC_Bar_(int16_t x, int16_t y, int16_t total, int16_t current) {
    int32_t max_div = (total > 1000) ? (int32_t)total : 1000;
    int32_t capped_cur = (current < total) ? (int32_t)current : (int32_t)total;

    // Calculate frame for the first animation (anim 35)
    int16_t frame1 = (int16_t)((capped_cur * 50) / max_div);

    // Calculate frame for the second animation (anim 34)
    // Note: Assembly uses ESI for this calculation and then refers to SI.
    int32_t frame2_full = ((capped_cur - (int32_t)total) * 50) / max_div + 50;
    int16_t frame2 = (int16_t)frame2_full;

    // Calculate Y offset for the second animation drawing
    // Assembly uses EDI for the first part, then modifies only DI (lower 16 bits).
    int32_t edi_val = ((capped_cur - max_div) * 10) / max_div;
    int16_t di_part = (int16_t)edi_val;
    di_part += (int16_t)(((int32_t)total - capped_cur) * 64 / max_div);
    di_part -= (int16_t)(((int32_t)total * 64) / max_div);

    // Reconstruct EDI value to match register behavior
    int32_t final_edi = (edi_val & 0xFFFF0000) | (uint16_t)di_part;

    graphics::Set_Window_(0, 0, 639 /* 0x27F */, 479 /* 0x1DF */);
    video::Clip_On_();

    // Draw the first part of the bar
    s_animation_header* anim_35 = COLONY::C_Anims_(35 /* 0x23 */);
    animate::Set_Animation_Frame_(anim_35, (int32_t)frame1);
    anim_35 = COLONY::C_Anims_(35 /* 0x23 */);
    animate::Draw_(x, y, anim_35);

    // Draw the second part of the bar at a calculated vertical offset
    s_animation_header* anim_34 = COLONY::C_Anims_(34 /* 0x22 */);
    animate::Set_Animation_Frame_(anim_34, (int32_t)frame2);
    anim_34 = COLONY::C_Anims_(34 /* 0x22 */);

    // Final Y is y + offset + 10
    int16_t y_final = (int16_t)(final_edi + (int32_t)y + 10);
    animate::Draw_(x, y_final, anim_34);

    video::Clip_Off_();
}

}