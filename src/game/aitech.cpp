#include "deps.h"
#include <cstring>

namespace aitech {
    /**
     * @brief Applies evolutionary upgrades to players who have picked them but not yet applied them.
     * 
     * The function iterates through all players, checks if they have a pending evolutionary upgrade,
     * and if so, replaces their current traits with the upgrade traits while preserving the 
     * first trait (traits[0]).
     */
    void Apply_Evolutionary_Upgrades_() {
        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            s_player* plr = &MOX::_player[i];

            if (plr->picked_evolutionary_upgrade != 0 && plr->applied_evolutionary_upgrade == 0) {
                int8_t saved_trait_0 = plr->traits[0];
                
                memcpy(plr->traits, plr->evolutionary_upgrade_traits, sizeof(plr->traits));
                
                plr->traits[0] = saved_trait_0;
                
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
}