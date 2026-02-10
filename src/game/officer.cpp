#include "deps.h"

namespace OFFICER {
    int16_t __cdecl Get_Officer_Level_(int16_t leader_idx) {
        int16_t current_xp = MOX::_leaders[leader_idx].xp;

        int16_t level = Get_Officer_Base_Level_(current_xp, 0);

        if (level > 5) {
            level = 5;
        }

        return level;
    }

    int16_t __cdecl Get_Officer_Base_Level_(int16_t xp, int8_t warlord) {
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

    int32_t __cdecl Officer_Skill_Bonus_(uint32_t param_1, int16_t leader_idx, int16_t skill_idx) {
        int16_t level;

        if (MOX::_leaders[leader_idx].player_index == -1) {
            level = Get_Officer_Level_(leader_idx);
        } else {
            level = Owned_Officer_Level_(static_cast<int32_t>(leader_idx));
        }

        int32_t current_level_val = static_cast<int32_t>(level) + 1;
        int32_t skill_div = static_cast<int32_t>(MOX::_skill_data[skill_idx].unknown2);
        int32_t bonus_base = static_cast<int32_t>(MOX::_skill_data[skill_idx].skill_bonus);

        int32_t intermediate = (current_level_val + skill_div - 1) / skill_div;

        return (intermediate * bonus_base) / 10;
    }

    int16_t __cdecl Owned_Officer_Level_(int leader_idx) {
        int8_t warlord_trait = 0;

        // leader_idx 0x42 (66) likely refers to a special entity or unassigned leader
        if (leader_idx != 0x42) {
            int8_t player_idx = MOX::_leaders[leader_idx].player_index;
            warlord_trait = MOX::_player[player_idx].traits[TRAIT_WARLORD];
        }

        int16_t level = OFFICER::Get_Officer_Base_Level_(MOX::_leaders[leader_idx].xp, warlord_trait);

        // Cap the maximum level at 5
        if (level > 5) {
            level = 5;
        }

        return level;
    }

    int16_t __cdecl Officer_Skill_Value_(int16_t leader_index) {
        int16_t total_value = 0;
        s_leader_data& leader = MOX::_leaders[leader_index];

        for (int16_t skill_idx = 0; skill_idx < 54; ++skill_idx) {
            s_skill_data& skill = MOX::_skill_data[skill_idx];

            if (skill.unknown1 == 0) {
                if (OFFICER::Officer_Has_General_Skill_(MOX::_PLAYER_NUM, leader_index, skill_idx)) {
                    int16_t val;
                    if (skill_idx == 2 || skill_idx == 3) {
                        // type 0 (Ship Officer) gets 3, type 1 (Colony Leader) gets 1
                        val = (leader.type == 0) ? 3 : 1;
                        if (skill_idx == 3) {
                            val *= 2;
                        }
                    } else {
                        val = (int8_t)skill.unknown3;
                    }
                    total_value += val;
                }
            }

            if (leader.type == 0) { // Ship Officer
                if (skill.unknown1 == 1) {
                    if (OFFICER::Officer_Has_Special_Skill_(MOX::_PLAYER_NUM, leader_index, skill_idx)) {
                        total_value += (int8_t)skill.unknown3;
                    }
                }
            } else if (leader.type == 1) { // Colony Leader
                if (skill.unknown1 == 2) {
                    if (OFFICER::Officer_Has_Special_Skill_(MOX::_PLAYER_NUM, leader_index, skill_idx)) {
                        total_value += (int8_t)skill.unknown3;
                    }
                }
            }
        }

        for (int i = 0; i < 3; ++i) {
            uint8_t app_idx = leader.tech_application[i];
            if (app_idx != 0) {
                int16_t field_id = TECHDATA::_technology_applications[app_idx].tech_field_id;
                if (field_id < 0) {
                    // Logic from disassembly: resets total_value to 1 if field_id is invalid/negative
                    total_value = 1;
                } else {
                    total_value += OFFICER::Get_Research_Cost_Skill_Value_(TECHDATA::_technology_fields[field_id].cost);
                }
            }
        }

        return total_value;
    }

    void __cdecl Get_Random_Skills_(uint32_t* primary_mask, uint32_t* secondary_mask, int16_t skill_class, int16_t count, int8_t randomize_class) {
        *primary_mask = 0;
        *secondary_mask = 0;

        if (count <= 0) {
            return;
        }

        int16_t remaining_count = count;
        do {
            int16_t selected_idx = 0;
            int16_t options_found = 0;
            int16_t current_class = skill_class;

            if (randomize_class != 0) {
                if (random::Random_(7) < 3) {
                    current_class = 0;
                }
            }

            for (int j = 0; j < 54; j += 2) {
                if (current_class == 0) {
                    // Search for secondary skills
                    if (MOX::_skill_data[j].unknown1 == 0 &&
                        ((*secondary_mask & MOX::_skill_data[j].skill_mask) == 0) &&
                        ((*secondary_mask & MOX::_skill_data[j + 1].skill_mask) == 0)) 
                    {
                        options_found++;
                        if (random::Random_(options_found) == 1) {
                            if (random::Random_(2) == 1) {
                                selected_idx = (int16_t)j;
                            } else {
                                selected_idx = (int16_t)(j + 1);
                            }
                        }
                    }
                } else {
                    // Search for primary skills of specific class
                    if ((int16_t)MOX::_skill_data[j].unknown1 == skill_class &&
                        ((*primary_mask & MOX::_skill_data[j].skill_mask) == 0) &&
                        ((*primary_mask & MOX::_skill_data[j + 1].skill_mask) == 0)) 
                    {
                        options_found++;
                        if (random::Random_(options_found) == 1) {
                            if (random::Random_(2) == 1) {
                                selected_idx = (int16_t)j;
                            } else {
                                selected_idx = (int16_t)(j + 1);
                            }
                        }
                    }
                }
            }

            if (current_class == 0) {
                *secondary_mask += MOX::_skill_data[selected_idx].skill_mask;
            } else {
                *primary_mask += MOX::_skill_data[selected_idx].skill_mask;
            }

            remaining_count--;
        } while (remaining_count != 0);
    }

    bool __cdecl One_Leader_With_Galactic_Lore_(int16_t player_idx) {
        return (bool)MOX::_player[player_idx].has_galactic_lore;
    }

    bool __cdecl Officer_Has_General_Skill_(int16_t player_num, int16_t leader_index, int16_t skill_id) {
        return (MOX::_skill_data[skill_id].skill_mask & MOX::_leaders[leader_index].general_skills) != 0;
    }

    bool __cdecl Officer_Has_Special_Skill_(int16_t player_num, int16_t leader_idx, int16_t skill_idx) {
        uint32_t mask = MOX::_skill_data[skill_idx].skill_mask;
        uint32_t leader_skills = MOX::_leaders[leader_idx].special_skills;

        return (mask & leader_skills) != 0;
    }

    int16_t __cdecl Get_Research_Cost_Skill_Value_(uint32_t cost) {
        if (cost < 1000) {
            return 1;
        }

        if (cost >= 5000) {
            return 4;
        }

        return 3;
    }

    /**
     * Increases an officer's experience to the minimum required for the next level.
     * The experience thresholds are 60, 150, 300, 500, and 1000 for levels 1 through 5.
     * If the officer is already at the maximum level (5), no change is made.
     */
    void __cdecl Increase_Officer_Level_By_One_(int16_t leader_index) {
        // Experience thresholds for levels 1, 2, 3, 4, 5
        const int16_t level_thresholds[] = { 60, 150, 300, 500, 1000 };

        s_leader_data& leader = MOX::_leaders[leader_index];
        int8_t warlord = 0;

        // Check if the officer's owner has the Warlord trait
        if (leader.player_index != -1) {
            if (MOX::_player[(uint8_t)leader.player_index].traits[TRAIT_WARLORD] != 0) {
                warlord = 1;
            }
        }

        // Calculate the current level based on XP and Warlord status
        int16_t current_level = Get_Officer_Base_Level_(leader.xp, warlord);

        // Increase level if not already at max (level 5 / Elite)
        if (current_level >= 0 && current_level < 5) {
            leader.xp = level_thresholds[current_level];
        }
    }

    void __cdecl Deassign_Officer_(int16_t leader_idx) {
        s_leader_data& leader = MOX::_leaders[leader_idx];
        int16_t player_idx = (int16_t)leader.player_index;

        OFFICER::Increase_Officer_Level_By_One_(leader_idx);

        if (leader.type == 1) { // Colony Administrator
            int16_t star_idx = leader.location;
            if (star_idx != -1) {
                if (leader.status == 5) { // Active/Assigned status
                    MOX::_star[star_idx].system_special = 0;
                }
                if (player_idx != -1 && MOX::_star[star_idx].officer_index[player_idx] == (int8_t)leader_idx) {
                    MOX::_star[star_idx].officer_index[player_idx] = -1;
                }
            }
        } else if (leader.type == 0 && player_idx != -1) { // Ship Officer
            int16_t ship_idx = leader.location;
            if (ship_idx != -1) {
                if (HACCESS::Ship_Owner_(ship_idx) == player_idx && MOX::_ship[ship_idx].officer_index == leader_idx) {
                    MOX::_ship[ship_idx].officer_index = -1;
                }
            }
        }

        // Reset leader state to unassigned
        leader.eta = 0;
        leader.status = -1;
        leader.location = -1;
        leader.player_index = -1;
    }

    void __cdecl Allocate_Adv_Civ_Game_Officers_() {
        int16_t leader_indices[MAX_LEADERS];
        uint8_t is_leader_assigned[MAX_LEADERS];

        for (int i = 0; i < MAX_LEADERS; ++i) {
            is_leader_assigned[i] = 0;
        }

        // Phase 1: Assign one leader to each Repulsive player from a shuffled pool of 10
        for (int16_t i = 0; i < MAX_LEADERS; ++i) {
            leader_indices[i] = i;
        }

        for (int i = 0; i < 10; ++i) {
            int16_t idx1 = (int16_t)random::Random_(10) - 1;
            int16_t idx2 = (int16_t)random::Random_(10) - 1;
            if (idx1 != idx2) {
                int16_t tmp = leader_indices[idx1];
                leader_indices[idx1] = leader_indices[idx2];
                leader_indices[idx2] = tmp;
            }
        }

        int16_t repulsive_pool_idx = 0;
        for (int16_t player_idx = 0; player_idx < (int16_t)MOX::_NUM_PLAYERS; ++player_idx) {
            if (MOX::_player[player_idx].traits[TRAIT_REPULSIVE] != 0) {
                int16_t leader_idx = leader_indices[repulsive_pool_idx++];
                if (leader_idx >= 0) {
                    MOX::_leaders[leader_idx].status = 4;
                    MOX::_leaders[leader_idx].player_index = (char)player_idx;
                    is_leader_assigned[leader_idx] = 1;
                }
            }
        }

        // Phase 2: Assign two leaders to each non-Repulsive player from a shuffled pool of 25
        for (int16_t i = 0; i < MAX_LEADERS; ++i) {
            leader_indices[i] = i;
        }

        for (int i = 0; i < 25; ++i) {
            int16_t idx1 = (int16_t)random::Random_(25) - 1;
            int16_t idx2 = (int16_t)random::Random_(25) - 1;
            if (idx1 != idx2) {
                int16_t tmp = leader_indices[idx1];
                leader_indices[idx1] = leader_indices[idx2];
                leader_indices[idx2] = tmp;
            }
        }

        for (int16_t player_idx = 0; player_idx < (int16_t)MOX::_NUM_PLAYERS; ++player_idx) {
            if (MOX::_player[player_idx].traits[TRAIT_REPULSIVE] == 0) {
                for (int count = 0; count < 2; ++count) {
                    int16_t pool_idx = 0;
                    int16_t leader_idx;
                    while (true) {
                        leader_idx = leader_indices[pool_idx];
                        if (is_leader_assigned[leader_idx] == 0) break;
                        pool_idx++;
                    }

                    if (leader_idx >= 0) {
                        MOX::_leaders[leader_idx].status = 4;
                        MOX::_leaders[leader_idx].player_index = (char)player_idx;
                        is_leader_assigned[leader_idx] = 1;
                    }
                }
            }
        }
    }

    int32_t __cdecl Officer_Cost_(int16_t player_idx, int16_t leader_idx) {
        uint16_t level_data = Get_Officer_Level_(leader_idx);
        int16_t level_multiplier = (int16_t)((level_data & 0xFF) + 1);

        int16_t base_calc = MOX::_leaders[leader_idx].skill_value * level_multiplier;
        int16_t bonus = Bonus_For_Famous_Heroes_(player_idx);

        int16_t total_cost = base_calc * 10;

        if (Officer_Has_General_Skill_(player_idx, leader_idx, 28) ||
            Officer_Has_General_Skill_(player_idx, leader_idx, 29)) {
            total_cost = base_calc * 20;
        }

        int32_t final_cost = (int32_t)total_cost - (int32_t)bonus;

        if (final_cost < 1) {
            return 0;
        }

        return final_cost;
    }

    int16_t __cdecl Officer_Maintenance_(int16_t player_num, int16_t leader_idx) {
        int32_t cost = OFFICER::Officer_Cost_(player_num, leader_idx);

        int16_t maintenance_cost = (int16_t)((cost + 99) / 100);

        bool has_cost_reduction_skill = OFFICER::Officer_Has_General_Skill_(player_num, leader_idx, 28) ||
                                         OFFICER::Officer_Has_General_Skill_(player_num, leader_idx, 29);

        if (leader_idx != 65 && !has_cost_reduction_skill) {
            if (maintenance_cost < 1) {
                maintenance_cost = 1;
            }
            return maintenance_cost;
        }

        return 0;
    }

    /**
     * Calculates the maximum bonus for "Famous" or "Super Famous" skills among the player's active leaders.
     *
     * @param player_idx The index of the player whose leaders are being checked.
     * @return The highest bonus value found, or 0 if no leaders have these skills or meet status criteria.
     */
    bool Is_Fleet_Officer_Screen_() {
        return MOX::_officer_scrn_type == 0;
    }

    void __cdecl Check_For_Colony_Calculation_Update_() {
        for (int16_t i = 0; i < MAX_LEADERS; ++i) {
            s_leader_data& leader = MOX::_leaders[i];

            if (leader.type == 1) {
                int16_t original_star_id = MOX::_officers_original_star_id[i];

                if (leader.location != original_star_id && leader.status == 0) {
                    COLCALC::Star_Colony_Calculation_(original_star_id, MOX::_PLAYER_NUM);
                }
            }
        }
    }

    /**
     * @address 0043e280
     */
    void __cdecl Move_From_Limbo_To_Pool_() {
        for (int16_t i = 0; i < MAX_LEADERS; ++i) {
            if (MOX::_leaders[i].status == 2) {
                MOX::_leaders[i].status = 0;
                MOX::_leaders[i].eta = 0;
            }
        }
    }

    int16_t __cdecl Bonus_For_Famous_Heroes_(int16_t player_idx) {
        int16_t max_bonus = 0;
        int16_t current_skill_bonus = 0;

        for (int16_t leader_idx = 0; leader_idx < MAX_LEADERS; ++leader_idx) {
            auto& leader = MOX::_leaders[leader_idx];

            // Check if the leader belongs to the player and is in a valid status (0 or 1)
            if (leader.player_index == player_idx && (leader.status == 0 || leader.status == 1)) {

                bool has_famous = Officer_Has_General_Skill_(player_idx, leader_idx, 10 /* FAMOUS */);
                if (has_famous) {
                    current_skill_bonus = (int16_t)Officer_Skill_Bonus_(player_idx, leader_idx, 10 /* FAMOUS */);
                } else {
                    bool has_super_famous = Officer_Has_General_Skill_(player_idx, leader_idx, 11 /* SUPER_FAMOUS */);
                    if (has_super_famous) {
                        current_skill_bonus = (int16_t)Officer_Skill_Bonus_(player_idx, leader_idx, 11 /* SUPER_FAMOUS */);
                    }
                }

                // Track the maximum bonus found across all valid leaders
                if (current_skill_bonus > max_bonus) {
                    max_bonus = current_skill_bonus;
                }
            }
        }

        return max_bonus;
    }
}