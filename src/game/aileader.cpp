#include "pch.h"

namespace aileader {
    void __cdecl Do_AI_Leaders_() {
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            s_player* player = &MOX::_player[i];

            if (player->objectives != 100 /* HUMAN_PLAYER */) {
                int8_t leader_idx = player->officer_for_hire;

                if (leader_idx != -1) {
                    bool hire_criteria_met = false;
                    s_leader_data* leader = &MOX::_leaders[leader_idx];
                    uint32_t gen_skills = leader->general_skills;

                    if ((gen_skills & 0x300) != 0) {
                        hire_criteria_met = true;
                    } else if ((gen_skills & 0x2000) != 0 && player->do_not_research == 0) {
                        hire_criteria_met = true;
                    } else if ((gen_skills & 0x1000) != 0 && player->do_not_research == 0) {
                        hire_criteria_met = true;
                    } else if ((gen_skills & 0x3CCC3) != 0) {
                        hire_criteria_met = true;
                    } else if ((gen_skills & 0x80000) != 0 && player->traits[19] == 0) {
                        hire_criteria_met = true;
                    } else if ((gen_skills & 0x40000) != 0 && player->traits[19] == 0) {
                        hire_criteria_met = true;
                    } else if (leader->type == 0 && (leader->special_skills & 0xC0C0) != 0) {
                        hire_criteria_met = true;
                    }

                    int16_t accept_final = 0;
                    if (hire_criteria_met) {
                        int16_t cost = OFFICER::Calculate_Officer_Cost_(i, (int16_t)leader_idx);
                        if (cost + 50 < player->bc) {
                            accept_final = 1;
                            player->bc -= (int32_t)cost;
                        }
                    }

                    OFFICER::Set_Officer_To_Player_((int16_t)leader_idx, i, (uint8_t)accept_final);
                }

                player->officer_for_hire = -1;
            }
        }

        for (int16_t star_idx = 0; star_idx < (int16_t)MOX::_NUM_STARS; ++star_idx) {
            s_star_data* star = &MOX::_star[star_idx];
            int8_t special = star->system_special;

            if (special >= 80 && special < 90) {
                int16_t player_idx = (int16_t)(special - 80);
                if (MOX::_player[player_idx].objectives != 100 /* HUMAN_PLAYER */) {
                    int16_t joined_result;
                    OFFICER::Marooned_Hero_At_Star_Can_Join_(star_idx, &joined_result, player_idx);
                }
            }
        }

        for (int32_t i = 0; i < (int32_t)MOX::_NUM_PLAYERS; ++i) {
            if (MOX::_player[i].objectives != 100 /* HUMAN_PLAYER */) {
                Assign_Unassigned_Leaders_(i);
            }
        }
    }
}