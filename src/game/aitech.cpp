#include "pch.h"

namespace aitech {
    int __cdecl Player_Best_Armor_(s_player* player) {
        // The loop checks armors from index 6 down to 1.
        // Index 0 is treated as the default return value if no other armor is researched.
        for (int i = 6; i > 0; --i) {
            // Check if the technology associated with this armor index is researched.
            // Value 3 typically represents TECH_STATE_RESEARCHED.
            int16_t tech_id = TECHDATA::_armor[i].tech_app_id;

            if (player->tech_applications[tech_id] == 3 /* TECH_STATE_RESEARCHED */) {
                return i;
            }
        }

        return 0;
    }

    int __cdecl Player_Best_Ship_Shield_(s_player* player) {
        int shield_idx = 5;

        // Loop iterates through shield indices 5 down to 1.
        // Index 0 is the default return value if no other shields are researched.
        while (shield_idx > 0) {
            // tech_app_id is at offset 0x06 in s_tech_shield_data
            int16_t tech_id = TECHDATA::_shields[shield_idx].tech_app_id;

            // Status 3 typically represents a researched/known technology.
            if (player->tech_applications[tech_id] == 3) {
                return shield_idx;
            }

            shield_idx--;
        }

        return shield_idx; // Returns 0
    }

    int __cdecl Player_Best_Rifle_(s_player* player) {
        int i = 5;
        while (i > 0) {
            if (player->tech_applications[TECHDATA::_rifles[i].tech_app_id] == 3) {
                return i;
            }
            i--;
        }
        return i;
    }

    int __cdecl Player_Best_Personal_Shield_(s_player* player) {
        int index = 1;
        s_tech_data_personal_shield* shield_data = &TECHDATA::_personal_shields[1];

        do {
            if (player->tech_applications[shield_data->tech_app_id] == 3 /* RESEARCHED */) {
                return index;
            }

            shield_data--;
            index--;
        } while (shield_data > TECHDATA::_personal_shields);

        return index;
    }

    int __cdecl Player_Best_Fuel_(s_player* player) {
        int fuel_idx = 5;

        while (fuel_idx > 0) {
            int16_t tech_idx = TECHDATA::_fuel[fuel_idx].tech_app_id;
            if (player->tech_applications[tech_idx] == 3) {
                return fuel_idx;
            }
            fuel_idx--;
        }

        return fuel_idx;
    }

    int __cdecl Player_Best_Computer_(s_player* player) {
        int computer_idx = 5;

        while (computer_idx > 0) {
            int16_t tech_app_idx = TECHDATA::_computers[computer_idx].tech_app_id;

            // Check if the player has researched the technology application associated with this computer.
            if (player->tech_applications[tech_app_idx] == 3 /* TECH_RESEARCHED */) {
                return computer_idx;
            }

            computer_idx--;
        }

        return computer_idx; // Returns 0
    }

    int __cdecl Player_Best_Planetary_Shield_(s_player* player) {
        // TECH_APP_PLANETARY_BARRIER_SHIELD = 0x198 (408)
        if (player->tech_applications[0x198] == 3) {
            return 3;
        }

        // TECH_APP_PLANETARY_FLUX_SHIELD = 0x199 (409)
        if (player->tech_applications[0x199] == 3) {
            return 2;
        }

        // TECH_APP_PLANETARY_RADIATION_SHIELD = 0x19d (413)
        if (player->tech_applications[0x19d] == 3) {
            return 1;
        }

        return 0;
    }

    int __cdecl Player_Best_Drive_(s_player* player) {
        int drive_idx = 6;

        // Loop from highest drive tier down to 1.
        // Index 0 (Nuclear Engine) is the fallback value and is not explicitly checked.
        while (drive_idx > 0) {
            // tech_app maps the drive to its corresponding entry in the tech_applications array.
            int16_t tech_app_idx = TECHDATA::_drives[drive_idx].tech_app_id;

            if (player->tech_applications[tech_app_idx] == 3) {
                return drive_idx;
            }

            drive_idx--;
        }

        return 0;
    }

