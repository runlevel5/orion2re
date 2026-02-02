#include "deps.h"

namespace BILL {
    void __cdecl Cache_Player_Scientist_Anim_() {
        Cache_Player_Scientist_Anim_Imp_(MOX::_PLAYER_NUM);
    }

    char* __cdecl Cache_Player_Scientist_Anim_Imp_(int16_t player_index) {
        char dir_entry[52];
        char source_path[128];

        s_player* player = &MOX::_player[player_index];

        _sprintf(BILL::_dest_file_name, s_SR_R_x_SC_LBX, (uint32_t)(uint8_t)player->race);

        if (!dos::DIR_(BILL::_dest_file_name, dir_entry)) {
            if (dos::DIR_(s_SR_R_wildcard_SC_LBX, dir_entry)) {
                dos::resolve_wildcard(dir_entry);
            }

            char* d = source_path;
            char* s = farload::_secondary_drive_path;
            while ((*d++ = *s++) != '\0');
            d--;
            s = BILL::_dest_file_name;
            while ((*d++ = *s++) != '\0');

            Copy_File_To_File_(source_path, BILL::_dest_file_name);
        }

        return BILL::_dest_file_name;
    }

    int16_t __cdecl Get_Their_Spy_Number_(int16_t player_idx, int16_t target_idx) {
        return static_cast<int16_t>(MOX::_player[player_idx].spies[target_idx] & 0x3F);
    }
}