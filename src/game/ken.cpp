#include "deps.h"
#include "ken.h"

namespace KEN {
    s_animation_header* __cdecl Do_Get_Ship_Picture_Seg(int16_t player_idx, int16_t ship_type) {
        int8_t color_index = 8;

        if (player_idx < 8) {
            color_index = MOX::_player[player_idx].color;
        }

        int lbx_entry = (int)ship_type + (color_index * 50);

        return (s_animation_header*)Buffer_Reload_(s_ships_lbx_005597b4, lbx_entry, MOX::_global_cache_seg);
    }

    s_animation_header* __cdecl Get_Ship_Picture_Seg_(int16_t ship_id, int16_t frame_index) {
        return Do_Get_Ship_Picture_Seg(ship_id, frame_index);
    }
}