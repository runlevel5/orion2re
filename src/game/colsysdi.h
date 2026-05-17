#pragma once

namespace COLSYSDI {
    void __cdecl Draw_Col_Sys_Disp_(int16_t x, int16_t y);

    s_animation_header* __cdecl Planet_Anims_(int16_t planet_type, int16_t planet_size);

    void __cdecl Planet_Summary_String_(char* param_1, int16_t param_2, int16_t param_3);

    void __cdecl Draw_Col_Sys_Disp_Last_Scanned_(int16_t param_1, int16_t param_2);

    void __cdecl Add_Col_Sys_Disp_Fields_(int16_t x, int16_t y, int16_t end_x);
}