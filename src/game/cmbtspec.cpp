#include "pch.h"

namespace CMBTSPEC {
    void __cdecl Draw_Cloak_(int16_t ship_idx, int16_t cloak_type) {
        int16_t unused_x, unused_y;
        int16_t offset_x, offset_y;
        int16_t rel_x, rel_y;
        int16_t duration;
        int16_t step;
        int32_t sound_handle;

        if (CMBTDRW1::Ship_On_Screen_(ship_idx) == 0) {
            return;
        }

        s_combat_data& ship = MOX::_combat_data[ship_idx];

        // This call's results are not used but maintained for semantic equivalence.
        CMBTDRW1::Get_Draw_Ship_Offsets_(ship.size_class, &unused_x, &unused_y);

        switch (cloak_type) {
            case 1:
            case 2:
                duration = 24;
                break;
            case 4:
                duration = 60;
                break;
            case 5:
                duration = 36;
                break;
            default:
                duration = ship_idx;
                break;
        }

        MOX::_dont_draw_ship = ship_idx;
        rel_x = (ship.x - MOX::_cur_combat_x) * 20;
        rel_y = (ship.y - MOX::_cur_combat_y) * 20;

        if (MOX::_speedx2_flag == 1 || MOX::_net_flag != 0) {
            step = 2;
        } else {
            step = 1;
        }

        sound_handle = ERICNET::Mox_Play_Sound_(10);

        if (duration > 0) {
            for (int16_t progress = 0; progress < duration; progress += step) {
                timer::Mark_Time_();
                video::Set_Page_Off_();
                CMBTDRW1::Draw_Main_Combat_Screen_();
                CMBTDRW1::Draw_Ship_To_Bitmap_(ship_idx, 0, cloak_type, progress);
                CMBTDRW1::Get_Draw_Ship_Offsets_(ship_idx, &offset_x, &offset_y);
                graphics::Set_Window_(0, 0, 639, 350);
                video::Clip_On_();
                animate::Draw_(offset_x + rel_x, offset_y + rel_y, MOX::_draw_ship_bitmap);
                video::Clip_Off_();
                graphics::Reset_Window_();
                RUSS::Mox_Sync_Update_();
                timer::Release_Time_(1);
            }
        }

        MOX::_dont_draw_ship = -1;
        sound::Fadeout_Sound_(sound_handle, 10);
    }
}