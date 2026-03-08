#include "pch.h"
#include "bomb.h"
#include "mox.h"
#include "events.h"
#include "msg.h"
#include "invasion.h"
#include "colbldg.h"
#include "colcalc.h"
#include "officer.h"
#include <cstring>

namespace BOMB {

    int16_t __cdecl Handle_Leader_At_Lost_Colony_(s_colony* colony) {
        int owner_idx = (int)colony->owner;
        int star_idx = (int)MOX::_planet[colony->planet].star_index;
        int16_t killed_leader_idx = -1;
        int16_t leader_idx = (int16_t)MOX::_star[star_idx].officer_index[owner_idx];

        if (leader_idx != -1) {
            bool other_colony_found = false;
            int16_t current_colony_idx = (int16_t)(colony - MOX::_colony);

            for (int i = 0; i < 5; ++i) {
                int16_t p_idx = MOX::_star[star_idx].planet_index[i];
                if (p_idx != -1) {
                    int16_t c_idx = MOX::_planet[p_idx].colony_index;
                    if (c_idx != -1 && MOX::_colony[c_idx].owner == owner_idx && c_idx != current_colony_idx) {
                        other_colony_found = true;
                        break;
                    }
                }
            }

            if (!other_colony_found) {
                if (MOX::_leaders[leader_idx].eta > 0) {
                    OFFICER::Move_Officer_To_Pool_(leader_idx);
                } else {
                    OFFICER::Kill_Officer_(leader_idx);
                    killed_leader_idx = leader_idx;
                }
                MOX::_star[star_idx].officer_index[owner_idx] = -1;
            }
        }

        return killed_leader_idx;
    }

    void __cdecl Destroy_Colony_(s_colony* colony, int32_t capturing_player) {
        s_star_data* star;
        int16_t colony_idx;
        int16_t leader_idx;
        s_MSG msg;

        star = &MOX::_star[MOX::_planet[colony->planet].star_index];
        colony_idx = (int16_t)(colony - MOX::_colony);

        EVENTS::Kill_All_Events_At_Colony_(colony_idx);

        leader_idx = BOMB::Handle_Leader_At_Lost_Colony_(colony);

        if (capturing_player != -1) {
            memset(&msg, 0, sizeof(s_MSG));
            msg.msg_type = 7 /* MSG_TYPE_LOST_COLONY */;
            msg.u.lost_colony.planet_idx = colony->planet;
            // TYPE MISMATCH: using 'reason' from orion2.h for 'player_idx'
            msg.u.lost_colony.reason = (int16_t)capturing_player;
            msg.u.lost_colony.leader_idx = leader_idx;
            // TYPE MISMATCH: using 'colony_or_outpost' from orion2.h for 'outpost_flag'
            msg.u.lost_colony.colony_or_outpost = colony->outpost_flag;
            MSG_::Add_Msg_(msg, colony->owner);
        }

        if (MOX::_player[(uint8_t)colony->owner].home_planet_id == colony->planet) {
            invasion::Assign_New_Home_Planet_(colony->owner, colony_idx);
        }

        COLBLDG::Clear_Colony_Queue_(colony_idx);

        MOX::_planet[colony->planet].colony_index = -1;

        memset(colony, 0, sizeof(s_colony));
        colony->planet = -1;
        colony->owner = -1;

        COLCALC::Compute_Star_Colony_Stuff_(star);
    }

} // namespace BOMB