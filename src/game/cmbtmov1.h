#pragma once
#include <cstdint>

namespace CMBTMOV1 {
    int8_t __cdecl Ship_Is_In_Stasis_(int16_t ship_idx);

    void __cdecl Rotate_Ship_(int16_t ship_idx, int16_t target_facing, int16_t screen_x, int16_t screen_y);

    void __cdecl Wait_Until_Net_Opponent_Finished_(int16_t show_message);

    int16_t __cdecl Move_Cost_(int16_t ship_idx, int16_t target_x, int16_t target_y);

    int16_t __cdecl Turn_Cost_(int16_t ship_idx);

    int16_t __cdecl Move_Ship_(int16_t ship_idx, int16_t dest_x, int16_t dest_y);

    void __cdecl Turn_To_Face_(int16_t ship_idx, int16_t target_x, int16_t target_y);

    bool __cdecl Ship_In_Black_Hole_(int16_t ship_idx);

    int16_t __cdecl Get_Facing_(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    void __cdecl Draw_Teleporting_Ship_(int16_t ship_idx, int16_t target_x, int16_t target_y);

    int16_t __cdecl Rotate_Ship_Until_Impact_(int16_t ship_idx, int16_t* target_facing);

    int16_t __cdecl Ship_In_Legal_Square_(int16_t ship_id, int16_t x, int16_t y, int16_t target_x, int16_t target_y);

    int16_t __cdecl Test_Move_Until_Impact_(int16_t ship_idx, int16_t* dest_x, int16_t* dest_y, int16_t* dest_facing);
}