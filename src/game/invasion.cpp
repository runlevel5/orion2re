#include "pch.h"

namespace invasion {

void __cdecl Compute_Player_Ground_Combat_Bonuses_(int player_idx, s_invasion_player_bonuses* bonuses, char is_defender) {
    memset(bonuses, 0, sizeof(s_invasion_player_bonuses));

    if (player_idx >= 8) {
        bonuses->antigrav_bonus = 10;
        bonuses->battleoids_bonus = 10;
        bonuses->has_battleoids = 1;
        bonuses->powered_armor_bonus = 10;
        bonuses->has_powered_armor = 1;
        bonuses->best_armor_tech_app_id = (int8_t)TECHDATA::_armor[6].tech_app_id;
        bonuses->troop_bonus = TECHDATA::_armor[6].troops_bonus;
        bonuses->rifle_tech_app_id = (int8_t)TECHDATA::_rifles[5].tech_app_id;
        bonuses->rifle_bonus = TECHDATA::_rifles[5].bonus;
        bonuses->personal_shield_tech_app_id = (int8_t)TECHDATA::_personal_shields[1].tech_app_id;
        bonuses->personal_shield_bonus = TECHDATA::_personal_shields[1].bonus;
        bonuses->difficulty_bonus = (MOX::_settings.game_difficulty * 2) - 4;
        return;
    }

    s_player* player = &MOX::_player[player_idx];

    if (player->tech_applications[76 /* TECH_APP_ANTI_GRAV_HARNESS */] == 3) {
        bonuses->antigrav_bonus = 10;
    }
    if (player->tech_applications[91 /* TECH_APP_BATTLEOIDS */] == 3) {
        bonuses->battleoids_bonus = 10;
        bonuses->has_battleoids = 1;
    }
    if (player->tech_applications[211 /* TECH_APP_POWERED_ARMOR */] == 3) {
        bonuses->powered_armor_bonus = 10;
        bonuses->has_powered_armor = 1;
    }

    int armor_idx = aitech::Player_Best_Armor_(player);
    bonuses->best_armor_tech_app_id = (int8_t)TECHDATA::_armor[armor_idx].tech_app_id;
    bonuses->troop_bonus = TECHDATA::_armor[armor_idx].troops_bonus;

    int rifle_idx = aitech::Player_Best_Rifle_(player);
    bonuses->rifle_tech_app_id = (int8_t)TECHDATA::_rifles[rifle_idx].tech_app_id;
    bonuses->rifle_bonus = TECHDATA::_rifles[rifle_idx].bonus;

    int shield_idx = aitech::Player_Best_Personal_Shield_(player);
    bonuses->personal_shield_tech_app_id = (int8_t)TECHDATA::_personal_shields[shield_idx].tech_app_id;
    bonuses->personal_shield_bonus = TECHDATA::_personal_shields[shield_idx].bonus;

    bonuses->ground_combat_bonus = player->traits[TRAIT_GROUND_COMBAT_BONUS];

    if (player->traits[TRAIT_HEAVY_G_WORLD] != 0) {
        bonuses->heavy_g_bonus = 1;
    } else if (player->traits[TRAIT_LOW_G_WORLD] != 0) {
        bonuses->low_g_penalty = (int8_t)0xF6; /* -10 */
    }

    if (player->traits[TRAIT_SUBTERRANEAN] != 0 && is_defender != 0) {
        bonuses->subterranean_bonus = 10;
    }

    if (player->objectives == 100) {
        bonuses->difficulty_bonus = 0;
    } else {
        bonuses->difficulty_bonus = MOX::_settings.game_difficulty - 2;
    }
}

void __cdecl Compute_Colony_Officer_Bonus_(s_colony* colony, s_invasion_player_bonuses* bonuses) {
    int leader_idx = COLCALC::Colony_Officer_(colony);

    if (leader_idx != -1) {
        s_leader_data* leader = &MOX::_leaders[leader_idx];

        if (leader->general_skills & 0x8) {
            int16_t level = Owned_Officer_Level_(leader_idx);
            bonuses->additional_modifier = (level + 1) * 3;
        } else if (leader->general_skills & 0x4) {
            int16_t level = Owned_Officer_Level_(leader_idx);
            bonuses->additional_modifier = level * 2 + 2;
        }

        if (bonuses->additional_modifier > 0) {
            // Accessing the low byte of the officer short field
            bonuses->field17_0x12 = static_cast<int8_t>(colony->officer);
        }
    }
}

void __cdecl Compute_Ground_Combat_Info_(s_invasion_side_info* side, s_invasion_player_bonuses* own_mods, s_invasion_player_bonuses* opponent_mods) {
    int16_t base_strength = (uint8_t)own_mods->troop_bonus +
                            (uint8_t)own_mods->personal_shield_bonus +
                            (uint8_t)own_mods->rifle_bonus +
                            (uint8_t)own_mods->antigrav_bonus;

    int16_t global_mod = own_mods->ground_combat_bonus +
                         own_mods->low_g_penalty +
                         (uint8_t)own_mods->subterranean_bonus +
                         own_mods->difficulty_bonus +
                         own_mods->additional_modifier;

    for (int16_t i = 0; i < 4; ++i) {
        if (i == 3) {
            if (opponent_mods != nullptr) {
                side->unit_strengths[i] = (uint8_t)opponent_mods->troop_bonus +
                                          (uint8_t)opponent_mods->personal_shield_bonus +
                                          (uint8_t)opponent_mods->rifle_bonus +
                                          (uint8_t)opponent_mods->antigrav_bonus;
            } else {
                side->unit_strengths[i] = 0;
            }
        } else {
            side->unit_strengths[i] = base_strength;
        }

        side->unit_strengths[i] += global_mod;
        side->unit_hps[i] = own_mods->heavy_g_bonus + 1;

        switch (i) {
            case 0:
                side->unit_strengths[i] += 10;
                side->unit_hps[i] += 1;
                side->unit_strengths[i] += (uint8_t)own_mods->battleoids_bonus;
                side->unit_hps[i] += own_mods->has_battleoids;
                break;
            case 1:
                side->unit_strengths[i] += (uint8_t)own_mods->powered_armor_bonus;
                side->unit_hps[i] += own_mods->has_powered_armor;
                break;
            case 2:
                side->unit_strengths[i] -= 10;
                break;
            case 3:
                side->unit_strengths[i] -= 20;
                break;
        }
    }

    side->round_hit_unit_type = -1;
    side->round_killed_unit_type = -1;
    side->current_troop_type_idx = 0;

    Find_Troops_(side);
}

void __cdecl Get_Boarding_Info_(int16_t attacker_player_id, int16_t attacker_unit_count, int16_t attacker_additional_modifier, int16_t defender_player_id, int16_t defender_unit_count, int16_t defender_additional_modifier, s_invasion_side_info* side_a, s_invasion_side_info* side_b) {
    memset(side_a, 0, sizeof(s_invasion_side_info));
    side_a->colony_id = -1;
    side_a->ship_list = nullptr;
    side_a->player_id = attacker_player_id;
    side_a->opponent_player_id = defender_player_id;
    side_a->n_ships = 0;
    side_a->unit_counts[1] = attacker_unit_count;

    memset(side_b, 0, sizeof(s_invasion_side_info));
    side_b->colony_id = -1;
    side_b->ship_list = nullptr;
    side_b->player_id = defender_player_id;
    side_b->opponent_player_id = attacker_player_id;
    side_b->n_ships = 0;
    side_b->unit_counts[1] = defender_unit_count;

    invasion::Compute_Player_Ground_Combat_Bonuses_(side_a->player_id, &side_a->combat_modifiers, 0);
    side_a->combat_modifiers.additional_modifier = attacker_additional_modifier;

    invasion::Compute_Player_Ground_Combat_Bonuses_(side_b->player_id, &side_b->combat_modifiers, 0);
    side_b->combat_modifiers.additional_modifier = defender_additional_modifier;

    invasion::Compute_Ground_Combat_Info_(side_a, &side_a->combat_modifiers, &side_b->combat_modifiers);
    invasion::Compute_Ground_Combat_Info_(side_b, &side_b->combat_modifiers, &side_a->combat_modifiers);
}

void __cdecl Check_All_Rebellions_() {
    s_colony* colony = MOX::_colony;
    s_colony* end = &colony[MOX::_NUM_COLONIES];

    while (colony < end) {
        if (colony->owner != -1 && colony->outpost_flag == 0) {
            Check_Rebellion_(colony);
        }
        colony++;
    }
}

void __cdecl Ground_Combat_Round_(s_invasion_side_info* side1, s_invasion_side_info* side2) {
    int16_t roll1 = side1->unit_strengths[(uint8_t)side1->current_troop_type_idx] + (int16_t)random::Random_(100);
    int16_t roll2 = side2->unit_strengths[(uint8_t)side2->current_troop_type_idx] + (int16_t)random::Random_(100);

    side1->round_hit_unit_type = -1;
    side1->round_killed_unit_type = -1;

    if (roll2 >= roll1) {
        uint8_t idx = (uint8_t)side1->current_troop_type_idx;
        side1->current_unit_damage++;

        if (side1->unit_hps[idx] == side1->current_unit_damage) {
            side1->round_killed_unit_type = idx;
            side1->unit_counts[idx]--;
            side1->current_unit_damage = 0;

            if (side1->unit_counts[idx] == 0) {
                Find_Troops_(side1);
            }
        } else {
            side1->round_hit_unit_type = idx;
        }
    }

    side2->round_hit_unit_type = -1;
    side2->round_killed_unit_type = -1;

    if (roll2 <= roll1) {
        uint8_t idx = (uint8_t)side2->current_troop_type_idx;
        side2->current_unit_damage++;

        if (side2->unit_hps[idx] == side2->current_unit_damage) {
            side2->round_killed_unit_type = idx;
            side2->unit_counts[idx]--;
            side2->current_unit_damage = 0;

            if (side2->unit_counts[idx] == 0) {
                Find_Troops_(side2);
            }
        } else {
            side2->round_hit_unit_type = idx;
        }
    }
}

void __cdecl Resolve_Ground_Combat_(s_invasion_side_info* side_a, s_invasion_side_info* side_b) {
    while (static_cast<uint8_t>(side_a->current_troop_type_idx) < 4 &&
           static_cast<uint8_t>(side_b->current_troop_type_idx) < 4) {
        Ground_Combat_Round_(side_a, side_b);
    }
}

void __cdecl Find_Troops_(s_invasion_side_info* side_info) {
    while (side_info->unit_counts[side_info->current_troop_type_idx] == 0) {
        // Potential bug: out-of-bounds access to unit_counts[4] if current_troop_type_idx reaches 4
        if (side_info->current_troop_type_idx >= 4) {
            break;
        }
        side_info->current_troop_type_idx++;
    }
    side_info->current_unit_damage = 0;
}

/**
 * @brief Assigns a new home planet to a player by selecting their colony with the largest population.
 * 
 * @param player_idx The index of the player.
 * @param excluded_colony_idx An index of a colony to exclude from the search (usually the one being lost).
 */
void __cdecl Assign_New_Home_Planet_(int8_t player_idx, int16_t excluded_colony_idx) {
    int max_pops = 0;
    int best_colony_idx = -1;

    for (int i = 0; i < (int)MOX::_NUM_COLONIES; ++i) {
        // Check if the current colony is not the excluded one and belongs to the player
        if (i != (int)excluded_colony_idx && MOX::_colony[i].owner == (int8_t)player_idx) {
            // Population is handled as unsigned byte in comparison logic
            int current_pops = (uint8_t)MOX::_colony[i].n_pops;

            // Select the colony with the largest population. 
            // Ties result in selecting the colony with the higher index.
            if (current_pops >= max_pops) {
                max_pops = current_pops;
                best_colony_idx = i;
            }
        }
    }

    if (best_colony_idx == -1) {
        // No suitable colony found for a new home
        MOX::_player[player_idx].home_planet_id = -1;
    }
    else {
        // Assign the planet index of the best colony found
        MOX::_player[player_idx].home_planet_id = MOX::_colony[best_colony_idx].planet;
    }
}

int32_t __cdecl Colony_Infantry_Limit_(s_colony* param_1) {
    int32_t limit = MOX::_planet_max_population[MOX::_planet[param_1->planet].size];
    
    if (MOX::_player[param_1->owner].traits[TRAIT_WARLORD] == 0) {
        limit /= 2;
    }
    
    if (limit == 0) {
        limit = 1;
    }
    
    return limit;
}

int __cdecl Unload_Transports_(s_colony* colony, int16_t* transport_ship_ids, int num_transports) {
    int count = 0;
    int limit = Colony_Infantry_Limit_(colony);

    if (colony->military[0] < limit) {
        while (count < num_transports) {
            int16_t ship_idx = *transport_ship_ids;
            transport_ship_ids++;
            count++;
            
            SHIPS::Kill_Ship_(ship_idx);
            
            colony->military[0] += 4;
            if (colony->military[0] >= limit) {
                break;
            }
        }
    }

    COLCALC::Update_Player_Stats_(&MOX::_player[colony->owner]);
    
    return count;
}

void __cdecl Check_Rebellion_(s_colony* colony) {
    if (colony->occupation_policy == 4) {
        return;
    }

    int8_t rebel_weights[8] = {0};
    int total_rebel_pop = 0;

    if (colony->n_pops != 0) {
        for (int i = 0; i < colony->n_pops; i++) {
            uint32_t pop_flags = colony->pop[i];
            if ((pop_flags & 0x400) != 0) {
                int plr_idx = pop_flags & 0xF;
                if (!MOX::_player[plr_idx].eliminated) {
                    total_rebel_pop++;
                    rebel_weights[plr_idx]++;
                }
            }
        }
    }

    if (total_rebel_pop <= 0) {
        return;
    }

    int16_t rebel_player = RUSS::Get_Weighted_Choice_Char_(rebel_weights, MOX::_NUM_PLAYERS);

    int chance = total_rebel_pop * 10;

    if (MOX::_player[colony->owner].objectives == 100 /* 'd' */ &&
        MOX::_player[rebel_player].objectives != 100 /* 'd' */) {
        chance += MOX::_settings.game_difficulty * 4 - 8;
    }

    if (colony->buildings[1 /* BUILDING_ALIEN_CONTROL_CENTER */] != 0) {
        chance /= 2;
    }

    if (colony->occupation_policy == 0) {
        chance *= 2;
    }

    if (random::Random_(1000) <= chance) {
        int rebel_count = random::Random_(total_rebel_pop);
        s_invasion_side_info local_70;
        s_invasion_side_info local_38;

        Get_Rebellion_Info_(rebel_player, rebel_count, colony, &local_70, &local_38);
        Resolve_Ground_Combat_(&local_70, &local_38);

        s_MSG msg = {};
        msg.msg_type = 0 /* MSG_TYPE_COLONY */;
        msg.u.colony.planet_idx = static_cast<uint8_t>(colony->planet);
        msg.u.colony.subtype = 9 /* s_MSG_colony_rebellion */;
        msg.u.colony.c.rebels.owner_player = static_cast<int8_t>(rebel_player);
        msg.u.colony.c.rebels.rebels = static_cast<int8_t>(rebel_count);
        msg.u.colony.c.rebels.lost = static_cast<int8_t>(rebel_count - local_70.unit_counts[3]);

        MSG_::Add_Msg_(msg, colony->owner);
        MSG_::Add_Msg_(msg, rebel_player);

        if (local_70.current_troop_type_idx < 4) {
            Change_Colony_Ownership_(colony, local_70.player_id, 0);
            Resolve_Rebellion_Troops_(local_70);
            colony->just_produced = -41;
        }
    }
}

void __cdecl Get_Rebellion_Info_(int16_t attacker_player_id, int marines_count, s_colony* colony, s_invasion_side_info* side_a, s_invasion_side_info* side_b) {
    memset(side_a, 0, sizeof(s_invasion_side_info));

    side_a->player_id = attacker_player_id;
    side_a->opponent_player_id = (int16_t)colony->owner;
    side_a->ship_list = nullptr;
    side_a->n_ships = 0;
    side_a->colony_id = (int16_t)(colony - MOX::_colony);
    side_a->unit_counts[3] = (int16_t)marines_count;

    memset(side_b, 0, sizeof(s_invasion_side_info));

    side_b->player_id = (int16_t)colony->owner;
    side_b->opponent_player_id = side_a->player_id;
    side_b->ship_list = nullptr;
    side_b->n_ships = 0;
    side_b->colony_id = (int16_t)(colony - MOX::_colony);
    side_b->unit_counts[0] = colony->military[1]; // Tanks
    side_b->unit_counts[1] = colony->military[0]; // Infantry
    side_b->unit_counts[2] = Colony_N_Militia_(colony);

    Compute_Player_Ground_Combat_Bonuses_(side_a->player_id, &side_a->combat_modifiers, 0);
    side_a->combat_modifiers.additional_modifier = 0;

    Compute_Player_Ground_Combat_Bonuses_(side_b->player_id, &side_b->combat_modifiers, 1);
    Compute_Colony_Officer_Bonus_(colony, &side_b->combat_modifiers);

    Compute_Ground_Combat_Info_(side_a, &side_a->combat_modifiers, &side_b->combat_modifiers);
    Compute_Ground_Combat_Info_(side_b, &side_b->combat_modifiers, &side_a->combat_modifiers);
}

void __cdecl Resolve_Rebellion_Troops_(s_invasion_side_info info) {
    s_colony* colony = &MOX::_colony[info.colony_id];
    int16_t limit = (int16_t)Colony_Infantry_Limit_(colony);

    if (info.unit_counts[3] < 4) {
        colony->military[0] = info.unit_counts[3];
    } else {
        colony->military[0] = 4;
    }

    if (colony->military[0] >= limit) {
        colony->military[0] = limit;
    }

    COLCALC::Compute_Star_Colony_Stuff_(&MOX::_star[MOX::_planet[colony->planet].star_index]);
}

void __cdecl Change_Colony_Ownership_(s_colony* colony, uint32_t new_owner_idx, s_battle_results_info* results) {
    int8_t old_owner_idx = colony->owner;
    int16_t colony_idx = (int16_t)(colony - MOX::_colony);

    // Increase war losses for the old owner against the new owner
    MOX::_player[old_owner_idx].colony_war_losses[new_owner_idx] += 10;

    invasion::Change_Pop_Ownership_(colony_idx, new_owner_idx);

    // Remove defensive/security buildings upon capture
    if (colony->buildings[2] != 0 /* Marine Barracks */) {
        ERICNET::Remove_Building_(colony_idx, 2);
    }
    if (colony->buildings[22] != 0 /* Armor Barracks */) {
        ERICNET::Remove_Building_(colony_idx, 22);
    }
    if (colony->buildings[1] != 0 /* Space Academy */) {
        ERICNET::Remove_Building_(colony_idx, 1);
    }

    // Reset military units and roundoff buffers
    colony->military[0] = 0;
    colony->infantry_roundoff = 0;
    colony->military[1] = 0;
    colony->tank_roundoff = 0;

    // Record captured population for the new owner
    MOX::_player[new_owner_idx].captured_population += (uint8_t)colony->n_pops;

    if (results != nullptr) {
        int16_t captured_techs[4] = { 0 };
        int16_t weights[212];
        int32_t captured_count = 0;
        int32_t chance_base;

        // Determine base chance to capture tech from buildings
        if (MOX::_player[new_owner_idx].objectives == 100 /* 'd' - likely human or specific AI type */) {
            chance_base = 125;
        } else {
            chance_base = MOX::_settings.game_difficulty * 5 + 115;
        }

        // Stage 1: Attempt to capture tech from existing buildings
        for (int32_t i = 0; i < 49; ++i) {
            if (colony->buildings[i] != 0) {
                int16_t tech_app = TECHDATA::_buildings[i].tech_app;
                if (COLCALC::Player_May_Get_Tech_App_(&MOX::_player[new_owner_idx], tech_app)) {
                    if (random::Random_(1000) <= (chance_base / (captured_count + 1))) {
                        captured_techs[captured_count] = tech_app;
                        captured_count++;
                        COLCALC::Player_Gets_Tech_App_(&MOX::_player[new_owner_idx], tech_app);
                        
                        if (captured_count >= 4) {
                            break;
                        }
                    }
                }
            }
        }

        // Stage 2: Attempt to capture remaining techs from the owner's knowledge
        if (captured_count < 4) {
            int32_t chance_random;
            if (MOX::_player[new_owner_idx].objectives == 100) {
                chance_random = 65;
            } else {
                chance_random = MOX::_settings.game_difficulty * 5 + 55;
            }

            memset(weights, 0, sizeof(weights));
            for (int32_t i = 0; i < 204 /* TECH_APP_COUNT_EXCLUDING_HIDDEN */; ++i) {
                if (MOX::_player[old_owner_idx].tech_applications[i] == 3 /* Fully Known */) {
                    if (COLCALC::Player_May_Get_Tech_App_(&MOX::_player[new_owner_idx], i)) {
                        weights[i] = 1;
                    }
                }
            }

            int32_t n_tries = (uint8_t)colony->n_pops;
            while (n_tries > 0 && captured_count < 4) {
                if (random::Random_(1000) <= (chance_random / (captured_count + 1))) {
                    int16_t tech_id = RUSS::Get_Weighted_Choice_Short_(weights, 204);
                    if (tech_id == -1) {
                        n_tries = 0; // No more techs to capture
                    } else {
                        weights[tech_id] = 0;
                        captured_techs[captured_count] = tech_id;
                        captured_count++;
                        COLCALC::Player_Gets_Tech_App_(&MOX::_player[new_owner_idx], tech_id);
                    }
                }
                n_tries--;
            }
        }

        // Write captured tech info to battle results
        memcpy(results->captured_techs_data, captured_techs, captured_count * sizeof(int16_t));
        results->captured_tech_id = (int16_t)captured_count;
        results->captured_tech_planet_id = colony->planet;
    }
}

int __cdecl Colony_N_Militia_(s_colony* colony) {
    int count = 0;
    uint8_t n_pops = static_cast<uint8_t>(colony->n_pops);

    for (uint8_t i = 0; i < n_pops; ++i) {
        uint32_t p = colony->pop[i];

        if (((p & 0x0F) < 8) && ((p & 0x0400) == 0)) {
            count++;
        }
    }

    return count / 5;
}

void __cdecl Change_Pop_Ownership_(int16_t colony_idx, uint32_t new_owner) {
    s_colony* colony = &MOX::_colony[colony_idx];
    int8_t old_owner = colony->owner;

    BOMB::Handle_Leader_At_Lost_Colony_(colony);

    MOX::_player[old_owner].colony_war_losses[new_owner] += (int16_t)colony->n_pops;

    if (colony->buildings[9 /* BUILDING_CAPITOL */] != 0) {
        ERICNET::Remove_Building_(colony_idx, 9 /* BUILDING_CAPITOL */);
    }

    if (MOX::_player[old_owner].home_planet_id == colony->planet) {
        invasion::Assign_New_Home_Planet_(old_owner, colony_idx);
    }

    colony->owner = (int8_t)new_owner;
    COLCALC::Do_Colony_Owner_Change_Star_Colony_Stuff_(colony);

    if (MOX::_player[new_owner].home_planet_id == -1) {
        MOX::_player[new_owner].home_planet_id = colony->planet;
    }

    colony->auto_building = 0;

    // Check for specific traits (e.g., Unification/Federation) that scrap morale buildings
    if (MOX::_player[new_owner].traits[0] / 2 == 3) {
        if (colony->buildings[20] != 0) {
            ERICNET::Remove_Building_(colony_idx, 20);
            MOX::_player[new_owner].bc += TECHDATA::_buildings[20].cost / 2;
        }
        if (colony->buildings[31 /* BUILDING_PLEASURE_DOME */] != 0) {
            ERICNET::Remove_Building_(colony_idx, 31 /* BUILDING_PLEASURE_DOME */);
            MOX::_player[new_owner].bc += TECHDATA::_buildings[31].cost / 2;
        }
    }

    int n_pops = (uint8_t)colony->n_pops;
    for (int i = 0; i < n_pops; ++i) {
        uint32_t pop_val = colony->pop[i];
        uint32_t race = pop_val & 0xF;
        uint32_t owner = (pop_val & 0x70) >> 4;

        if (race == new_owner) {
            pop_val &= ~0x400; // Clear conquered bit
            pop_val = (pop_val & ~0x70) | ((new_owner << 4) & 0x70);
        } else if (race == 8 || race == 9) {
            // Special races (likely Androids) just update owner
            pop_val = (pop_val & ~0x70) | ((new_owner << 4) & 0x70);
        } else {
            if (owner == new_owner) {
                pop_val &= ~0x400; // Clear conquered bit
            } else if (MOX::_player[new_owner].traits[25] != 0) {
                // Assimilation trait (e.g. Telepathic)
                pop_val &= ~0x400;
                pop_val = (pop_val & ~0x70) | ((new_owner << 4) & 0x70);
            } else {
                pop_val |= 0x400; // Set conquered bit
            }
        }
        colony->pop[i] = pop_val;
    }

    COLCALC::Colony_Environmental_Stuff_(colony);
    invasion::Enforce_Population_Limits_At_Colony_(colony);
    COLBLDG::Clear_Colony_Queue_(colony_idx);

    colony->production_spent = 0;
    colony->just_produced = -40;
    colony->occupation_points = 0;
    colony->occupation_policy = 4;
    colony->buildings[11] = 0; // Likely clearing Outpost/Colony Base status

    int star_idx = (uint8_t)MOX::_planet[colony->planet].star_index;
    COLCALC::Compute_Star_Colony_Stuff_(&MOX::_star[star_idx]);

    colony->n_turns_existed = 0;
}

void __cdecl Enforce_Population_Limits_At_Colony_(s_colony* colony) {
    int16_t race_pop_limit[10];
    int16_t race_pop_count[10];
    uint8_t limit_has_pops[42];

    memset(limit_has_pops, 0, sizeof(limit_has_pops));
    memset(race_pop_count, 0, sizeof(race_pop_count));
    for (int i = 0; i < 10; ++i) {
        race_pop_limit[i] = -1;
    }

    for (int i = 0; i < (int)colony->n_pops; ++i) {
        uint32_t pop = colony->pop[i];
        int race = (int)(pop & 0xf);
        race_pop_count[race]++;
        if (race_pop_limit[race] == -1) {
            race_pop_limit[race] = COLCALC::Colony_Race_Pop_Limit_(colony, (int16_t)race);
            limit_has_pops[race_pop_limit[race]] = 1;
        }
    }

    RUSS::Shuffle_(colony->pop, 4, (int16_t)colony->n_pops);

    for (int current_limit = 1; current_limit < 42; ++current_limit) {
        if (limit_has_pops[current_limit] == 0) {
            continue;
        }

        bool limit_satisfied = false;
        while (!limit_satisfied) {
            uint32_t* ptr_race_9 = nullptr;
            uint32_t* ptr_diff_orig_owner = nullptr;
            uint32_t* ptr_same_orig_owner = nullptr;
            uint32_t* ptr_race_8 = nullptr;
            uint32_t* ptr_owner_race = nullptr;
            int pops_with_limit = 0;

            for (int i = 0; i < (int)colony->n_pops; ++i) {
                uint32_t pop = colony->pop[i];
                int race = (int)(pop & 0xf);
                if (race_pop_limit[race] <= current_limit) {
                    pops_with_limit++;
                    if (race == 9) {
                        ptr_race_9 = &colony->pop[i];
                    } else if (race == 8) {
                        ptr_race_8 = &colony->pop[i];
                    } else if ((int)colony->owner == race) {
                        ptr_owner_race = &colony->pop[i];
                    } else {
                        int original_owner = (int)((pop & 0x70) >> 4);
                        if (original_owner == (int)colony->owner) {
                            ptr_same_orig_owner = &colony->pop[i];
                        } else {
                            ptr_diff_orig_owner = &colony->pop[i];
                        }
                    }
                }
            }

            if (pops_with_limit <= current_limit) {
                limit_satisfied = true;
            } else {
                colony->n_pops--;
                uint32_t* target_ptr = nullptr;
                if (ptr_race_9 != nullptr) {
                    target_ptr = ptr_race_9;
                } else if (ptr_diff_orig_owner != nullptr) {
                    target_ptr = ptr_diff_orig_owner;
                } else if (ptr_same_orig_owner != nullptr) {
                    target_ptr = ptr_same_orig_owner;
                } else if (ptr_race_8 != nullptr) {
                    target_ptr = ptr_race_8;
                } else {
                    target_ptr = ptr_owner_race;
                }
                *target_ptr = colony->pop[(int)colony->n_pops];
            }
        }
    }

    if (COLCALC::Colony_Food2_Per_Farmer_(colony, (int16_t)colony->owner, nullptr) == 0) {
        for (int i = 0; i < (int)colony->n_pops; ++i) {
            uint32_t pop = colony->pop[i];
            if ((pop & 0x180) == 0 && (pop & 0xf) != 8) {
                colony->pop[i] = (pop & 0xfffffeff) | 0x80;
            }
        }
    }
}

uint32_t __cdecl Colony_Infantry_Barracks_Limit_(s_colony* colony) {
    int32_t limit = static_cast<uint8_t>(colony->n_pops);

    if (MOX::_player[colony->owner].traits[TRAIT_WARLORD] == 0) {
        limit /= 2;
    }

    if (limit == 0) {
        limit = 1;
    }

    return limit;
}

int __cdecl Colony_Tank_Barracks_Limit_(s_colony* colony) {
    int limit;
    int pops = static_cast<uint8_t>(colony->n_pops);

    if (MOX::_player[colony->owner].traits[30 /* TRAIT_WARLORD */] == 0) {
        limit = pops / 4;
    } else {
        limit = pops / 2;
    }

    if (limit == 0) {
        limit = 1;
    }

    return limit;
}

}