    void Apply_Evolutionary_Upgrades_() {
        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            s_player* plr = &MOX::_player[i];

            if (plr->picked_evolutionary_upgrade != 0 && plr->applied_evolutionary_upgrade == 0) {
                int8_t govt = plr->traits[TRAIT_CURRENT_GOVERNMENT];
                
                memcpy(plr->traits, plr->evolutionary_upgrade_traits, sizeof(plr->traits));
                
                plr->traits[TRAIT_CURRENT_GOVERNMENT] = govt;
                
                plr->applied_evolutionary_upgrade = 1;
            }
        }
    }

    void __cdecl Compute_AI_Tech_Info_(s_ai_tech_info* info) {
        memset(info, 0, sizeof(s_ai_tech_info));

        for (int p_idx = 0; p_idx < MOX::_NUM_PLAYERS; ++p_idx) {
            s_player* plr = &MOX::_player[p_idx];
            if (plr->eliminated) continue;

            s_ai_tech_info_player* plr_info = &info->players[p_idx];

            plr_info->best_shield = aitech::Player_Best_Ship_Shield_(plr);
            plr_info->best_armor = aitech::Player_Best_Armor_(plr);
            plr_info->best_computer = aitech::Player_Best_Computer_(plr);
            plr_info->best_fuel = aitech::Player_Best_Fuel_(plr);
            plr_info->best_planet_shield = aitech::Player_Best_Planetary_Shield_(plr);
            plr_info->best_drive = aitech::Player_Best_Drive_(plr);

            // Calculate Best Weapon scores based on technology costs
            for (int w_idx = 1; w_idx <= 13; ++w_idx) {
                uint16_t app_id = TECHDATA::_weapons[w_idx].tech_application_id;
                if (plr->tech_applications[app_id] == 3 /* LEARNED */) {
                    int16_t field_id = TECHDATA::_technology_applications[app_id].tech_field_id;
                    uint8_t cost_score = static_cast<uint8_t>(TECHDATA::_technology_fields[field_id].cost / 100);
                    if (cost_score > static_cast<uint8_t>(plr_info->best_weapon1)) {
                        plr_info->best_weapon1 = cost_score;
                    }
                }
            }

            for (int w_idx = 14; w_idx <= 20; ++w_idx) {
                uint16_t app_id = TECHDATA::_weapons[w_idx].tech_application_id;
                if (plr->tech_applications[app_id] == 3 /* LEARNED */) {
                    int16_t field_id = TECHDATA::_technology_applications[app_id].tech_field_id;
                    uint8_t cost_score = static_cast<uint8_t>(TECHDATA::_technology_fields[field_id].cost / 100);
                    if (cost_score > static_cast<uint8_t>(plr_info->best_weapon2)) {
                        plr_info->best_weapon2 = cost_score;
                    }
                }
            }

            s_invasion_player_bonuses grnd_bonuses;
            invasion::Compute_Player_Ground_Combat_Bonuses_(static_cast<int16_t>(p_idx), &grnd_bonuses, 1);

            int16_t bonus = grnd_bonuses.powered_armor_bonus + grnd_bonuses.troop_bonus + 
                            (int8_t)grnd_bonuses.personal_shield_bonus + grnd_bonuses.antigrav_bonus + 
                            grnd_bonuses.low_g_penalty + (int8_t)grnd_bonuses.subterranean_bonus + 
                            grnd_bonuses.ground_combat_bonus;
            // space_academy_bonus is missing in existing structures, so it's ignored.
            bonus += static_cast<int16_t>(grnd_bonuses.rifle_bonus) >> 2;
            plr_info->ground_bonus = bonus;

            plr_info->groun_tech_bonus = grnd_bonuses.has_battleoids + 
                                         (grnd_bonuses.has_powered_armor + (int8_t)grnd_bonuses.heavy_g_bonus) * 4 + 4;
        }

        // Galactic comparison pass
        for (int p_idx = 0; p_idx < MOX::_NUM_PLAYERS; ++p_idx) {
            s_player* plr = &MOX::_player[p_idx];
            if (plr->eliminated) continue;

            s_ai_tech_info_player* plr_info = &info->players[p_idx];

            for (int other_idx = 0; other_idx < MOX::_NUM_PLAYERS; ++other_idx) {
                if (p_idx == other_idx || MOX::_player[other_idx].eliminated) continue;

                s_ai_tech_info_player* other_info = &info->players[other_idx];
                bool has_contact = (plr->contact[other_idx] != 0);

                // Tech difference accumulation (comparing how far behind current player is)
                auto accumulate_delta = [&](int16_t delta, int8_t& target, bool contact_only = false) {
                    if (delta > 0) {
                        if (has_contact) target += static_cast<int8_t>(delta * 3);
                        else if (!contact_only) target += static_cast<int8_t>(delta);
                    } else if (delta == 0 && !contact_only) {
                        target += static_cast<int8_t>(random::Random_(2) - 1);
                    }
                };

                accumulate_delta(static_cast<int16_t>(other_info->best_shield) - plr_info->best_shield, plr_info->best_shield_delta);
                accumulate_delta(static_cast<int16_t>(other_info->best_armor) - plr_info->best_armor, plr_info->armor_delta);
                accumulate_delta(static_cast<int16_t>(other_info->best_computer) - plr_info->best_computer, plr_info->best_computer_delta);
                accumulate_delta(static_cast<int16_t>(other_info->best_weapon1) - plr_info->best_weapon1, plr_info->best_weapon1_delta);
                accumulate_delta(static_cast<int16_t>(other_info->best_weapon2) - plr_info->best_weapon2, plr_info->best_weapon2_delta);

                if (has_contact) {
                    plr_info->contact_count++;
                    
                    // Planetary Shield Delta
                    int16_t ps_delta = static_cast<int16_t>(other_info->best_planet_shield) - plr_info->best_planet_shield;
                    if (ps_delta > 0) plr_info->planet_shield_delta += static_cast<int8_t>(ps_delta);

                    // Ground Tech/Bonus Delta
                    int16_t tech_delta = static_cast<int16_t>(other_info->groun_tech_bonus) - plr_info->groun_tech_bonus;
                    int16_t bonus_delta = other_info->ground_bonus - plr_info->ground_bonus;
                    
                    int32_t ground_accum = 0;
                    if (tech_delta > 0 && bonus_delta > 0) {
                        ground_accum = (static_cast<int32_t>(bonus_delta) * tech_delta) / 20;
                    } else if (tech_delta > 0) {
                        ground_accum = (tech_delta + (tech_delta < 0 ? 3 : 0)) >> 2; // Divide by 4
                    } else if (bonus_delta > 0) {
                        ground_accum = bonus_delta / 5;
                    }

                    if (ground_accum == 0 && (tech_delta > 0 || bonus_delta > 0)) ground_accum = 1;
                    plr_info->ground_bonus_delta += static_cast<int8_t>(ground_accum);
                }
            }

            // Adjustments for AI Objectives and Personality
            if (plr->objectives == 1 /* CONQUEROR */) {
                plr_info->best_shield_delta++;
                plr_info->best_computer_delta++;
            } else if (plr->objectives == 3 /* TECHNOLOGIST */) {
                plr_info->best_computer_delta++;
            }

            if (plr->personality == 2 /* AGGRESSIVE */) {
                plr_info->best_shield_delta++;
                plr_info->best_computer_delta++;
            }

            // Check for Colonization status
            bool can_colonize = (plr->tech_applications[129 /* THORIUM_FUEL_CELLS */] == 3) || 
                                (aimove::Best_Uncolonized_Undestined_Planet_(static_cast<int16_t>(p_idx)) != -1);
            
            plr_info->colonization_idle = can_colonize ? 0 : 1;
        }
    }

    void __cdecl All_AI_Tech_Select_() {
        s_ai_tech_info local_aitech_info;

        AIDATA::_aitech = &local_aitech_info;
        aitech::Compute_AI_Tech_Info_(&local_aitech_info);

        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            s_player* plr = &MOX::_player[i];

            if (plr->eliminated == 0 && plr->objectives != 100) {
                aitech::AI_Tech_Select_(plr);

                // Index 211 is TECH_APP_EVOLUTIONARY_MUTATION (last element of tech_applications[212])
                if (plr->tech_applications[211] == 3 && plr->picked_evolutionary_upgrade == 0) {
                    raceopt::Setup_Evolutionary_Upgrade_((short)i);
                }
            }
        }

        AIDATA::_aitech = nullptr;
    }

    int16_t __cdecl System_Proximity_Count_(int16_t player_id, uint8_t other_player_id) {
        uint8_t player_bit = 1 << (player_id & 0x1F);
        uint8_t other_player_bit = 1 << (other_player_id & 0x1F);
        int16_t score = 0;

        int num_stars = static_cast<int>(MOX::_NUM_STARS);
        for (int i = 0; i < num_stars; ++i) {
            s_star_data* star = &MOX::_star[i];

            // Offset 0x38: has_colony is a bitmask where bit N is set if player N has a colony
            if (star->has_colony & other_player_bit) {
                if (star->has_colony & player_bit) {
                    score += 5;
                } else if (SHIPMOVE::Star_In_Normal_Range_Of_Player_(star, player_id)) {
                    score += 1;
                }
            }
        }

        return score;
    }

    void __cdecl AI_Tech_Select_(s_player *player) {
        bool needs_new_tech = false;

        // Check if current field research is completed (level 3) or if a new app was recently acquired
        if (player->tech_fields[(uint8_t)player->current_research_field] == 3 || player->got_new_app != 0) {
            needs_new_tech = true;
        }

        player->got_new_app = 0;

        if (needs_new_tech) {
            int16_t player_idx = (int16_t)(player - MOX::_player);
            int16_t app_id = newtech::Choose_Tech_Application_(player_idx);

            player->current_research_application = (int8_t)app_id;
            
            // Look up field ID from technology applications table
            int8_t field_id = (int8_t)TECHDATA::_technology_applications[app_id].tech_field_id;
            player->current_research_field = field_id;

            // If we've reached a high-tier research field, stop active research selection logic
            if (field_id >= 75 /* HYPER_ADVANCED_THRESHOLD */) {
                player->do_not_research = 1;
            }
        }
    }
}