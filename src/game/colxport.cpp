#include "pch.h"

namespace COLXPORT {

/**
 * @brief Returns the number of non-outpost colonies belonging to a specific player.
 * 
 * If a "cheezy hack" state is active, it returns a cached value.
 * Otherwise, it iterates through the global colony list.
 */
short __cdecl N_Colonies_(short player_idx) {
    if (_cheezy_hack_col_count_state != 0) {
        return _cheezy_hack_col_count[player_idx];
    }

    short colony_count = 0;
    for (int16_t i = 0; i < MOX::_NUM_COLONIES; ++i) {
        s_colony* colony = &MOX::_colony[i];

        // Only count colonies owned by player_idx that are NOT outposts
        if (colony->owner == (char)player_idx && colony->outpost_flag == 0) {
            colony_count++;
        }
    }

    return colony_count;
}

/**
 * @brief Rebuilds the global list of colonies belonging to the current player.
 *
 * Scans through all existing colonies, filtering for those owned by the current player
 * and not marked as outposts. The resulting indices are stored in a global list,
 * which is then padded with -1 up to its capacity of 250.
 */
void __cdecl Build_Global_Colony_List_() {
    int16_t count = 0;

    for (int16_t i = 0; i < MOX::_NUM_COLONIES; ++i) {
        if (MOX::_colony[i].owner == static_cast<char>(MOX::_PLAYER_NUM) && MOX::_colony[i].outpost_flag == 0) {
            _g_colony_list_ptr[count] = i;
            count++;
        }
    }

    for (; count < 250; ++count) {
        _g_colony_list_ptr[count] = -1;
    }
}

void __cdecl Load_Xport_Anims_() {
    _anims[3] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLPUPS_LBX, 0x11, MOX::_screen_seg);
    _anims[1] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLPUPS_LBX, 0x13, MOX::_screen_seg);
    _anims[2] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLPUPS_LBX, 0x14, MOX::_screen_seg);
}

} // namespace COLXPORT