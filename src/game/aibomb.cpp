#include "pch.h"

namespace aibomb {

bool __cdecl AI_Invasion_Will_Succeed_(int8_t attacker_id, int16_t* ship_list, int n_ships, s_colony* colony) {
    s_invasion_side_info invader;
    s_invasion_side_info defender;

    invasion::Get_Invasion_Info_(ship_list, n_ships, colony, &invader, &defender);

    if (COMBFIND::_current_star_has_player[attacker_id].transport_count > 0) {
        invader.current_troop_type_idx = 1;
        invader.unit_counts[1] += COMBFIND::_current_star_has_player[attacker_id].transport_count * 4 - 2;
    }

    if (invader.unit_counts[1] > 0) {
        s_invasion_side_info backup_invader = invader;
        s_invasion_side_info backup_defender = defender;

        int remaining_tries = 10;
        int invader_wins = 0;

        do {
            if (invader_wins >= 7 || invader_wins + remaining_tries < 7) {
                break;
            }

            remaining_tries--;

            invasion::Resolve_Ground_Combat_(&invader, &defender);

            if (static_cast<uint8_t>(invader.current_troop_type_idx) < 4) {
                invader_wins++;
            }

            invader = backup_invader;
            defender = backup_defender;

        } while (remaining_tries > 0);

        return invader_wins >= 7;
    }

    return false;
}

}