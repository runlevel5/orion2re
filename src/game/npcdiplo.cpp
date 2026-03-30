#include "pch.h"

namespace npcdiplo {
    void __cdecl Clear_Last_Proposal_Turn_(int16_t player_id_1, int16_t player_id_2) {
        MOX::_player[player_id_1].last_diplomacy_proposal_turn[player_id_2] = 0;
        MOX::_player[player_id_2].last_diplomacy_proposal_turn[player_id_1] = 0;
    }

    void __cdecl NPC_Diplomacy_Buildup_Penalties_() {
        // Only run every 2 stardates after date 35000
        int32_t date_diff = static_cast<int32_t>(MOX::_stardate) - 35000;
        if (date_diff % 2 != 0) {
            return;
        }

        if (MOX::_NUM_PLAYERS <= 0) {
            return;
        }

        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            for (int16_t j = 0; j < MOX::_NUM_PLAYERS; ++j) {
                if (i == j) {
                    continue;
                }

                // Skip if either player is pursuing "objective" status (e.g., Guardian/Antaran)
                if (MOX::_player[i].objectives == 100) {
                    continue;
                }

                if (MOX::_player[j].objectives == 100) {
                    continue;
                }

                // Only consider active players with colonies
                if (MOX::_player[j].total_colonies <= 0) {
                    continue;
                }

                // Check for non-aggression or alliance treaties
                int8_t treaty_status = MOX::_player[j].treaty[i];
                if (treaty_status == 2 || treaty_status == 3) {
                    continue;
                }

                int32_t threshold = static_cast<int32_t>(MOX::_settings.galaxy_size) * 3 + 3;
                if (threshold >= MOX::_player[i].total_colonies) {
                    continue;
                }

                // 25% chance per eligible pair per turn to apply penalty
                if (random::Random_(4) == 1) {
                    int32_t difficulty_mod = (static_cast<int32_t>(MOX::_settings.galaxy_size) - MOX::_settings.game_difficulty) + 6;
                    int32_t expansion_excess = threshold - MOX::_player[i].total_colonies;

                    // Note: expansion_excess is negative here
                    int32_t base_penalty = (expansion_excess / difficulty_mod) * random::Random_(4);
                    int16_t relations_change = static_cast<int16_t>((base_penalty / 3) * 4);

                    // Redundant check against assembly: always true given previous JZ skip
                    if (MOX::_player[i].objectives != 100) {
                        relations_change /= 2;
                    }

                    diplomac::Change_Relations_(relations_change, i, j, 8 /* Expansion buildup penalty */, 0, 0);
                }
            }
        }
    }

    void __cdecl NPC_Diplomacy_() {
        diplomac::NPC_Sneak_Attacks_();
        NPC_Diplomacy_Buildup_Penalties_();
        NPC_Threats_();
        NPC_To_NPC_Treaty_Negotiations_();
        NoOp3();
        NPC_Declarations_Of_War_();
        NPC_Peace_Negotiations_();
        NPC_Tech_Exchange_();
    }

    void __cdecl NPC_Threats_() {
        for (int p1_idx = 0; p1_idx < MOX::_NUM_PLAYERS; ++p1_idx) {
            s_player& p1 = MOX::_player[p1_idx];
            if (p1.objectives == 100 /* 'd' */) {
                continue;
            }

            for (int p2_idx = 0; p2_idx < MOX::_NUM_PLAYERS; ++p2_idx) {
                s_player& p2 = MOX::_player[p2_idx];
                if (p2.objectives == 100 /* 'd' */) {
                    continue;
                }

                short severity = p1.diplomacy_severity[p2_idx];
                if (severity == 0) {
                    continue;
                }

                if (p1.diplomacy_incident[p2_idx] != 0 && severity != 10000) {
                    int8_t increment = 1;

                    // Treaty modifier check: Alliance, Peace, or Trade/Research/Tribute
                    if ((p1.treaty[p2_idx] >= 1 && p1.treaty[p2_idx] <= 3) ||
                        p1.trade_treaty[p2_idx] != 0 ||
                        p1.research_treaty[p2_idx] != 0 ||
                        p1.tribute_treaty[p2_idx] < 0)
                    {
                        increment = 2;
                    }

                    if (severity > 0) {
                        // Positive severity reduces threats over time
                        if (random::Random_(100) < (severity * 2)) {
                            p1.threats[p2_idx]--;
                            if (p1.threats[p2_idx] < 0) {
                                p1.threats[p2_idx] = 0;
                            }
                            p2.threats[p1_idx] = p1.threats[p2_idx];
                        } else {
                            // Resolve incident if no threat reduction occurred
                            p1.diplomacy_incident[p2_idx] = 0;
                        }
                    } else {
                        // Negative severity (diplomatic insult/attack) increases threats
                        short threat_threshold;
                        if (p1.personality == 4 /* Aggressive/Ruthless */ && p1.dishonored_flag[p2_idx] == 1) {
                            threat_threshold = diplodef::_personality_threat_modifiers[6];
                        } else {
                            threat_threshold = diplodef::_personality_threat_modifiers[p1.personality];
                        }

                        // Severity is negative, so subtracting it increases the random threshold
                        if (random::Random_(100) < (threat_threshold - severity * 2) || increment == 2) {
                            p1.threats[p2_idx] += increment;
                            p2.threats[p1_idx] = p1.threats[p2_idx];
                        } else {
                            // Resolve incident if no threat increase occurred
                            p1.diplomacy_incident[p2_idx] = 0;
                        }
                    }
                }
            }
        }
    }

    void __cdecl NPC_Declarations_Of_War_() {
        int16_t decision[8];
        int16_t reason[8];

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            auto& p_source = MOX::_player[i];

            if (p_source.objectives == 100 /* HUMAN_OR_INVALID */ || p_source.eliminated) {
                continue;
            }

            int16_t flux = EVENTS::Event_Check_Hyperspace_Flux_();
            if (flux != 0) {
                if (flux != 1 || p_source.traits[29 /* TRAIT_TRANS_DIMENSIONAL */] != 1) {
                    continue;
                }
            }

            for (int j = 0; j < MOX::_NUM_PLAYERS; ++j) {
                if (j != i && p_source.treaty[j] >= 4 && p_source.contact[j] == 1) {
                    decision[j] = 3;
                } else {
                    decision[j] = 0;
                }
                reason[j] = 0;
            }

            // Case 1: Ruthless Aggression
            if (p_source.personality == 3 /* RUTHLESS */) {
                for (int j = 0; j < MOX::_NUM_PLAYERS; ++j) {
                    auto& p_target = MOX::_player[j];
                    if (j != i && decision[j] == 0 && p_source.within_range[j] == 1 &&
                        p_source.peace_duration[j] < 1 && !p_target.eliminated) {
                        
                        if (diplomac::Fleet_Strength_Comparison_(i, j) >= 100 && p_target.objectives != 100) {
                            int diff = MOX::_settings.game_difficulty;
                            int roll_limit = (diff * diff * 2 + 2) * 15;
                            if (random::Random_(roll_limit) == 1) {
                                decision[j] = 1;
                                reason[j] = 20 /* PERSONALITY_AGGRESSION */;
                            }
                        }
                    }
                }
            }

            // Case 2: Opportunistic Attack based on Relations
            for (int j = 0; j < MOX::_NUM_PLAYERS; ++j) {
                auto& p_target = MOX::_player[j];
                if (j != i && p_target.objectives != 100 && !p_target.eliminated) {
                    if (p_source.contact[j] == 1 && p_source.within_range[j] == 1 &&
                        p_source.peace_duration[j] < 1 && decision[j] == 0) {
                        
                        if (diplomac::Fleet_Strength_Comparison_(i, j) >= 100 && (int)MOX::_stardate % MOX::_NUM_PLAYERS == j) {
                            int diff = MOX::_settings.game_difficulty;
                            int threshold = (-5 - p_source.relations[j]) / (diff * 2 + 1);
                            if (random::Random_(100) <= threshold) {
                                decision[j] = 1;
                                reason[j] = 68 /* OPPORTUNISTIC_ATTACK */;
                            }
                        }
                    }
                }
            }

            // Case 3: Failed to Expand
            if (p_source.failed_to_expand == 1) {
                for (int j = 0; j < MOX::_NUM_PLAYERS; ++j) {
                    auto& p_target = MOX::_player[j];
                    if (j != i && p_target.objectives != 100 && !p_target.eliminated) {
                        if (p_source.contact[j] == 1 && p_source.within_range[j] == 1 &&
                            p_source.peace_duration[j] < 1 && decision[j] == 0) {
                            
                            if (diplomac::Fleet_Strength_Comparison_(i, j) >= 100) {
                                decision[j] = 1;
                                reason[j] = 113 /* EXPANSION_FAILURE */;
                            }
                        }
                    }
                }
            }

            // Case 4: Food Shortage desperation
            if (random::Random_(100) < p_source.food_shortage_duration) {
                for (int j = 0; j < MOX::_NUM_PLAYERS; ++j) {
                    auto& p_target = MOX::_player[j];
                    if (j != i && p_target.objectives != 100 && !p_target.eliminated) {
                        if (p_source.contact[j] == 1 && p_source.within_range[j] == 1 &&
                            p_source.peace_duration[j] < 1 && decision[j] == 0) {
                            
                            if (diplomac::Fleet_Strength_Comparison_(i, j) >= 100) {
                                decision[j] = 1;
                                reason[j] = 113 /* HUNGER_ATTACK */;
                            }
                        }
                    }
                }
            }

            // Case 5: Revenge for colony losses
            for (int j = 0; j < MOX::_NUM_PLAYERS; ++j) {
                auto& p_target = MOX::_player[j];
                if (j != i && p_target.objectives != 100 && !p_target.eliminated) {
                    if (p_source.contact[j] == 1 && p_source.within_range[j] == 1 &&
                        p_source.peace_duration[j] < 1 && decision[j] == 0) {
                        
                        if (p_source.colony_war_losses[j] > 0) {
                            decision[j] = 2;
                            reason[j] = 22 /* REVENGE */;
                        }
                    }
                }
            }

            // Case 6: Detailed personality threshold comparison
            for (int j = 0; j < MOX::_NUM_PLAYERS; ++j) {
                auto& p_target = MOX::_player[j];
                if (j != i && p_target.objectives != 100 && !p_target.eliminated) {
                    if (p_source.within_range[j] == 1 && decision[j] == 0 &&
                        p_source.peace_duration[j] < 1 && (int)MOX::_stardate % MOX::_NUM_PLAYERS == j) {
                        
                        int personality_idx = diplomac::Get_Player_Diplomacy_Personality_(i, j);
                        int threshold = diplodef::_personality_relation_modifiers[personality_idx] * 3 +
                                        random::Random_(200) + (MOX::_settings.game_difficulty + 5) * 25;
                        
                        int8_t t = p_source.treaty[j];
                        if (t == 1) threshold += 50;
                        else if (t == 2) threshold += 100;
                        else if (t == 3) threshold += 200;

                        if (p_source.trade_treaty[j]) threshold += 50;
                        if (p_source.research_treaty[j]) threshold += 50;
                        if (p_source.tribute_treaty[j] == 1) threshold += 50;
                        else if (p_source.tribute_treaty[j] == 2) threshold += 100;

                        for (int k = 0; k < MOX::_NUM_PLAYERS; ++k) {
                            if (k != i && k != j && !MOX::_player[k].eliminated && 
                                MOX::_player[k].objectives == 100 && MOX::_player[k].within_range[j] == 1) {
                                
                                int strength = diplomac::Fleet_Strength_Comparison_(k, i);
                                if (strength > 100) {
                                    threshold -= strength;
                                }
                            }
                        }

                        if (diplomac::Fleet_Strength_Comparison_(i, j) >= threshold) {
                            decision[j] = 1;
                            reason[j] = 23 /* CALCULATED_ATTACK */;
                        }
                    }
                }
            }

            // Case 7: Alliance protection logic (Difficulty >= Average)
            if (MOX::_settings.game_difficulty >= 3 /* HARD */) {
                for (int j = 0; j < MOX::_NUM_PLAYERS; ++j) {
                    if (j != i && !MOX::_player[j].eliminated && MOX::_player[j].objectives != 100) {
                        for (int k = 0; k < MOX::_NUM_PLAYERS; ++k) {
                            if (k != i && k != j && !MOX::_player[k].eliminated && MOX::_player[k].objectives != 100) {
                                if (MOX::_player[k].treaty[j] >= 4) {
                                    decision[j] = 0;
                                }
                            }
                        }
                    }
                }
            }

            // Execution Loop 1: Immediate/Forced Declarations
            for (int j = 0; j < MOX::_NUM_PLAYERS; ++j) {
                if (j != i && decision[j] == 2 && p_source.peace_duration[j] < 1) {
                    if (MOX::_player[j].objectives == 100 /* HUMAN */) {
                        diplomac::Change_Relations_(-10000, j, i, reason[j], 0, 0);
                    }
                    diplomac::Declare_War_(i, j);
                }
            }

            // Execution Loop 2: Randomly pick ONE from potential declarations
            int protective_treaties = 0;
            int potential_targets_count = 0;
            for (int j = 0; j < MOX::_NUM_PLAYERS; ++j) {
                if (j != i && p_source.peace_duration[j] < 1) {
                    if (p_source.treaty[j] >= 4 || p_source.sneak_attack_planet != -1) {
                        protective_treaties++;
                    }
                    if (decision[j] == 1) {
                        potential_targets_count++;
                    }
                }
            }

            if (protective_treaties == 0 && potential_targets_count > 0) {
                int target_idx = random::Random_(potential_targets_count);
                for (int j = 0; j < MOX::_NUM_PLAYERS; ++j) {
                    if (j != i && decision[j] == 1) {
                        if (target_idx == 0) {
                            if (MOX::_player[j].objectives == 100 /* HUMAN */) {
                                diplomac::Change_Relations_(-10000, j, i, reason[j], 0, 0);
                            }
                            diplomac::Declare_War_(i, j);
                            break;
                        }
                        target_idx--;
                    }
                }
            }
        }
    }

    void __cdecl NPC_Tech_Exchange_() {
        if (MOX::_NUM_PLAYERS <= 0) {
            return;
        }

        for (int16_t p1_idx = 0; p1_idx < (int16_t)MOX::_NUM_PLAYERS; ++p1_idx) {
            s_player* p1 = &MOX::_player[p1_idx];

            if (p1->objectives != 100 && p1->traits[TRAIT_REPULSIVE] == 0) {
                if (MOX::_stardate % (uint32_t)MOX::_NUM_PLAYERS == (uint32_t)p1_idx) {
                    for (int16_t p2_idx = 0; p2_idx < (int16_t)MOX::_NUM_PLAYERS; ++p2_idx) {
                        if (p1_idx == p2_idx) continue;

                        s_player* p2 = &MOX::_player[p2_idx];
                        if (p2->objectives != 100 && p2->traits[TRAIT_REPULSIVE] == 0 && p1->contact[p2_idx] != 0) {
                            if (random::Random_(100) < (int)p1->relations[p2_idx] + 50) {
                                if (random::Random_(4) <= (int)MOX::_settings.game_difficulty) {
                                    int16_t best_tech_for_p1 = -1;
                                    uint32_t max_val_for_p1 = 0;
                                    int16_t best_tech_for_p2 = -1;
                                    uint32_t max_val_for_p2 = 0;

                                    for (int16_t tech_id = 1; tech_id < 204; ++tech_id) {
                                        // P1 offers to P2
                                        if (p1->tech_applications[tech_id] == 3 && p2->tech_applications[tech_id] != 3) {
                                            if (COLCALC::Player_May_Get_Tech_App_(p2, tech_id)) {
                                                int16_t field_id = TECHDATA::_technology_applications[tech_id].tech_field_id;
                                                uint32_t cost = TECHDATA::_technology_fields[field_id].cost;
                                                if (TECHDATA::_technology_applications[tech_id].ai_group == 39) {
                                                    cost *= 1000;
                                                }
                                                if (cost > max_val_for_p2) {
                                                    max_val_for_p2 = cost;
                                                    best_tech_for_p2 = tech_id;
                                                }
                                            }
                                        }

                                        // P2 offers to P1
                                        if (p2->tech_applications[tech_id] == 3 && p1->tech_applications[tech_id] != 3) {
                                            if (COLCALC::Player_May_Get_Tech_App_(p1, tech_id)) {
                                                int16_t field_id = TECHDATA::_technology_applications[tech_id].tech_field_id;
                                                uint32_t cost = TECHDATA::_technology_fields[field_id].cost;
                                                if (TECHDATA::_technology_applications[tech_id].ai_group == 39) {
                                                    cost *= 1000;
                                                }
                                                if (cost > max_val_for_p1) {
                                                    max_val_for_p1 = cost;
                                                    best_tech_for_p1 = tech_id;
                                                }
                                            }
                                        }
                                    }

                                    if (best_tech_for_p1 != -1 && best_tech_for_p2 != -1) {
                                        COLCALC::Player_Gets_Tech_App_(p1, best_tech_for_p1);
                                        COLCALC::Player_Gets_Tech_App_(p2, best_tech_for_p2);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void __cdecl NPC_Peace_Negotiations_() {
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            auto& p_i = MOX::_player[i];

            if (p_i.objectives == 100 /* 'd' */ || p_i.eliminated != 0) {
                continue;
            }

            for (int16_t j = 0; j < (int16_t)MOX::_NUM_PLAYERS; ++j) {
                if (i == j) {
                    continue;
                }

                auto& p_j = MOX::_player[j];

                if (p_j.objectives == 100 /* 'd' */ || p_j.eliminated != 0) {
                    continue;
                }

                // Check if at war (treaty 4 or 5) and in contact
                if ((p_i.treaty[j] == 4 || p_i.treaty[j] == 5) && p_i.contact[j] == 1) {
                    int16_t war_count = 0;
                    for (int16_t k = 0; k < (int16_t)MOX::_NUM_PLAYERS; ++k) {
                        auto& p_k = MOX::_player[k];
                        if (p_k.objectives == 100 /* 'd' */ && p_k.eliminated == 0) {
                            if (i != k && j != k && p_i.treaty[k] >= 4) {
                                war_count++;
                            }
                        }
                    }

                    int threshold = (6 - MOX::_settings.game_difficulty) * 15 - (int)war_count * 20;

                    if (threshold < (int)(uint8_t)p_i.time_since_last_attack[j]) {
                        int16_t surrender_to = NPC_Surrender_Check_(i);
                        if (surrender_to == -1) {
                            diplomac::Declare_Peace_(j, i);
                        } else {
                            COLCALC::Surrender_Empire_(i, surrender_to);
                            EVENTS::Event_Surrender_((int8_t)i, surrender_to);
                        }
                    }
                }
            }
        }
    }

    void __cdecl NPC_Treaty_Hatred_() {
        if (MOX::_settings.game_difficulty < 3 /* Hard */) {
            return;
        }

        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            s_player& p_i = MOX::_player[i];

            if (p_i.objectives == 100 /* 'd' */) {
                int16_t hatred_score = 0;

                for (int16_t j = 0; j < (int16_t)MOX::_NUM_PLAYERS; ++j) {
                    if (j != i && p_i.contact[j]) {
                        if (p_i.treaty[j] == 1 /* Non-aggression treaty */) {
                            hatred_score += 100;
                        } else if (p_i.treaty[j] == 2 /* Alliance treaty */) {
                            hatred_score += 200;
                        }

                        if (p_i.trade_treaty[j] != 0) {
                            hatred_score += 50;
                        }

                        if (p_i.research_treaty[j] != 0) {
                            hatred_score += 50;
                        }
                    }
                }

                // Resistance to jealousy if Xeno Psychology learned
                if (p_i.tech_applications[200 /* TECH_APP_XENO_PSYCHOLOGY */] == 3) {
                    hatred_score = (int16_t)((hatred_score * 2) / 3);
                }

                // Reduction on Hard difficulty compared to Impossible
                if (MOX::_settings.game_difficulty == 3 /* Hard */) {
                    hatred_score /= 2;
                }

                if (hatred_score > 0) {
                    for (int16_t k = 0; k < (int16_t)MOX::_NUM_PLAYERS; ++k) {
                        s_player& p_k = MOX::_player[k];

                        // Check for contact and ensure p_k is not also looking for a diplomatic win 'd'
                        if (p_k.objectives != 100 /* 'd' */ &&
                            p_i.contact[k] &&
                            p_i.treaty[k] == 0 &&
                            p_i.research_treaty[k] == 0 &&
                            p_i.trade_treaty[k] == 0 &&
                            p_k.personality != 5) {
                            
                            // Relationship penalty based on total treaties held by the winner candidate
                            p_i.relations[k] -= (int8_t)(hatred_score / 100);

                            // Probabilistic reduction for the remainder
                            if (random::Random_(100) <= (hatred_score % 100)) {
                                p_i.relations[k] -= 1;
                            }

                            // Clamp relationship minimum
                            if (p_i.relations[k] < -100) {
                                p_i.relations[k] = -100;
                            }

                            // Mirror relationship changes
                            p_k.relations[i] = p_i.relations[k];
                        }
                    }
                }
            }
        }
    }

    void __cdecl NoOp3() {
        return;
    }

    /**
     * Checks if an NPC player should surrender and identifies a target empire to surrender to.
     */
    int16_t __cdecl NPC_Surrender_Check_(int16_t player_idx) {
        s_player* plr = &MOX::_player[player_idx];

        // Already surrendered
        if (plr->objectives == 100 /* d */) {
            return -1;
        }

        // Too early for surrender
        if ((int)(MOX::_stardate - 35000) < 150) {
            return -1;
        }

        int32_t max_pop = 1;
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            if (i != player_idx && !MOX::_player[i].eliminated) {
                if (max_pop < MOX::_player[i].total_pop) {
                    max_pop = MOX::_player[i].total_pop;
                }
            }
        }

        uint32_t max_strength = 1;
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            for (int16_t j = 0; j < (int16_t)MOX::_NUM_PLAYERS; ++j) {
                if (i != player_idx && j != player_idx && i != j) {
                    if (!MOX::_player[i].eliminated && !MOX::_player[j].eliminated) {
                        if (max_strength < (uint32_t)MOX::_player[i].total_ship_strength[j]) {
                            max_strength = MOX::_player[i].total_ship_strength[j];
                        }
                    }
                }
            }
        }

        int32_t contact_count = 0;
        int32_t total_strength_vs_npc = 0;
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            if (i != player_idx && plr->contact[i] == 1 && !MOX::_player[i].eliminated) {
                contact_count++;
                total_strength_vs_npc += plr->total_ship_strength[i];
            }
        }

        int32_t avg_strength = 0;
        if (contact_count > 0) {
            avg_strength = total_strength_vs_npc / contact_count;
        }

        int32_t pop_ratio = (plr->total_pop * 100) / max_pop;
        int32_t surrender_prob = 0;
        int32_t strength_ratio = (avg_strength * 100) / max_pop;

        if (pop_ratio < 26 || strength_ratio < 26) {
            surrender_prob = 5;
            if (pop_ratio < 26) {
                surrender_prob = ((26 - pop_ratio) * 5) / 12;
            }
            if (strength_ratio < 26) {
                surrender_prob = ((26 - strength_ratio) * surrender_prob) / 12;
            }
        }

        int32_t total_pop = plr->total_pop;
        if (total_pop < 1) total_pop = 1;

        int16_t losses = 0;
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            if (i != player_idx) {
                losses += plr->colony_war_losses[i];
            }
        }

        int32_t loss_factor = (int32_t)losses;
        loss_factor = ((loss_factor * loss_factor * 100) / (total_pop + loss_factor)) / (total_pop + loss_factor);

        if (surrender_prob < loss_factor) {
            surrender_prob = loss_factor;
        }

        if (random::Random_(100) > surrender_prob) {
            return -1;
        }

        int16_t best_target = -1;
        int16_t best_pop = 0;

        // Search 1: Pick best contact (Max Pop) with no war
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            if (plr->contact[i] == 1 && i != player_idx && !MOX::_player[i].eliminated) {
                if (COLCALC::May_Surrender_Empire_(player_idx, i) &&
                    MOX::_player[i].objectives != 100 /* d */ &&
                    plr->treaty[i] < 4 /* NO_WAR */) 
                {
                    if (best_pop <= MOX::_player[i].total_pop) {
                        best_target = i;
                        best_pop = MOX::_player[i].total_pop;
                    }
                }
            }
        }

        if (best_target == -1) {
            // Search 2: Pick best contact with minimal pop (Broken logic in original code, likely unreachable)
            best_pop = 0;
            for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
                if (plr->contact[i] == 1 && i != player_idx && !MOX::_player[i].eliminated) {
                    if (MOX::_player[i].objectives != 100 /* d */ &&
                        COLCALC::May_Surrender_Empire_(player_idx, i)) 
                    {
                        if (best_pop > MOX::_player[i].total_pop) {
                            best_target = i;
                            best_pop = MOX::_player[i].total_pop;
                        }
                    }
                }
            }
        }

        if (best_target == -1 && MOX::_settings.game_difficulty < 3 /* HARD */) {
            // Search 3: Low difficulty fallback (Broken logic, likely unreachable)
            best_pop = 0;
            for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
                if (plr->contact[i] == 1 && i != player_idx && !MOX::_player[i].eliminated) {
                    if (COLCALC::May_Surrender_Empire_(player_idx, i) &&
                        plr->treaty[i] < 4 /* NO_WAR */) 
                    {
                        if (best_pop > MOX::_player[i].total_pop) {
                            best_target = i;
                            best_pop = MOX::_player[i].total_pop;
                        }
                    }
                }
            }
        }

        if (best_target == -1 && MOX::_settings.game_difficulty < 3 /* HARD */) {
            // Search 4: Final low difficulty fallback (Max Pop)
            best_pop = 0;
            for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
                if (plr->contact[i] == 1 && i != player_idx && !MOX::_player[i].eliminated) {
                    if (COLCALC::May_Surrender_Empire_(player_idx, i)) {
                        if (best_pop < MOX::_player[i].total_pop) {
                            best_target = i;
                            best_pop = MOX::_player[i].total_pop;
                        }
                    }
                }
            }
        }

        return best_target;
    }

    void __cdecl Eliminate_NPC_Treaty_Conflicts_() {
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            int16_t k = i + 1;

            if (k >= (int16_t)MOX::_NUM_PLAYERS) {
                break;
            }

            if (MOX::_player[i].objectives != 100) {
                for (int16_t j = k; j < (int16_t)MOX::_NUM_PLAYERS; ++j) {
                    if (MOX::_player[j].objectives != 100) {
                        /*
                         * Check for diplomatic conflict:
                         * If player j and player k (i+1) are allied, but player i
                         * only has a peace treaty with both of them.
                         * 4 = ALLIANCE, 2 = PEACE
                         */
                        if (MOX::_player[j].treaty[k] >= 4 /* ALLIANCE */ &&
                            MOX::_player[i].treaty[j] == 2 /* PEACE */ &&
                            MOX::_player[i].treaty[k] == 2 /* PEACE */)
                        {
                            diplomac::Break_Treaties_(i, j);
                            diplomac::Break_Treaties_(i, k);
                        }
                    }
                }
            }
        }
    }

    void __cdecl Honor_Alliances_(int16_t player_idx, int16_t ally_idx) {
        if (MOX::_player[player_idx].treaty[ally_idx] == 2 /* TREATY_ALLIANCE */) {
            for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
                if (i == player_idx) {
                    continue;
                }

                if (MOX::_player[ally_idx].treaty[i] >= 4 /* TREATY_WAR */ &&
                    MOX::_player[player_idx].treaty[i] < 4 /* TREATY_WAR */ &&
                    MOX::_player[player_idx].contact[i] == 1 &&
                    MOX::_player[player_idx].within_range[i] == 1) {
                    
                    if (MOX::_player[i].objectives == 100 /* 'd' */) {
                        diplomac::Change_Relations_(-10000, i, player_idx, 19 /* 0x13 */, 0, 0);
                    }

                    MOX::_player[player_idx].diplomacy_proposal_war_player[ally_idx] = (int8_t)i;
                    diplomac::Declare_War_(player_idx, i);
                }
            }
        }
    }

    void __cdecl NPC_To_NPC_Treaty_Negotiations_() {
        if (MOX::_NUM_PLAYERS <= 0) {
            Eliminate_NPC_Treaty_Conflicts_();
            return;
        }

        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            s_player& plr_i = MOX::_player[i];
            if (plr_i.objectives == 100 /* HUMAN_PLAYER */) continue;

            for (int16_t j = 0; j < (int16_t)MOX::_NUM_PLAYERS; ++j) {
                s_player& plr_j = MOX::_player[j];

                if (plr_j.objectives == 100 /* HUMAN_PLAYER */) continue;
                if (plr_i.traits[TRAIT_REPULSIVE] != 0) continue;
                if (plr_j.traits[TRAIT_REPULSIVE] != 0) continue;
                if (i == j) continue;

                if (random::Random_(250 - (40 * (int32_t)MOX::_settings.game_difficulty)) != 1) continue;
                if (plr_i.contact[j] != 1) continue;

                if (plr_i.treaty[j] < 4 && plr_i.treaty_modifier[j] > -30) {
                    int32_t score_base = (int32_t)plr_i.relations[j] + (int32_t)plr_i.broken_treaties_modifier[j];

                    if (plr_i.treaty[j] == 1 /* TREATY_PEACE */) score_base += 20;
                    if (plr_i.treaty[j] == 2 /* TREATY_NON_AGGRESSION */) score_base += 40;
                    if (plr_i.trade_treaty[j] != 0) score_base += 20;

                    int32_t common_score = score_base;
                    for (int16_t k = 0; k < (int16_t)MOX::_NUM_PLAYERS; ++k) {
                        if (k != i && k != j) {
                            if (MOX::_player[k].treaty[i] >= 4) common_score += 10;
                            if (MOX::_player[k].treaty[j] >= 4) common_score += 20;
                            if (MOX::_player[k].threats[i] > 0) common_score += 5;
                        }
                    }

                    int32_t num_human_allies = 0;
                    int32_t num_ai_allies = 0;
                    for (int16_t k = 0; k < (int16_t)MOX::_NUM_PLAYERS; ++k) {
                        if (k != i && k != j) {
                            if (MOX::_player[k].treaty[j] >= 4 || MOX::_player[k].treaty[i] >= 4) {
                                if (MOX::_player[k].objectives == 100 /* HUMAN_PLAYER */) {
                                    num_human_allies++;
                                } else {
                                    num_ai_allies++;
                                }
                            }
                        }
                    }

                    int32_t alliance_score = common_score + 
                                             diplodef::_personality_relation_modifiers[(uint8_t)plr_j.personality] +
                                             diplodef::_personality_relation_modifiers[(uint8_t)plr_i.personality] +
                                             plr_i.treaty_modifier[j] + random::Random_(100) + (num_human_allies * 50);

                    if (alliance_score >= 200 && plr_i.treaty[j] == 1 /* TREATY_PEACE */ && num_ai_allies == 0) {
                        diplomac::Start_Treaty_(i, j, 2 /* ALLIANCE_TYPE */);
                    } else if (alliance_score >= 100 && plr_i.treaty[j] != 1 && plr_i.treaty[j] != 2) {
                        diplomac::Start_Treaty_(i, j, 1 /* PEACE_TYPE */);
                    }

                    int32_t trade_score = common_score + 
                                          diplodef::_personality_relation_modifiers[(uint8_t)plr_j.personality] +
                                          plr_i.trade_modifier[j] + random::Random_(100);

                    if (trade_score >= 111) {
                        if (plr_i.research_treaty[j] == 0) {
                            plr_i.research_treaty[j] = 1;
                            plr_j.research_treaty[i] = 1;
                        }
                    } else if (trade_score > 80) {
                        if (plr_i.trade_treaty[j] == 0) {
                            plr_i.trade_treaty[j] = 1;
                            plr_j.trade_treaty[i] = 1;
                        }
                    }

                    int32_t tribute_score_val = common_score + 
                                                diplodef::_personality_relation_modifiers[(uint8_t)plr_j.personality] +
                                                diplodef::_personality_relation_modifiers[(uint8_t)plr_i.personality] +
                                                plr_i.treaty_modifier[j] + random::Random_(100);

                    bool is_paying_tribute = false;
                    // BUG: loop index k not used, but exactly as decompiled
                    for (int16_t k = 0; k < (int16_t)MOX::_NUM_PLAYERS; ++k) {
                        if (plr_i.tribute_treaty[j] < 0) {
                            is_paying_tribute = true;
                        }
                    }

                    if (tribute_score_val > 150 && !is_paying_tribute) {
                        if (diplomac::Fleet_Strength_Comparison_(i, j) < 100) {
                            if (plr_i.bc_produced < plr_j.bc_produced && 
                                random::Random_(20) <= (int32_t)MOX::_settings.game_difficulty + 1) {
                                
                                random::Random_(4);
                                diplomac::Start_Tribute_Treaty_(i, j, 2);
                                
                                int32_t rel_change = random::Random_(3) + 3;
                                diplomac::Change_Relations_((int16_t)rel_change, j, i, 14 /* TRIBUTE_START */, 0, 0);
                            }
                        }
                    }
                }

                Honor_Alliances_(i, j);
                diplomac::Adjust_Diplomat_Modifiers_(i, j);
                diplomac::Adjust_Diplomat_Modifiers_(i, j);
                diplomac::Adjust_Diplomat_Modifiers_(i, j);
            }
        }

        Eliminate_NPC_Treaty_Conflicts_();
    }

    /**
     * @brief Adjusts the diplomacy proposal rejection value between two players based on personality and acceptance.
     *
     * This function is called when a diplomatic proposal is either accepted or rejected.
     * It modifies the rejection counter which tracks the player's history of rejecting proposals from another player.
     *
     * @param player_idx The index of the player whose personality influences the change.
     * @param other_player_idx The index of the other involved player.
     * @param accepted Whether the proposal was accepted (1) or rejected (0).
     */
    void __cdecl NPC_Proposal_Rejection_Accept_(int16_t player_idx, int16_t other_player_idx, int32_t accepted) {
        s_player* player = &MOX::_player[player_idx];
        int16_t rejection_val = player->diplomacy_proposal_rejection[other_player_idx];

        if (accepted == 1) {
            if (rejection_val < 0) {
                switch (player->personality) {
                    case 0: /* Ruthless */
                        rejection_val++;
                        break;
                    case 1: /* Aggressive */
                    case 2: /* Erratic */
                    case 3: /* Honorable */
                    case 4: /* Pacific */
                    case 5: /* Xenophobic */
                        rejection_val = 1;
                        break;
                }
            } else {
                rejection_val++;
            }
        } else {
            // Case for rejection (accepted != 1)
            if (rejection_val <= 0) {
                rejection_val--;
            } else {
                switch (player->personality) {
                    case 0: /* Ruthless */
                        rejection_val = -1;
                        break;
                    case 1: /* Aggressive */
                    case 2: /* Erratic */
                    case 3: /* Honorable */
                        rejection_val /= 2;
                        break;
                    case 4: /* Pacific */
                        if (player->dishonored_flag[other_player_idx] == 1) {
                            rejection_val = -1;
                        }
                        break;
                    case 5: /* Xenophobic */
                        rejection_val--;
                        break;
                }
            }
        }

        player->diplomacy_proposal_rejection[other_player_idx] = rejection_val;
        MOX::_player[other_player_idx].diplomacy_proposal_rejection[player_idx] = rejection_val;
    }

    bool __cdecl NPC_Good_Proposal_Check_(int16_t npc_idx, int16_t player_idx) {
        s_player* npc = &MOX::_player[npc_idx];
        s_player* other = &MOX::_player[player_idx];

        // Check if player has at least non-aggression or better
        if (npc->treaty[player_idx] >= 4) {
            return false;
        }

        int bonus = COLCALC::Best_Leader_Diplomat_Bonus_(player_idx);
        int score = bonus / -15;

        score -= diplodef::_personality_relation_modifiers[(uint8_t)npc->personality] / 5;
        score -= npc->broken_treaties_modifier[player_idx] / 20;
        score += npc->relations[player_idx] / 20;
        score += -npc->treaty_modifier[player_idx] / 20;
        score += npc->diplomacy_proposal_rejection[player_idx] * 3;
        score += 10 + MOX::_settings.game_difficulty * 4;

        if (other->treaty[npc_idx] == 1 /* TREATY_NON_AGGRESSION */) {
            score -= 4;
        } else if (other->treaty[npc_idx] == 2 /* TREATY_TRADE_AGREEMENT */) {
            score -= 8;
        }

        // Technology check (offset 200 within tech applications)
        if (other->tech_applications[200] == 3) {
            score -= 5;
        }

        if (score < 0) {
            score = 0;
        }
        if (score > 40) {
            score = 40;
        }

        int divisor = (score * score) / 50 + 1;
        int threshold = (int)npc->last_diplomacy_proposal_turn[player_idx] / divisor;

        int random_val = random::Random_(100);

        return random_val <= threshold;
    }

    int __cdecl reverse_compare_tech_(const void* a, const void* b) {
        return static_cast<const s_npcdiplo_tech*>(b)->value - static_cast<const s_npcdiplo_tech*>(a)->value;
    }

    void __cdecl Get_Differential_Tech_List_(
        int16_t player_idx,
        int16_t other_player_idx,
        uint32_t min_tech_value,
        uint32_t max_tech_value,
        uint8_t* tech_list_indices,
        int32_t* tech_list_values,
        int16_t* tech_count,
        int16_t sort_flag
    ) {
        *tech_count = 0;

        newtech::Set_Competition_Tech_Values_(player_idx);

        for (int16_t tech_idx = 0; tech_idx < 204; ++tech_idx) {
            if (MOX::_player[other_player_idx].tech_applications[tech_idx] == 3) {
                uint32_t tech_value = newtech::Calc_Tech_Value_(player_idx, tech_idx);

                if (tech_idx == 0x2a && MOX::_player[player_idx].traits[0] != 0) {
                    tech_value = 0;
                }
                if (tech_idx == 0x5c && MOX::_player[player_idx].traits[0] != 2) {
                    tech_value = 0;
                }
                if (tech_idx == 0x41 && MOX::_player[player_idx].traits[0] != 4) {
                    tech_value = 0;
                }
                if (tech_idx == 0x4d && MOX::_player[player_idx].traits[0] != 6) {
                    tech_value = 0;
                }

                if (!COLCALC::Player_May_Get_Tech_App_(&MOX::_player[player_idx], tech_idx)) {
                    tech_value = 0;
                }

                if (tech_value != 0 && tech_value >= min_tech_value && tech_value <= max_tech_value) {
                    _human_tech[*tech_count].index = static_cast<uint8_t>(tech_idx);
                    _human_tech[*tech_count].value = static_cast<int32_t>(tech_value);
                    *tech_count += 1;
                }
            }
        }

        if (sort_flag == 1 && *tech_count != 0) {
            _qsort(_human_tech, *tech_count, sizeof(s_npcdiplo_tech), reverse_compare_tech_);
        }

        if (*tech_count > 0) {
            for (int16_t i = 0; i < *tech_count; ++i) {
                tech_list_indices[i] = _human_tech[i].index;
                tech_list_values[i] = _human_tech[i].value;
            }
        }
    }

    void __cdecl NPC_Message_Rejection_Check_(int16_t player_idx, int16_t target_idx, int16_t* msg_type) {
        int16_t rejection_val = MOX::_player[player_idx].diplomacy_proposal_rejection[target_idx];
        if (rejection_val > -4) {
            return;
        }

        if (*msg_type != 0x8a && *msg_type != 0x8b && *msg_type != 0x8c &&
            *msg_type != 0x8d && *msg_type != 0x8e && *msg_type != 0x8f &&
            *msg_type != 0x90 && *msg_type != 0x91 && *msg_type != 0x92 &&
            *msg_type != 0x93 && *msg_type != 0x94 && *msg_type != 0x95 &&
            *msg_type != 0x96 && *msg_type != 0x97 && *msg_type != 0x98 &&
            *msg_type != 0xae) {
            return;
        }

        *msg_type = 0xaf;

        switch (MOX::_player[player_idx].personality) {
            case 0:
                rejection_val = static_cast<int16_t>(random::Random_(4) - 6);
                break;
            case 1:
            case 2:
                rejection_val = static_cast<int16_t>(-1 - random::Random_(3));
                break;
            case 3:
                rejection_val = static_cast<int16_t>(-random::Random_(4));
                break;
            case 4:
                if (MOX::_player[player_idx].dishonored_flag[target_idx] == 1) {
                    rejection_val = static_cast<int16_t>(random::Random_(4) - 6);
                } else {
                    rejection_val = static_cast<int16_t>(-random::Random_(3));
                }
                break;
            case 5:
                rejection_val = static_cast<int16_t>(-random::Random_(3));
                break;
            default:
                break;
        }

        MOX::_player[player_idx].diplomacy_proposal_rejection[target_idx] = rejection_val;
        MOX::_player[target_idx].diplomacy_proposal_rejection[player_idx] = rejection_val;
    }

    void __cdecl NPC_Tech_Exchange_Check_(int16_t player_idx, int16_t target_player_idx) {
        int16_t local_43a = 0;
        int16_t local_424[106];
        int32_t local_350[212];
        
        if (MOX::_player[player_idx].diplomacy_message[target_player_idx] == '\0' &&
            MOX::_player[player_idx].treaty[target_player_idx] < 4) 
        {
            int32_t expectation = Get_Tech_Exchange_Expectation_(target_player_idx, player_idx);
            if (expectation < 10) {
                expectation = 10;
            }
            
            int32_t num_elements = 0;
            newtech::Set_Competition_Tech_Values_(player_idx);
            
            for (int32_t i = 0; i < 204; ++i) {
                if (MOX::_player[target_player_idx].tech_applications[i] == 3) {
                    int32_t tech_value = static_cast<int32_t>(newtech::Calc_Tech_Value_(player_idx, static_cast<int16_t>(i)));
                    if ((tech_value * 2) != 0) {
                        _human_tech[num_elements].index = static_cast<uint8_t>(i);
                        _human_tech[num_elements].value = tech_value * 2;
                        num_elements++;
                    }
                }
            }
            
            if (num_elements > 0) {
                _qsort(_human_tech, num_elements, sizeof(s_npcdiplo_tech), reverse_compare_tech_);
                int32_t rand_idx = random::Random_((num_elements + 1) / 2) - 1;
                uint8_t selected_tech_index = _human_tech[rand_idx].index;
                int32_t exchange_val = (_human_tech[rand_idx].value * expectation) / 100;
                
                Get_Differential_Tech_List_(target_player_idx, player_idx, 0, (uint32_t)exchange_val, (uint8_t*)local_424, local_350, &local_43a, 0);
                
                if (local_43a > 0) {
                    MOX::_player[player_idx].diplomacy_message[target_player_idx] = 101; /* 'e' */
                    MOX::_player[player_idx].diplomacy_proposal_exchange_max_value[target_player_idx] = exchange_val;
                    MOX::_player[player_idx].diplomacy_proposal_exchange_tech[target_player_idx] = selected_tech_index;
                }
            }
        }
    }
}