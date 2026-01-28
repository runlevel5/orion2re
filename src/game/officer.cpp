#include "deps.h"

namespace OFFICER {
    int16_t __cdecl Get_Officer_Base_Level_(int16_t xp, uint8_t warlord) {
        if (xp < 60) {
            return 0;
        }
        if (xp < 150) {
            return 1;
        }
        if (xp < 300) {
            return 2;
        }
        if (xp < 500) {
            return 3;
        }
        if (xp < 1000) {
            return 4;
        }
        return (warlord != 0) ? 5 : 4;
    }

    int16_t __cdecl Owned_Officer_Level_(int leader_idx) {
        int8_t warlord = 0;

        if (leader_idx != 0x42) {
            int8_t player_idx = MOX::_leaders[leader_idx].player_index;
            warlord = MOX::_player[player_idx].traits[0];
        }

        int16_t xp = MOX::_leaders[leader_idx].xp;
        int16_t level = OFFICER::Get_Officer_Base_Level_(xp, warlord);

        if (level > 5) {
            level = 5;
        }

        return level;
    }
}