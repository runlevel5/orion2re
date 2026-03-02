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

    /**
     * @address 0042a430
     */
    /**
     * @address 0042a6b0
     */
    void __cdecl Assign_Star_Officer_(int player_index, int leader_index) {
        int star_scores[72];
        memset(star_scores, 0, sizeof(star_scores));

        uint32_t skills = MOX::_leaders[leader_index].special_skills;

        for (int i = 0; i < MOX::_NUM_COLONIES; ++i) {
            s_colony* colony = &MOX::_colony[i];

            if (colony->owner == player_index && colony->outpost_flag == 0) {
                int star_index = MOX::_planet[colony->planet].star_index;

                if (MOX::_star[star_index].officer_index[player_index] == -1) {

                    // Environmentalist (Bits 0-1)
                    if ((skills & 0x0003) && (*AIDATA::_ai_colony_info)[i].trait_environment_immune != 0) {
                        star_scores[star_index] += colony->pollution;
                    }

                    // Farmer (Bits 2-3)
                    if ((skills & 0x000C) && colony->food2_per_farmer != 0) {
                        star_scores[star_index] += colony->production[0];
                    }

                    // Administrator (Bits 4-5)
                    if (skills & 0x0030) {
                        star_scores[star_index] += (uint8_t)colony->n_pops;
                    }

                    // Industrialist (Bits 8-9)
                    if (skills & 0x0300) {
                        star_scores[star_index] += colony->production[1];
                    }

                    // Researcher (Bits 12-13)
                    if (skills & 0x3000) {
                        star_scores[star_index] += colony->production[2];
                    }

                    // Diplomat/Telepath? (Bits 14-15)
                    if ((skills & 0xC000) && (MOX::_player[player_index].traits[0] / 2 != 3)) {
                        star_scores[star_index] += (uint8_t)colony->n_pops;
                    }

                    // Expansionist (Bits 10-11)
                    if (skills & 0x0C00) {
                        int bonus = (uint8_t)(*AIDATA::_ai_colony_info)[i].pop_limit - (uint8_t)colony->n_pops;
                        if (bonus > 0) {
                            star_scores[star_index] += bonus;
                        }
                    }

                    // Fleet Officer (Bits 16-17)
                    if (skills & 0x00030000) {
                        star_scores[star_index] += (uint8_t)(*AIDATA::_ai_star_info)[star_index].per_player[player_index].neutral_count +
                                                   (uint8_t)(*AIDATA::_ai_star_info)[star_index].per_player[player_index].alliance_count;
                    }
                }
            }
        }

        int best_star_idx = MOX::_NUM_STARS - 1;
        for (int i = MOX::_NUM_STARS - 2; i >= 0; --i) {
            if (star_scores[i] > star_scores[best_star_idx]) {
                best_star_idx = i;
            }
        }

        if (star_scores[best_star_idx] != 0) {
            Ai_Assign_Leader_To_Star_(&MOX::_player[player_index], &MOX::_leaders[leader_index], &MOX::_star[best_star_idx]);
        }
    }

    /**
     * @address 0042a4a0
     */
    void __cdecl Assign_Fleet_Officer_(int player_index, int leader_index) {
        s_ship_data* best_ship = nullptr;
        s_ship_data* ship_list = MOX::_ship;
        int num_ships = (int)MOX::_NUM_SHIPS;

        for (int i = 0; i < num_ships; ++i) {
            s_ship_data* current_ship = &ship_list[i];

            if (current_ship->owner == (char)player_index &&
                current_ship->status <= 2 &&
                current_ship->d.ship_type == 0 &&
                current_ship->officer_index == -1)
            {
                if (best_ship == nullptr || (
                    best_ship->d.size <= current_ship->d.size &&
                    best_ship->d.shield_type <= current_ship->d.shield_type &&
                    best_ship->d.armor_type <= current_ship->d.armor_type &&
                    best_ship->d.computer_type <= current_ship->d.computer_type &&
                    best_ship->crew_experience < current_ship->crew_experience))
                {
                    best_ship = current_ship;
                }
            }
        }

        if (best_ship != nullptr) {
            OFFICER::Ai_Assign_Leader_To_Ship_(
                &MOX::_player[player_index],
                &MOX::_leaders[leader_index],
                best_ship
            );
        }
    }

    /**
     * @address 0042a570
     */
    void __cdecl Reassign_Fleet_Officer_(int player_index, int leader_index) {
        short orig_loc = MOX::_leaders[leader_index].location;
        s_ship_data* original_ship = &MOX::_ship[orig_loc];
        s_ship_data* best_ship = original_ship;

        s_ship_data* ships_end = &MOX::_ship[MOX::_NUM_SHIPS];

        for (s_ship_data* ship = MOX::_ship; ship < ships_end; ++ship) {
            if (ship->owner == player_index && ship->status <= 2 && ship->d.ship_type == 0 && ship->officer_index == -1) {
                short ship_idx = static_cast<short>(ship - MOX::_ship);
                if (OFFICER::Ships_In_Same_Place_(ship_idx, orig_loc)) {
                    if (ship->d.size >= best_ship->d.size &&
                        ship->d.shield_type >= best_ship->d.shield_type &&
                        ship->d.armor_type >= best_ship->d.armor_type &&
                        ship->d.computer_type >= best_ship->d.computer_type &&
                        ship->crew_experience > best_ship->crew_experience)
                    {
                        best_ship = ship;
                    }
                }
            }
        }

        if (best_ship != original_ship) {
            OFFICER::Ai_Assign_Leader_To_Ship_(&MOX::_player[player_index], &MOX::_leaders[leader_index], best_ship);
        }
    }

    void __cdecl Assign_Unassigned_Leaders_(int32_t player_idx) {
        // There are 67 leaders in total (0xF71 / 0x3B)
        for (int i = 0; i < 67; ++i) {
            s_leader_data* leader = &MOX::_leaders[i]; // leader size is 0x3B

            if (leader->player_index == (char)player_idx) {
                if (leader->status == 0 /* Available/Unassigned */) {
                    if (leader->type == 0 /* Fleet Officer */) {
                        Assign_Fleet_Officer_(player_idx, i);
                    } else /* Star Officer */ {
                        Assign_Star_Officer_(player_idx, i);
                    }
                } else if (leader->status == 1 /* Assigned */) {
                    // Only fleet officers seem to be reassigned in this manner
                    if (leader->type == 0 /* Fleet Officer */) {
                        Reassign_Fleet_Officer_(player_idx, i);
                    }
                }
            }
        }
    }
}