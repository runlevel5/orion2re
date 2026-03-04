#include "pch.h"

namespace diplomac {
    // Suggested name: Sneak_Attack_Evaluations
    int16_t __cdecl Sneak_Attack_Evaluations_(int16_t player_id, int16_t target_id, int16_t *out_severity, int16_t *out_msg_id, int16_t *unused) {
        int8_t rel = MOX::_player[player_id].relations[target_id];
        int8_t rel_divisor = (rel < 50) ? 10 : 5;

        int16_t msg_id = 67 /* 0x43 */;
        int16_t severity = 200;
        int16_t score = rel / rel_divisor;
        int16_t min_mod = score;

        int8_t threats = MOX::_player[player_id].threats[target_id];
        if (threats > 0 && MOX::_player[player_id].last_bad_diplomatic_incident[target_id] != 0) {
            int16_t personality = Get_Player_Diplomacy_Personality_(player_id, target_id);
            int16_t threat_mod = -(int16_t)((threats * 10) / diplodef::_personality_max_threats[personality]);
            if (threat_mod < min_mod) {
                msg_id = (uint8_t)MOX::_player[player_id].last_bad_diplomatic_incident[target_id] + 70 /* 0x46 */;
                min_mod = threat_mod;
            }
            score += threat_mod;
        }

        if (MOX::_player[player_id].failed_to_expand == 1) {
            if (MOX::_player[player_id].objectives == 2 /* ECON_EXPANSION */ || 
                MOX::_player[player_id].total_pop > COLCALC::Get_Player_Max_Population_(target_id) / 2) {
                msg_id = 114 /* 0x72 */;
                min_mod = -150;
                score = -150;
            }
        }

        if (MOX::_player[player_id].personality == 3 /* RUTHLESS */) {
            if (random::Random_(200) <= MOX::_settings.game_difficulty + 1) {
                msg_id = 109 /* 0x6D */;
                severity = 100;
                min_mod = -150;
                score = -150;
            }
        }

        if (random::Random_(100) < MOX::_player[player_id].food_shortage_duration) {
            msg_id = 119 /* 0x77 */;
            min_mod = -150;
            score = -150;
        }

        if (MOX::_player[player_id].personality == 1 /* AGGRESSIVE */) {
            if (Fleet_Strength_Comparison_(player_id, target_id) >= 100) {
                if (MOX::_player[player_id].opportunity_actual_attacker_ratio[target_id] >= 200 && 
                    MOX::_player[player_id].opportunity_planet_num[target_id] != -1) {
                    int16_t opp_mod = (int16_t)(MOX::_player[player_id].opportunity_actual_attacker_ratio[target_id] / -20);
                    if (opp_mod < min_mod) {
                        msg_id = 115 /* 0x73 */;
                        min_mod = opp_mod;
                    }
                    score += opp_mod;
                }
            }
        }

        if (MOX::_player[player_id].personality == 0 /* XENOPHOBIC */) {
            if (aitech::System_Proximity_Count_(player_id, (uint8_t)target_id) >= random::Random_(400)) {
                msg_id = 121 /* 0x79 */;
                min_mod = -150;
                score = -150;
            }
        }

        int16_t trust_mod = (MOX::_player[player_id].trust_worthiness[target_id] * 3) / 5;
        if (trust_mod < min_mod && MOX::_player[player_id].trust_breaker_player[target_id] != -1) {
            min_mod = trust_mod;
            msg_id = 176 /* 0xB0 */ + (MOX::_player[player_id].trust_breaker_player[target_id] == player_id);
        }
        score += trust_mod;

        if ((Fleet_Strength_Comparison_(player_id, target_id) >= 300 || MOX::_player[player_id].total_pop * 3 > MOX::_player[player_id].total_pop * 2) && 
            MOX::_player[player_id].personality != 5 /* PACIFIST */) {
            int16_t str_mod = (int16_t)(Fleet_Strength_Comparison_(player_id, target_id) / -40);
            if (str_mod < min_mod) {
                msg_id = 112 /* 0x70 */;
                min_mod = str_mod;
            }
            severity = 150;
            score += str_mod;
        }

        int16_t active_players = 0;
        int16_t max_other_pop = 0;
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            if (MOX::_player[i].objectives != 100 && !MOX::_player[i].eliminated) {
                active_players++;
                if (i != player_id && MOX::_player[i].total_pop > max_other_pop) {
                    max_other_pop = MOX::_player[i].total_pop;
                }
            }
        }

        if (((active_players < 3 && MOX::_player[player_id].total_pop > max_other_pop) || active_players == 1)) {
            if (min_mod > -10) {
                msg_id = 178 /* 0xB2 */;
                min_mod = -10;
            }
            severity = 150;
            score -= 10;
        }

        int16_t turn = (int16_t)MOX::_stardate + 30535 /* 0x7747 */;
        if (turn > 99) {
            int16_t comparison_turn = (turn < 350) ? turn - 40 : 309;
            int16_t source_growth = (uint8_t)MOX::_player[player_id].population_history[turn] - (uint8_t)MOX::_player[player_id].population_history[comparison_turn];
            int16_t target_growth = (uint8_t)MOX::_player[target_id].population_history[turn] - (uint8_t)MOX::_player[target_id].population_history[comparison_turn];
            if (source_growth < target_growth) {
                int16_t growth_mod = (source_growth - target_growth) / 2;
                if (growth_mod < min_mod) {
                    msg_id = 117 /* 0x75 */;
                    min_mod = growth_mod;
                }
                score += growth_mod;
            }
        }

        if (MOX::_player[player_id].treaty[target_id] == 1) score += 10;
        if (MOX::_player[player_id].treaty[target_id] == 2) score += 20;

        int16_t personality_idx = Get_Player_Diplomacy_Personality_(player_id, target_id);
        score += _personality_break_peace_treaty_chance[personality_idx + 16];

        if (MOX::_player[target_id].tech_applications[200] == 3) score += 5;
        if (MOX::_player[target_id].traits[20] == 1 /* TRAIT_TELEPATHIC */) score += 10;

        score = (score + COLCALC::Best_Leader_Diplomat_Bonus_(target_id) - MOX::_settings.game_difficulty) + 15;

        int16_t final_prob;
        int8_t last_threat_turn = MOX::_player[player_id].last_diplomacy_threat_turn[target_id];

        if (score < 0) {
            final_prob = -(int16_t)(last_threat_turn * score);
        } else {
            uint32_t divisor = isqrt_(score * score * score + 5);
            final_prob = (divisor != 0) ? (int16_t)(last_threat_turn / divisor) : 0;
        }

        if ((MOX::_player[player_id].treaty[target_id] == 1 || MOX::_player[player_id].treaty[target_id] == 2) && 
            Get_Player_Diplomacy_Personality_(player_id, target_id) == 4) {
            final_prob = 0;
        }

        int16_t demands_count = (int16_t)(Fleet_Strength_Comparison_(player_id, target_id) / 40) - 1 + random::Random_(3);
        if (demands_count < 1) demands_count = 1;

        if (Get_Demands_(player_id, target_id, demands_count, 1, 1, 1, 1) == 0) {
            demands_count = 0;
        }

        if (random::Random_(100) <= final_prob && last_threat_turn > 9) {
            if (random::Random_(16) + MOX::_settings.game_difficulty < 16) {
                if (demands_count > 0 && min_mod != -150) {
                    bool is_most_populous = true;
                    for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                        if (i != player_id && MOX::_player[player_id].total_pop < MOX::_player[i].total_pop) {
                            is_most_populous = false;
                            break;
                        }
                    }

                    if (random::Random_(4) < 4 && is_most_populous && diplodef::_COUNCIL_FLAG == 1 && demands_count > 3) {
                        *out_severity = severity;
                        *out_msg_id = msg_id;
                        return 4; /* ULTIMATUM */
                    }

                    if (MOX::_player[player_id].traits[19] == 0 && MOX::_player[target_id].traits[19] == 0) {
                        if (isqrt_(demands_count + 4) < (uint32_t)random::Random_(4)) {
                            *out_severity = severity;
                            *out_msg_id = msg_id;
                            return 3; /* DEMAND */
                        }
                        *out_severity = severity;
                        *out_msg_id = msg_id;
                        return 1; /* ATTACK */
                    }
                }
            } else {
                *out_severity = severity;
                *out_msg_id = msg_id;
                return 2; /* THREAT */
            }
        }

        return 0;
    }

    void __cdecl NPC_Sneak_Attacks_() {
        int16_t eval_results[8];
        int16_t eval_messages[8];
        int16_t eval_ratios[8];
        int16_t eval_unknown[8];

        for (int16_t p1 = 0; p1 < (int16_t)MOX::_NUM_PLAYERS; ++p1) {
            s_player* plr1 = &MOX::_player[p1];

            if (plr1->objectives == 100 || plr1->eliminated) {
                continue;
            }

            int16_t human_war_count = 0;
            int16_t npc_war_count = 0;

            for (int16_t p2 = 0; p2 < (int16_t)MOX::_NUM_PLAYERS; ++p2) {
                if (plr1->treaty[p2] >= 4) {
                    if (MOX::_player[p2].objectives == 100) {
                        human_war_count++;
                    } else {
                        npc_war_count++;
                    }
                }
            }

            if (plr1->sneak_attack_planet == -1 && human_war_count == 0) {
                for (int16_t p2 = 0; p2 < (int16_t)MOX::_NUM_PLAYERS; ++p2) {
                    if (plr1->opportunity_planet_num[p2] == -1) {
                        plr1->opportunity_planet_value[p2] = 0;
                        plr1->opportunity_actual_attacker_ratio[p2] = 0;
                    }
                }

                for (int16_t p2 = 0; p2 < (int16_t)MOX::_NUM_PLAYERS; ++p2) {
                    s_player* plr2 = &MOX::_player[p2];
                    if (plr2->objectives == 100 &&
                        plr1->treaty[p2] < 4 &&
                        plr1->within_range[p2] == 1 &&
                        plr1->contact[p2] == 1 &&
                        !plr2->eliminated &&
                        plr1->last_turn_treaty[p2] != 1 &&
                        plr1->last_turn_treaty[p2] != 2 &&
                        plr1->last_turn_treaty[p2] != 3) 
                    {
                        eval_results[p2] = Sneak_Attack_Evaluations_(p1, p2, &eval_ratios[p2], &eval_messages[p2], &eval_unknown[p2]);
                    } else {
                        eval_results[p2] = 0;
                    }
                }

                for (int16_t p2 = 0; p2 < (int16_t)MOX::_NUM_PLAYERS; ++p2) {
                    if (MOX::_player[p2].objectives != 100) {
                        continue;
                    }

                    if (plr1->peace_duration[p2] != 0) {
                        int16_t personality = Get_Player_Diplomacy_Personality_(p1, p2);
                        int16_t break_chance = _personality_break_peace_treaty_chance[personality];
                        if (random::Random_(100) >= (int32_t)break_chance) {
                            if (eval_results[p2] != 0) {
                                plr1->war_buildup_duration = (int8_t)random::Random_(20) + 20;
                            }
                            continue;
                        }
                    }

                    if (eval_results[p2] == 2) {
                        if (Get_Player_Diplomacy_Personality_(p1, p2) != 4) {
                            Clear_Last_Proposal_Turn_(p1, p2);
                            if (plr1->opportunity_planet_num[p2] == -1 || 
                                plr1->opportunity_actual_attacker_ratio[p2] <= (int32_t)eval_ratios[p2]) 
                            {
                                plr1->war_buildup_duration = (int8_t)random::Random_(20) + 20;
                                if (npc_war_count > 0 && random::Random_(4) < (int32_t)MOX::_settings.game_difficulty) {
                                    for (int16_t p3 = 0; p3 < (int16_t)MOX::_NUM_PLAYERS; ++p3) {
                                        if (MOX::_player[p3].objectives != 100 && plr1->treaty[p3] >= 4) {
                                            Declare_Peace_(p1, p3);
                                        }
                                    }
                                }
                            } else if (plr1->opportunity_planet_value[p2] > 0) {
                                plr1->sneak_attack_planet = plr1->opportunity_planet_num[p2];
                                plr1->sneak_attack_message = (int8_t)eval_messages[p2];
                                plr1->war_buildup_duration = (int8_t)random::Random_(20) + 20;
                                if (npc_war_count > 0) {
                                    for (int16_t p3 = 0; p3 < (int16_t)MOX::_NUM_PLAYERS; ++p3) {
                                        if (MOX::_player[p3].objectives != 100 && plr1->treaty[p3] >= 4) {
                                            Declare_Peace_(p1, p3);
                                        }
                                    }
                                }
                            }
                        }
                    } else if (eval_results[p2] == 1) {
                        Clear_Last_Proposal_Turn_(p1, p2);
                        plr1->diplomacy_demand_rejection_message[p2] = (int8_t)eval_messages[p2];
                        plr1->war_buildup_duration = (int8_t)random::Random_(20) + 20;
                        plr1->diplomacy_severity[p2] = 10000;
                        plr1->diplomacy_incident[p2] = 0;
                        plr1->diplomacy_message[p2] = 0x6a;
                        Set_Demands_(p1, p2);
                    } else if (eval_results[p2] == 3) {
                        Clear_Last_Proposal_Turn_(p1, p2);
                        plr1->diplomacy_severity[p2] = 10000;
                        plr1->diplomacy_incident[p2] = 0;
                        plr1->diplomacy_message[p2] = 0x69;
                        plr1->war_buildup_duration = (int8_t)random::Random_(20) + 10;
                        Set_Demands_(p1, p2);
                    } else if (eval_results[p2] == 4) {
                        plr1->council_threat = (int8_t)p2;
                        Clear_Last_Proposal_Turn_(p1, p2);
                        plr1->diplomacy_severity[p2] = 10000;
                        plr1->diplomacy_incident[p2] = 0;
                        plr1->diplomacy_message[p2] = 0x7c;
                        plr1->war_buildup_duration = (int8_t)random::Random_(20) + 20;
                        Set_Demands_(p1, p2);
                    }
                }
            } else if (plr1->war_buildup_duration == 0) {
                plr1->sneak_attack_planet = -1;
            }
        }
    }

    void __cdecl Diplomacy_Broken_Contacts_() {
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            for (int16_t j = 0; j < (int16_t)MOX::_NUM_PLAYERS; ++j) {
                // If there is no contact between player i and player j
                if (MOX::_player[i].contact[j] == 0) {
                    // Reset economic and research treaties/modifiers
                    MOX::_player[i].trade_modifier[j] = 0;
                    MOX::_player[i].trade_treaty[j] = 0;
                    MOX::_player[i].research_treaty[j] = 0;
                }
            }
        }
    }

    void Init_Diplomatic_Relations_() {
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            for (int16_t j = 0; j < (int16_t)MOX::_NUM_PLAYERS; ++j) {
                int32_t race_i = static_cast<uint8_t>(MOX::_player[i].race);
                int32_t race_j = static_cast<uint8_t>(MOX::_player[j].race);
                
                int16_t relation_val = _starting_relations[race_i * 14 + race_j];
                MOX::_player[i].relations[j] = static_cast<int8_t>(relation_val);
                MOX::_player[i].base_relations[j] = MOX::_player[i].relations[j];
                
                MOX::_player[i].treaty[j] = 0;
                MOX::_player[i].trade_treaty[j] = 0;
                MOX::_player[i].research_treaty[j] = 0;
                MOX::_player[i].first_contact[j] = 0;
                MOX::_player[i].diplomacy_severity[j] = 0;
                MOX::_player[i].diplomacy_incident[j] = 0;
                MOX::_player[i].diplomacy_value[j] = 0;
                MOX::_player[i].diplomacy_system[j] = 0;
                MOX::_player[i].treaty_modifier[j] = 0;
                MOX::_player[i].trade_modifier[j] = 0;
                MOX::_player[i].tech_exchange_modifier[j] = 0;
                MOX::_player[i].peace_modifier[j] = 0;
                MOX::_player[i].last_bad_diplomatic_incident[j] = 0;
                MOX::_player[i].broken_treaties_modifier[j] = 0;
                MOX::_player[i].threats[j] = 0;
                MOX::_player[i].trust_worthiness[j] = 0;
                MOX::_player[i].trust_breaker_player[j] = -1;
                MOX::_player[i].last_diplomacy_threat_turn[j] = 0;
                MOX::_player[i].reward_amount[j] = 0;
                MOX::_player[i].reward_tech[j] = 0;
                MOX::_player[i].reward_attack_player[j] = -1;
                MOX::_player[i].peace_duration[j] = 0;
                MOX::_player[i].tribute_treaty[j] = 0;
                MOX::_player[i].ship_war_losses[j] = 0;
                MOX::_player[i].colony_war_losses[j] = 0;
                MOX::_player[i].biological_weapons_flag[j] = 0;
                MOX::_player[i].time_since_last_attack[j] = 0;
                MOX::_player[i].dishonored_flag[j] = 0;
                MOX::_player[i].last_turn_treaty[j] = 0;
                MOX::_player[i].diplomacy_message[j] = 0;
                MOX::_player[i].diplomacy_proposal_request[j] = 0;
                MOX::_player[i].diplomacy_proposal_tribute_bribe[j] = 0;
                MOX::_player[i].diplomacy_proposal_tech_bribe1[j] = 0;
                MOX::_player[i].diplomacy_proposal_tech_bribe2[j] = 0;
                MOX::_player[i].diplomacy_proposal_gold_bribe[j] = 0;
                MOX::_player[i].diplomacy_proposal_war_player[j] = 0;
                MOX::_player[i].opportunity_planet_num[j] = -1;
                MOX::_player[i].delayed_diplomacy_orders[j] = -1;
                
                npcdiplo::Clear_Last_Proposal_Turn_(i, j);
            }
            MOX::_player[i].war_buildup_duration = 0;
            MOX::_player[i].council_voted_for = -1;
            MOX::_player[i].council_threat = -1;
            MOX::_player[i].sneak_attack_planet = -1;
            MOX::_player[i].sneak_attack_message = 0;
            MOX::_player[i].time_since_last_lucky_event = 0;
        }

        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            if (MOX::_player[i].objectives != 100) {
                for (int16_t j = i + 1; j < (int16_t)MOX::_NUM_PLAYERS; ++j) {
                    if (MOX::_player[j].objectives == 100) {
                        int16_t rejection_mod = 0;
                        switch (MOX::_player[i].personality) {
                            case 0: rejection_mod = -3; break;
                            case 1: rejection_mod = -2; break;
                            case 2:
                            case 3: rejection_mod = 0; break;
                            case 4: rejection_mod = 2; break;
                            case 5: rejection_mod = 5; break;
                        }
                        MOX::_player[i].diplomacy_proposal_rejection[j] = rejection_mod;
                        MOX::_player[j].diplomacy_proposal_rejection[i] = rejection_mod;
                    }
                }
            }
        }

        // Initialize Galactic Council global flags/state.
        diplodef::_COUNCIL_FLAG = 0;
        diplodef::_council_winner = -1;
        diplodef::_next_council_meeting_turn = 25;
    }

    void NoOp2() {
        return;
    }

    void Clear_Diplomacy_Messages_() {
        NoOp2();
        Modifier_Diplomacy_Adjustments_();

        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            for (int16_t j = 0; j < (int16_t)MOX::_NUM_PLAYERS; ++j) {
                s_player* p = &MOX::_player[i];
                p->last_turn_diplomacy_severity[j] = p->diplomacy_severity[j];
                p->diplomacy_severity[j] = 0;
                p->diplomacy_incident[j] = 0;
                p->diplomacy_proposal_request[j] = 0;
                p->diplomacy_proposal_tribute_bribe[j] = 0;
                p->diplomacy_proposal_tech_bribe1[j] = 0;
                p->diplomacy_proposal_tech_bribe2[j] = 0;
                p->diplomacy_proposal_gold_bribe[j] = 0;
                p->diplomacy_message[j] = 0;
            }
        }

        // Handle peace duration expiration and treaty revocation
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            for (int16_t j = i + 1; j < (int16_t)MOX::_NUM_PLAYERS; ++j) {
                if (MOX::_player[i].peace_duration[j] > 0) {
                    MOX::_player[i].peace_duration[j]--;
                }
                if (MOX::_player[j].peace_duration[i] > 0) {
                    MOX::_player[j].peace_duration[i]--;
                }

                // If peace duration hits 0 and the treaty was a Peace Treaty (0x03), break it
                if (MOX::_player[i].peace_duration[j] == 0 && MOX::_player[i].treaty[j] == 3 /* TREATY_PEACE */) {
                    MOX::_player[i].treaty[j] = 0;
                    MOX::_player[j].treaty[i] = 0;
                }
            }
        }

        // Increment time since last attack for players at war
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            for (int16_t j = i + 1; j < (int16_t)MOX::_NUM_PLAYERS; ++j) {
                // Treaty >= 4 usually represents a state of war
                if (MOX::_player[i].treaty[j] >= 4 /* TREATY_WAR */) {
                    if (static_cast<uint8_t>(MOX::_player[i].time_since_last_attack[j]) < 250) {
                        MOX::_player[i].time_since_last_attack[j]++;
                    }
                    if (static_cast<uint8_t>(MOX::_player[j].time_since_last_attack[i]) < 250) {
                        MOX::_player[j].time_since_last_attack[i]++;
                    }
                }
            }
        }
    }

    void __cdecl End_Of_Turn_Diplomacy_Adjustments_() {
        Diplomacy_Broken_Contacts_();
        npcdiplo::NPC_Treaty_Hatred_();
        DIP_SCRN::Add_Diplomacy_Turn_Summary_Messages_();

        // Store current treaties as last turn's state
        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            for (int j = 0; j < (int)MOX::_NUM_PLAYERS; ++j) {
                MOX::_player[i].last_turn_treaty[j] = MOX::_player[i].treaty[j];
            }
        }

        // Adjust food shortage duration
        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            if (MOX::_player[i].surplus_food < 0) {
                MOX::_player[i].food_shortage_duration++;
            } else {
                MOX::_player[i].food_shortage_duration = 0;
            }
        }

        // Decay diplomatic modifiers towards zero
        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            for (int j = 0; j < (int)MOX::_NUM_PLAYERS; ++j) {
                MOX::_player[i].treaty_modifier[j] += 10;
                if (MOX::_player[i].treaty_modifier[j] > 0) {
                    MOX::_player[i].treaty_modifier[j] = 0;
                }

                MOX::_player[i].trade_modifier[j] += 10;
                if (MOX::_player[i].trade_modifier[j] > 0) {
                    MOX::_player[i].trade_modifier[j] = 0;
                }

                MOX::_player[i].tech_exchange_modifier[j] += 10;
                if (MOX::_player[i].tech_exchange_modifier[j] > 0) {
                    MOX::_player[i].tech_exchange_modifier[j] = 0;
                }

                MOX::_player[i].peace_modifier[j] += 10;
                if (MOX::_player[i].peace_modifier[j] > 0) {
                    MOX::_player[i].peace_modifier[j] = 0;
                }
            }
        }

        // Decay war buildup duration
        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            if (MOX::_player[i].war_buildup_duration != 0) {
                MOX::_player[i].war_buildup_duration--;
            }
        }

        // Update diplomacy counters for living players regarding dead players they have contact with
        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            for (int j = 0; j < (int)MOX::_NUM_PLAYERS; ++j) {
                if (MOX::_player[i].objectives != 100 /* 'd' */ && MOX::_player[j].objectives == 100 /* 'd' */) {
                    if (MOX::_player[i].contact[j] != 0) {
                        if (MOX::_player[i].last_diplomacy_proposal_turn[j] < 250) {
                            MOX::_player[i].last_diplomacy_proposal_turn[j]++;
                        }
                        if (MOX::_player[i].last_diplomacy_threat_turn[j] < 250) {
                            MOX::_player[i].last_diplomacy_threat_turn[j]++;
                        }
                    }
                }
            }
        }

        // Terminate specific peace treaties (Type 3) under special conditions
        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            for (int j = 0; j < (int)MOX::_NUM_PLAYERS; ++j) {
                if (MOX::_player[i].treaty[j] == 3) {
                    // BUG: specific debug check from original binary comparison that seems broken/invalid
                    if (&MOX::_player[i] == reinterpret_cast<s_player*>(std::uintptr_t{0xFFFFF8D1})) {
                        MOX::_player[i].treaty[j] = 0;
                        MOX::_player[j].treaty[i] = 0;
                        MSG_::Add_Peace_Ended_Msg_(i, j);
                    }
                }
            }
        }
    }

    int16_t __cdecl Get_Player_Diplomacy_Personality_(int16_t player_idx, int16_t target_idx) {
        // If personality is 4 (Honorable) and player has dishonored a treaty with the target
        if (MOX::_player[player_idx].personality == 4 &&
            MOX::_player[player_idx].dishonored_flag[target_idx] == 1) {
            return 6; /* PERSONALITY_DISHONORED_HONORABLE */
        }

        // Return the actual personality byte, zero-extended to short
        return (int16_t)(uint8_t)MOX::_player[player_idx].personality;
    }

    /**
     * @brief Calculates a weighted count of players at war with the specified player.
     * 
     * Iterates through all players and counts those who are at war with player_idx,
     * excluding both player_idx and exclude_idx. Players with objective 100 (often 
     * associated with extermination goals) are weighted double.
     * 
     * @param player_idx Index of the player whose war status is being checked.
     * @param exclude_idx Index of a second player to exclude from the count.
     * @return The weighted war count.
     */
    int16_t __cdecl Get_War_Count_(int16_t player_idx, int16_t exclude_idx) {
        int16_t war_count = 0;

        if ((int16_t)MOX::_NUM_PLAYERS > 0) {
            for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
                // Skip the target player and the specified excluded player
                if (i != player_idx && i != exclude_idx) {
                    // Check diplomatic treaty status. A value of 4 or higher indicates a state of war.
                    if (MOX::_player[player_idx].treaty[i] >= 4) {
                        // Players with specific objectives (100 / 'd') contribute double to the war count
                        if (MOX::_player[i].objectives == 100 /* 'd' */) {
                            war_count += 2;
                        } else {
                            war_count += 1;
                        }
                    }
                }
            }
        }

        return war_count;
    }

    void __cdecl Sneak_Attack_Or_Declare_War_(int16_t attacker_idx, int16_t target_idx, int8_t message_idx) {
        s_player* player_ptr = MOX::_player;
        s_player& attacker = player_ptr[attacker_idx];

        int16_t planet_num = attacker.opportunity_planet_num[target_idx];
        int32_t actual_ratio = attacker.opportunity_actual_attacker_ratio[target_idx];

        if (planet_num != -1 && actual_ratio >= 200) {
            int16_t personality = Get_Player_Diplomacy_Personality_(attacker_idx, target_idx);
            if (personality != 4) {
                attacker.sneak_attack_planet = planet_num;
                attacker.sneak_attack_message = message_idx + 70;
                return;
            }
        }

        int32_t attacker_strength = attacker.total_ship_strength[target_idx];
        int32_t target_strength = player_ptr[target_idx].total_ship_strength[attacker_idx];

        if (((attacker_strength * 4 + 4) * 25) / (target_strength + 1) > 66) {
            Declare_War_(target_idx, attacker_idx);
            attacker.diplomacy_message[target_idx] = message_idx + 56;
        }
    }

    void Determine_First_Contacts_() {
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            // Check if player is human or standard AI ('d' = 100)
            if (MOX::_player[i].objectives == 100 /* 'd' */) {
                for (int16_t j = 0; j < (int16_t)MOX::_NUM_PLAYERS; ++j) {
                    if (MOX::_player[i].contact[j] == 1) {
                        if (MOX::_player[i].first_contact[j] == 0) {
                            MOX::_player[i].first_contact[j] = 1;
                            MOX::_player[i].treaty[j] = 0;
                            MOX::_player[j].treaty[i] = 0;

                            int16_t personality_idx_i = Get_Player_Diplomacy_Personality_(i, j);
                            MOX::_player[i].peace_duration[j] += (int8_t)(_personality_peace_duration[personality_idx_i] * 2);

                            int16_t personality_idx_j = Get_Player_Diplomacy_Personality_(j, i);
                            MOX::_player[j].peace_duration[i] += (int8_t)(_personality_peace_duration[personality_idx_j] * 2);

                            if (MOX::_player[i].objectives == 100 && MOX::_player[j].objectives == 100) {
                                if (diplodef::_COUNCIL_FLAG != 3) {
                                    MOX::_player[j].diplomacy_message[i] = MOX::_player[j].personality + 1;
                                }
                            } else {
                                Change_Relations_(-10000, i, j, 17 /* INCIDENT_FIRST_CONTACT */, 0, 0);
                            }
                        } else if ((MOX::_player[i].just_established_contact >> j) & 1) {
                            if (MOX::_player[i].objectives == 100) {
                                if (MOX::_player[j].objectives == 100) {
                                    if (diplodef::_COUNCIL_FLAG != 3) {
                                        if (MOX::_player[j].relations[i] < 0 && MOX::_player[j].treaty[i] < 4) {
                                            MOX::_player[j].diplomacy_message[i] = MOX::_player[j].personality + 7;
                                        } else {
                                            MOX::_player[j].diplomacy_message[i] = MOX::_player[j].personality + 13;
                                        }
                                    }
                                } else {
                                    Change_Relations_(-10000, i, j, 18 /* INCIDENT_ESTABLISHED_CONTACT */, 0, 0);
                                }
                            } else {
                                Change_Relations_(-10000, i, j, 18 /* INCIDENT_ESTABLISHED_CONTACT */, 0, 0);
                            }
                        }
                    }
                }
            }
        }
    }

    void Determine_Diplomacy_Messages_() {
        Limit_Treaty_Modifiers_();

        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            for (int16_t j = 0; j < (int16_t)MOX::_NUM_PLAYERS; ++j) {
                auto& player_i = MOX::_player[i];
                auto& player_j = MOX::_player[j];

                if (player_i.contact[j] == 1 &&
                    player_i.diplomacy_message[j] == 0 &&
                    player_i.objectives != 100 /* OBJ_SURRENDER_WON */ &&
                    player_j.objectives == 100 &&
                    player_j.eliminated == 0 &&
                    player_i.eliminated == 0 &&
                    (player_i.treaty[j] != 6 /* TREATY_SURRENDER */ || player_i.last_turn_treaty[j] != 6)) {

                    uint8_t incident = player_i.diplomacy_incident[j];

                    if (diplodef::_COUNCIL_FLAG == 3 /* COUNCIL_FINAL_VOTE */) {
                        if (incident >= 1 && incident <= 11) {
                            int severity_abs = abs(player_i.diplomacy_severity[j] / 10);
                            if (severity_abs >= random::Random_(100)) {
                                player_i.diplomacy_message[j] = '{';
                            }
                        }
                    } else {
                        switch (incident) {
                        case 1: case 2: case 3: case 4: case 5:
                        case 6: case 7: case 8: case 9:
                            Determine_Bad_Message_(i, j);
                            break;

                        case 10:
                        case 11:
                            if (player_i.treaty[j] < 4 /* TREATY_WAR */) {
                                int16_t personality = Get_Player_Diplomacy_Personality_(i, j);
                                if (personality == 4 && 
                                   (player_i.treaty[j] == 1 || player_i.treaty[j] == 2 || 
                                    player_i.trade_treaty[j] != 0 || player_i.research_treaty[j] != 0)) {
                                    player_i.dishonored_flag[j] = 1;
                                    player_j.dishonored_flag[i] = 1;
                                    player_i.diplomacy_message[j] = 'B';
                                } else {
                                    player_i.diplomacy_message[j] = (incident == 10) ? 'P' : 'Q';
                                }
                                Declare_War_(j, i);
                            }
                            break;

                        case 12: case 13: case 14: case 15: case 16:
                            Determine_Good_Message_(i, j, incident);
                            break;

                        case 17:
                            player_i.diplomacy_message[j] = player_i.personality + 1;
                            break;

                        case 18:
                            if (player_i.relations[j] < 0 && player_i.treaty[j] < 4) {
                                player_i.diplomacy_message[j] = player_i.personality + 7;
                            } else {
                                player_i.diplomacy_message[j] = player_i.personality + 13;
                            }
                            break;

                        case 19: player_i.diplomacy_message[j] = 'F'; break;
                        case 20: player_i.diplomacy_message[j] = 'l'; break;
                        case 21: player_i.diplomacy_message[j] = 'R'; break;
                        case 22: player_i.diplomacy_message[j] = 'P'; break;
                        case 23: player_i.diplomacy_message[j] = 'o'; break;
                        default: break;
                        }

                        if (player_i.diplomacy_message[j] == 0 && player_i.treaty[j] >= 4) {
                            Peace_Proposal_(i, j);
                        }
                        if (player_i.diplomacy_message[j] == 0) {
                            Required_Alliances_(i, j);
                        }
                        if (player_i.diplomacy_message[j] == 0) {
                            NPC_Required_Alliances_(i, j);
                        }
                        if (player_i.diplomacy_message[j] == 0) {
                            Unprovoked_Good_Messages_(i, j);
                        }
                    }

                    uint8_t msg = player_i.diplomacy_message[j];
                    // Ranges check for Repulsive trait canceling certain positive messages
                    if (((msg >= 0x13 && msg <= 0x38) || (msg >= 0x56 && msg <= 0x6B) || (msg == 0x7C || msg == 0x7D)) &&
                        (player_i.traits[0] == 1 /* TRAIT_REPULSIVE */ || player_j.traits[0] == 1)) {
                        player_i.diplomacy_message[j] = 0;
                        player_j.diplomacy_message[i] = 0;
                    }
                }
            }
        }
    }

    void __cdecl Start_Tribute_Treaty_(int16_t player_idx, int16_t target_idx, int16_t amount) {
        MOX::_player[player_idx].tribute_treaty[target_idx] = amount;

        // Clear existing threats between both players
        MOX::_player[player_idx].threats[target_idx] = 0;
        MOX::_player[target_idx].threats[player_idx] = 0;

        // Update peace duration for player_idx based on personality
        int16_t personality1 = Get_Player_Diplomacy_Personality_(player_idx, target_idx);
        MOX::_player[player_idx].peace_duration[target_idx] += (int8_t)(_personality_peace_duration[personality1] * 2);

        // Update peace duration for target_idx based on personality
        int16_t personality2 = Get_Player_Diplomacy_Personality_(target_idx, player_idx);
        MOX::_player[target_idx].peace_duration[player_idx] += (int8_t)(_personality_peace_duration[personality2] * 2);
    }

    void __cdecl Start_Treaty_(int16_t p1, int16_t p2, int16_t treaty_type) {
        MOX::_player[p1].treaty[p2] = (int8_t)treaty_type;
        MOX::_player[p2].treaty[p1] = (int8_t)treaty_type;

        MOX::_player[p1].threats[p2] = 0;
        MOX::_player[p2].threats[p1] = 0;

        int16_t pers1 = Get_Player_Diplomacy_Personality_(p1, p2);
        int16_t pers2 = Get_Player_Diplomacy_Personality_(p2, p1);

        if (treaty_type == 1 /* TREATY_PEACE */) {
            int16_t dur1 = _personality_peace_duration[pers1];
            MOX::_player[p1].peace_duration[p2] += (int8_t)(dur1 + dur1 / 2);

            int16_t dur2 = _personality_peace_duration[pers2];
            MOX::_player[p2].peace_duration[p1] += (int8_t)(dur2 + dur2 / 2);
            return;
        }

        if (treaty_type == 2 /* TREATY_ALLIANCE */) {
            int16_t dur1 = _personality_peace_duration[pers1];
            MOX::_player[p1].peace_duration[p2] += (int8_t)(dur1 * 2);

            int16_t dur2 = _personality_peace_duration[pers2];
            MOX::_player[p2].peace_duration[p1] += (int8_t)(dur2 * 2);
        }
    }

    void __cdecl Declare_War_(int16_t player_idx, int16_t target_idx) {
        s_player* p1 = &MOX::_player[player_idx];
        s_player* p2 = &MOX::_player[target_idx];

        if (p1->objectives == 'd' && p1->first_contact[target_idx] == 0) {
            p1->first_contact[target_idx] = 1;
        }

        if (p2->objectives == 'd' && p2->first_contact[player_idx] == 0) {
            p2->first_contact[player_idx] = 1;
        }

        if (player_idx < MOX::_NUM_PLAYERS && target_idx < MOX::_NUM_PLAYERS && p1->treaty[target_idx] < 4) {
            p1->threats[target_idx] = 0;
            p2->threats[player_idx] = 0;

            p1->ship_war_losses[target_idx] = 0;
            p2->ship_war_losses[player_idx] = 0;

            p1->colony_war_losses[target_idx] = 0;
            p2->colony_war_losses[player_idx] = 0;

            p1->biological_weapons_flag[target_idx] = 0;
            p2->biological_weapons_flag[player_idx] = 0;

            Break_Trade_(player_idx, target_idx);
            Break_Research_(player_idx, target_idx);
            Break_Tribute_(player_idx, target_idx);
            Break_Treaties_(player_idx, target_idx);

            p1->base_relations[target_idx] -= 10;
            p2->base_relations[player_idx] -= 10;

            int8_t rand_offset = static_cast<int8_t>(random::Random_(25));
            int8_t initial_relations = static_cast<int8_t>(181) - rand_offset;

            p1->relations[target_idx] = initial_relations;
            p2->relations[player_idx] = initial_relations;

            int8_t war_treaty_type;
            if (p1->objectives == 'd' || p2->objectives == 'd') {
                war_treaty_type = (MOX::_settings.game_difficulty < 3) ? 5 : 6;
            } else {
                war_treaty_type = 4;
            }

            p1->treaty[target_idx] = war_treaty_type;
            p2->treaty[player_idx] = war_treaty_type;

            p1->treaty_modifier[target_idx] = -200;
            p2->treaty_modifier[player_idx] = -200;
            p1->trade_modifier[target_idx] = -200;
            p2->trade_modifier[player_idx] = -200;
            p1->tech_exchange_modifier[target_idx] = -200;
            p2->tech_exchange_modifier[player_idx] = -200;
            p1->peace_modifier[target_idx] = -130;
            p2->peace_modifier[player_idx] = -130;

            p1->reward_attack_player[target_idx] = -1;
            p2->reward_attack_player[player_idx] = -1;

            p1->time_since_last_attack[target_idx] = 0;
            p2->time_since_last_attack[player_idx] = 0;

            p1->peace_duration[target_idx] = 0;
            p2->peace_duration[player_idx] = 0;
        }
    }

    int __cdecl Fleet_Strength_Comparison_(int16_t player_idx_1, int16_t player_idx_2) {
        uint32_t strength1_v_2 = MOX::_player[player_idx_1].total_ship_strength[player_idx_2];
        uint32_t strength2_v_1 = MOX::_player[player_idx_2].total_ship_strength[player_idx_1];

        // Result calculation: ((Strength(p1->p2) * 4 + 4) * 25) / (Strength(p2->p1) + 1)
        uint32_t result = ((strength1_v_2 * 4 + 4) * 25) / (strength2_v_1 + 1);

        if (result > 800) {
            result = 800;
        }

        // Halve the result for each third-party player that player_idx_1 is allied with.
        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            if (i != player_idx_1 && i != player_idx_2) {
                if (MOX::_player[player_idx_1].treaty[i] >= 4 /* TREATY_ALLIANCE */) {
                    result >>= 1;
                }
            }
        }

        return (int)result;
    }

    /**
     * Compares the total research strength between two players and returns a relative score.
     * Calculated as: (100 * player1_research) / (player2_research + 1) + 1
     */
    int __cdecl Tech_Strength_Comparison_(int16_t player_idx1, int16_t player_idx2) {
        // Load research values and perform scaling.
        // The assembly uses MOVSX to sign-extend the 16-bit research value to 32-bit.
        uint32_t scaled_research1 = static_cast<uint32_t>(static_cast<int32_t>(MOX::_player[player_idx1].total_research) * 100);
        uint32_t divisor = static_cast<uint32_t>(static_cast<int32_t>(MOX::_player[player_idx2].total_research) + 1);

        // DIV instruction in assembly performs unsigned division.
        return static_cast<int>(scaled_research1 / divisor) + 1;
    }

    /**
     * @brief Compares the total maintenance of two players.
     *
     * Calculates a ratio based on the total maintenance of player_idx_1 and player_idx_2.
     * The formula used is: (maint1 * 100) / (maint2 + 1) + 1.
     * All calculations are performed using unsigned 32-bit arithmetic as per the
     * x86 DIV instruction behavior.
     */
    int __cdecl Maintenance_Strength_Comparison_(int16_t player_idx_1, int16_t player_idx_2) {
        // total_maintenance is a signed int at offset 0xB4 of s_player
        uint32_t maint_1 = static_cast<uint32_t>(MOX::_player[player_idx_1].total_maintenance);
        uint32_t maint_2 = static_cast<uint32_t>(MOX::_player[player_idx_2].total_maintenance);

        // Division by zero is avoided by the +1 in the denominator.
        // The return value is the quotient + 1.
        return static_cast<int>((maint_1 * 100) / (maint_2 + 1) + 1);
    }

    void __cdecl Reward_Check_(int16_t param_1, int16_t param_2, int16_t param_3, int16_t param_4) {
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            if (param_2 == i || i == param_3) {
                continue;
            }

            s_player* plr = &MOX::_player[i];

            if (plr->reward_attack_player[param_2] == param_3) {
                Change_Relations_(200, param_2, i, 15, param_4, param_3);
            }
            else {
                if (plr->relations[param_3] <= -50 && 
                    plr->contact[param_2] == 1 && 
                    plr->contact[param_3] == 1 &&
                    plr->reward_attack_player[param_2] == -1) {
                    
                    if (plr->ship_war_losses[i] > plr->ship_war_losses[i]) { /* Dead code branch */
                        int16_t chance = diplodef::_personality_reward_proposal_chance[(uint8_t)plr->personality];
                        if (chance > random::Random_(100)) {
                            int16_t random_amount = (int16_t)random::Random_(15) + 15;
                            Change_Relations_(random_amount, param_2, i, 15, param_4, param_3);
                        }
                    }
                }
            }
        }
    }

    void __cdecl Change_Relations_(int16_t amount, int16_t p_a, int16_t p_b, int16_t incident, int16_t system, int16_t value) {
        if (p_a > 7 || p_b > 7 || p_a > (int16_t)MOX::_NUM_PLAYERS || p_b > (int16_t)MOX::_NUM_PLAYERS || p_a == p_b || amount == 0) {
            return;
        }

        if (MOX::_player[p_b].objectives == 100 /* 'd' (Human Player) */) {
            return;
        }

        if (MOX::_player[p_a].eliminated || MOX::_player[p_b].eliminated) {
            return;
        }

        if (MOX::_player[p_a].contact[p_b] == 0 || MOX::_player[p_b].contact[p_a] == 0) {
            return;
        }

        if (MOX::_player[p_a].treaty[p_b] == 6 /* War */) {
            return;
        }

        if (incident == 10) {
            Reward_Check_(amount, p_a, p_b, system);
        }

        // Special mode: Force a severe diplomatic incident
        if (amount == -10000) {
            if (MOX::_player[p_a].objectives == 100 /* 'd' */ && incident > 0) {
                MOX::_player[p_b].diplomacy_severity[p_a] = 10000;
                MOX::_player[p_b].diplomacy_incident[p_a] = (char)incident;
                MOX::_player[p_b].diplomacy_value[p_a] = value;
                MOX::_player[p_b].diplomacy_system[p_a] = system;
            }
            return;
        }

        int8_t current_rel = MOX::_player[p_b].relations[p_a];
        int16_t scaled_amount;

        // Relation change scaling based on current relations
        if (amount > 0) {
            if (current_rel < 0) {
                scaled_amount = amount * 2;
                if (scaled_amount + current_rel > 10) {
                    scaled_amount = 10 - current_rel;
                }
            } else {
                scaled_amount = amount / (current_rel / 25 + 1);
            }
        } else {
            if (current_rel > 0) {
                scaled_amount = amount * 2;
            } else {
                scaled_amount = amount / (1 - current_rel / 25);
            }
        }

        // Personality modifiers
        if (MOX::_player[p_b].personality == 4 /* Ruthless/Aggressive */ && scaled_amount < 0) {
            scaled_amount *= 2;
        }

        if (MOX::_player[p_b].personality == 0 /* Pacifist */) {
            if (scaled_amount < 0) {
                scaled_amount = (scaled_amount * 3) / 2;
            } else if (scaled_amount > 0) {
                scaled_amount = (scaled_amount * 3) / 4;
            }
        }

        // Trait modifier: Diplomatic (trait 20)
        if (MOX::_player[p_a].traits[20] == 1) {
            if (scaled_amount < 1) {
                scaled_amount /= 2;
            } else {
                scaled_amount *= 2;
            }
        }

        int8_t treaty = MOX::_player[p_b].treaty[p_a];
        int16_t final_change = scaled_amount;

        // Alliance (4, 5) logic
        if ((treaty == 4 || treaty == 5) && incident >= 1 && incident <= 9 && final_change < 0) {
            if (MOX::_player[p_a].objectives == 100 /* 'd' */) {
                int16_t peace_adj = final_change / 4;
                MOX::_player[p_b].peace_modifier[p_a] += peace_adj;
                MOX::_player[p_a].peace_modifier[p_b] -= peace_adj;
                return;
            }
            final_change = 0;
        }

        // Main relation update (only for non-alliance/war states)
        if (treaty < 4) {
            if (MOX::_player[p_a].objectives != 100) {
                // If late game (stardate > 35100) and both players are AI
                if (MOX::_player[p_b].objectives != 100 && (MOX::_stardate > 35100)) {
                    if (final_change > 0) {
                        final_change *= 2;
                    } else if (final_change < 0) {
                        final_change /= (MOX::_settings.game_difficulty / 2 + 1);
                    }
                }
            }

            if (final_change != 0) {
                int new_rel = MOX::_player[p_b].relations[p_a] + (int8_t)final_change;
                if (new_rel < -100) new_rel = -100;
                if (new_rel > 100) new_rel = 100;
                MOX::_player[p_b].relations[p_a] = (int8_t)new_rel;

                // Sync modifiers symmetrically
                MOX::_player[p_b].treaty_modifier[p_a] += final_change;
                MOX::_player[p_b].trade_modifier[p_a] += final_change;
                MOX::_player[p_b].tech_exchange_modifier[p_a] += final_change;

                MOX::_player[p_a].treaty_modifier[p_b] = MOX::_player[p_b].treaty_modifier[p_a];
                MOX::_player[p_a].trade_modifier[p_b] = MOX::_player[p_b].trade_modifier[p_a];
                MOX::_player[p_a].tech_exchange_modifier[p_b] = MOX::_player[p_b].tech_exchange_modifier[p_a];

                // Update peace modifier
                if (scaled_amount < 0) {
                    int16_t peace_adj = scaled_amount / 4;
                    MOX::_player[p_a].peace_modifier[p_b] -= peace_adj;
                    MOX::_player[p_b].peace_modifier[p_a] += peace_adj;
                } else {
                    MOX::_player[p_b].peace_modifier[p_a] += scaled_amount;
                }

                // Check if this incident is more significant than the current stored incident
                int current_sev_abs = abs((int)MOX::_player[p_b].diplomacy_severity[p_a]);
                int final_chg_abs = abs((int)final_change);
                if (current_sev_abs < final_chg_abs) {
                    MOX::_player[p_b].diplomacy_severity[p_a] = final_change;
                    MOX::_player[p_b].diplomacy_incident[p_a] = (char)incident;
                    MOX::_player[p_b].diplomacy_value[p_a] = value;
                    MOX::_player[p_b].diplomacy_system[p_a] = system;
                }
            }

            // Cap relations based on treaty status
            if (MOX::_player[p_b].treaty[p_a] >= 4 && MOX::_player[p_b].relations[p_a] > -25) {
                MOX::_player[p_b].relations[p_a] = -25;
            }
            if (MOX::_player[p_b].treaty[p_a] != 2 /* Non-Peace treaty state */ && MOX::_player[p_b].relations[p_a] > 65) {
                MOX::_player[p_b].relations[p_a] = 65;
            }

            // Sync relations symmetrically
            MOX::_player[p_a].relations[p_b] = MOX::_player[p_b].relations[p_a];
        }
    }

    void __cdecl Break_Treaties_(int16_t p1_idx, int16_t p2_idx) {
        if (p2_idx >= (int16_t)MOX::_NUM_PLAYERS || p1_idx >= (int16_t)MOX::_NUM_PLAYERS) {
            return;
        }

        s_player* p1 = &MOX::_player[p1_idx];
        s_player* p2 = &MOX::_player[p2_idx];

        if (p1->treaty[p2_idx] == 0) {
            return;
        }

        if (p2->treaty[p1_idx] >= 4) {
            return;
        }

        uint8_t current_treaty = p1->treaty[p2_idx];

        p2->last_broken_treaty[p1_idx] = p2->treaty[p1_idx];
        p2->threats[p1_idx]++;
        p2->dishonored_flag[p1_idx] = 1;
        p1->peace_duration[p2_idx] = 0;
        p2->peace_duration[p1_idx] = 0;

        int16_t penalty = 0;
        if (current_treaty == 1) {
            penalty = -20;
        } else if (current_treaty == 2 || current_treaty == 3) {
            penalty = -40;
        }

        if (p1->personality == 4 /* PERSONALITY_AGGRESSIVE */) {
            penalty *= 3;
        }

        p1->broken_treaties_modifier[p2_idx] -= (char)penalty;

        for (int16_t other_idx = 0; other_idx < (int16_t)MOX::_NUM_PLAYERS; ++other_idx) {
            if (other_idx == p1_idx || other_idx == p2_idx) {
                continue;
            }

            s_player* other = &MOX::_player[other_idx];
            if (other->eliminated == 0 && (p1->contact[other_idx] == 1 || p2->contact[other_idx] == 1)) {
                if (other->personality == 4) {
                    other->trust_worthiness[p1_idx] -= 20;
                } else {
                    other->trust_worthiness[p1_idx] -= 10;
                }
                other->trust_breaker_player[p1_idx] = (char)p2_idx;
                other->trust_breaker_treaty[p1_idx] = current_treaty;
            }
        }

        if (p2->personality == 4) {
            p2->trust_worthiness[p1_idx] -= 20;
        } else {
            p2->trust_worthiness[p1_idx] -= 10;
        }
        p2->trust_breaker_player[p1_idx] = (char)p2_idx;
        p2->trust_breaker_treaty[p1_idx] = current_treaty;

        if (current_treaty == 2) {
            int new_base = p1->base_relations[p2_idx] - penalty;
            if (new_base < -100) {
                p1->base_relations[p2_idx] = (char)-100;
            } else {
                p1->base_relations[p2_idx] -= (char)penalty;
            }

            if (p1->base_relations[p2_idx] < -100) {
                p1->base_relations[p2_idx] = (char)-100;
            }
            p2->base_relations[p1_idx] = p1->base_relations[p2_idx];
        }

        if (penalty != 0) {
            p1->last_broken_treaty_type[p2_idx] = current_treaty;
            p2->last_broken_treaty_type[p1_idx] = current_treaty;
            p2->relations[p1_idx] = p1->relations[p2_idx];
        }

        p1->treaty[p2_idx] = 0;
        p2->treaty[p1_idx] = 0;
        p1->reward_attack_player[p2_idx] = 0;
        p2->reward_attack_player[p1_idx] = 0;

        if (p1->relations[p2_idx] < -100) {
            p1->relations[p2_idx] = (char)-100;
        }
        p2->relations[p1_idx] = p1->relations[p2_idx];

        p1->treaty_modifier[p2_idx] = -200;
        p1->trade_modifier[p2_idx] = -200;
        p1->tech_exchange_modifier[p2_idx] = -200;
        p1->peace_modifier[p2_idx] = -200;

        p2->treaty_modifier[p1_idx] = -200;
        p2->trade_modifier[p1_idx] = -200;
        p2->tech_exchange_modifier[p1_idx] = -200;
        p2->peace_modifier[p1_idx] = -200;
    }

    void __cdecl Break_Trade_(int16_t p1_idx, int16_t p2_idx) {
        if (p1_idx >= (int16_t)MOX::_NUM_PLAYERS || p2_idx >= (int16_t)MOX::_NUM_PLAYERS) {
            return;
        }

        s_player* plr1 = &MOX::_player[p1_idx];
        s_player* plr2 = &MOX::_player[p2_idx];

        uint8_t original_treaty = plr1->trade_treaty[p2_idx];
        if (original_treaty == 0) {
            return;
        }

        plr2->last_broken_treaty[p1_idx] = 7;

        if (plr1->relations[p2_idx] < -100) {
            plr1->relations[p2_idx] = -100;
        }

        plr2->relations[p1_idx] = plr1->relations[p2_idx];
        plr1->trade_treaty[p2_idx] = 0;
        plr2->trade_treaty[p1_idx] = 0;
        plr1->reward_attack_player[p2_idx] = 0;
        plr2->reward_attack_player[p1_idx] = 0;

        // Additional penalties if no non-aggression pact exists and it was a trade treaty (type 1)
        if (plr2->treaty[p1_idx] < 4 && original_treaty == 1) {
            if (plr1->personality == 4 /* PER_RUTHLESS */) {
                plr1->broken_treaties_modifier[p2_idx] -= 15;
            } else {
                plr1->broken_treaties_modifier[p2_idx] -= 5;
            }

            int32_t drop = random::Random_(20) + random::Random_(20);
            int32_t new_rel = (int32_t)plr1->relations[p2_idx] - drop;
            if (new_rel < -100) {
                plr1->relations[p2_idx] = -100;
            } else {
                plr1->relations[p2_idx] = (int8_t)new_rel;
            }

            if (plr1->relations[p2_idx] < -100) {
                plr1->relations[p2_idx] = -100;
            }
            plr2->relations[p1_idx] = plr1->relations[p2_idx];

            int32_t base_drop = random::Random_(20);
            int32_t new_base = (int32_t)plr1->base_relations[p2_idx] - base_drop;
            if (new_base < -100) {
                plr1->base_relations[p2_idx] = -100;
            } else {
                plr1->base_relations[p2_idx] = (int8_t)new_base;
            }
            plr2->base_relations[p1_idx] = plr1->base_relations[p2_idx];

            plr1->last_broken_treaty_type[p2_idx] = 7;

            for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
                if (i != p1_idx && i != p2_idx) {
                    MOX::_player[i].broken_treaties_modifier[p1_idx] -= 5;
                }
            }
        }
    }

    void __cdecl Limit_Treaty_Modifiers_() {
        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            for (int j = 0; j < (int)MOX::_NUM_PLAYERS; ++j) {
                if (i == j) {
                    continue;
                }

                if (MOX::_player[i].treaty_modifier[j] > 120) {
                    MOX::_player[i].treaty_modifier[j] = 120;
                }

                if (MOX::_player[i].treaty_modifier[j] < -200) {
                    MOX::_player[i].treaty_modifier[j] = -200;
                }
            }
        }
    }

    /**
     * Establishes peace between two players, resets war-related stats, and adjusts relations.
     * @param p1 First player index
     * @param p2 Second player index
     */
    void __cdecl Determine_Good_Message_(int16_t player_idx, int16_t other_player_idx, int incident) {
        int chance = 3;

        if (incident == 15 /* 0x0F */) {
            chance = 10;
        } else if (incident == 16 /* 0x10 */) {
            chance = 20;
        }

        if (random::Random_(100) > chance) {
            return;
        }

        MOX::_player[player_idx].threats[other_player_idx] = 0;

        switch (incident) {
            case 12: /* 0x0C */
                MOX::_player[player_idx].diplomacy_message[other_player_idx] = 'Y';
                break;

            case 13: /* 0x0D */
                MOX::_player[player_idx].diplomacy_message[other_player_idx] = 'Z';
                break;

            case 14: /* 0x0E */
                MOX::_player[player_idx].diplomacy_message[other_player_idx] = '[';
                break;

            case 15: /* 0x0F */
                if (MOX::_player[player_idx].reward_attack_player[other_player_idx] != -1) {
                    MOX::_player[player_idx].reward_attack_player[other_player_idx] = -1;
                    MOX::_player[player_idx].diplomacy_message[other_player_idx] = ']';
                } else if (random::Random_(100) + MOX::_player[player_idx].relations[other_player_idx] >= 75 /* 0x4B */) {
                    MOX::_player[player_idx].diplomacy_message[other_player_idx] = '^';
                } else {
                    if (random::Random_(3) == 1 &&
                        Get_Demands_(other_player_idx, player_idx, 4, 0, 1, 1, 0) == 1) {
                        MOX::_player[player_idx].reward_amount[other_player_idx] = _diplomacy_proposal_gold_bribe;
                        MOX::_player[player_idx].reward_tech[other_player_idx] = _diplomacy_proposal_tech_bribe1;
                        MOX::_player[player_idx].diplomacy_message[other_player_idx] = ']';
                        Set_Demands_(player_idx, other_player_idx);
                    } else {
                        MOX::_player[player_idx].diplomacy_message[other_player_idx] = '\\';
                    }
                }
                break;

            case 16: /* 0x10 */
                if (random::Random_(4) == 1 &&
                    Get_Demands_(other_player_idx, player_idx, 4, 0, 1, 1, 0) == 1) {
                    MOX::_player[player_idx].diplomacy_message[other_player_idx] = '`';
                    Set_Demands_(player_idx, other_player_idx);
                } else {
                    MOX::_player[player_idx].diplomacy_message[other_player_idx] = '_';
                }
                break;

            default:
                break;
        }
    }

    void __cdecl Determine_Bad_Message_(int16_t current_player, int16_t target_player) {
        s_player* plr = &MOX::_player[current_player];
        int16_t severity = plr->diplomacy_severity[target_player];

        if (severity == 0) return;

        if (plr->treaty[target_player] >= 4) {
            Peace_Proposal_(current_player, target_player);
            return;
        }

        bool p1_repulsive = (MOX::_player[current_player].traits[TRAIT_REPULSIVE] == 1);
        bool p2_repulsive = (MOX::_player[target_player].traits[TRAIT_REPULSIVE] == 1);

        if (!(p1_repulsive || p2_repulsive) || severity == -10000) {
            int16_t personality = Get_Player_Diplomacy_Personality_(current_player, target_player);
            int16_t adj_severity = abs(severity * 2);

            adj_severity -= diplodef::_personality_relation_modifiers[personality];
            adj_severity -= plr->broken_treaties_modifier[target_player];
            adj_severity -= plr->trust_worthiness[target_player];

            uint8_t incident = plr->diplomacy_incident[target_player];
            if (incident == 9 && plr->council_threat == target_player) {
                adj_severity = 100;
                plr->council_threat = -1;
            }

            if (plr->relations[target_player] < 0) {
                adj_severity -= (int16_t)plr->relations[target_player];
            }

            if (random::Random_(100) <= adj_severity) {
                plr->threats[target_player]++;
            }
            if (random::Random_(100) <= adj_severity / 2) {
                plr->threats[target_player]++;
            }
            if (random::Random_(100) <= adj_severity / 4) {
                plr->threats[target_player]++;
            }

            bool has_active_bond = (plr->treaty[target_player] == 1 || plr->treaty[target_player] == 2 ||
                                   plr->trade_treaty[target_player] != 0 || plr->research_treaty[target_player] != 0);

            if (incident >= 1 && incident <= 9) {
                plr->last_bad_diplomatic_incident[target_player] = incident;
            }

            if (plr->threats[target_player] > 0) {
                int16_t p_check = Get_Player_Diplomacy_Personality_(current_player, target_player);
                if (p_check == 4 /* RUTHLESS */) {
                    if (has_active_bond) {
                        plr->dishonored_flag[target_player] = 1;
                        MOX::_player[target_player].dishonored_flag[current_player] = 1;
                        Declare_War_(target_player, current_player);
                        plr->diplomacy_message[target_player] = 0x42 /* 'B' */;
                        return;
                    }
                }
            }

            int8_t threats = plr->threats[target_player];
            if (threats < 3 && (threats < 2 || has_active_bond)) {
                if (threats == 1) {
                    if (has_active_bond) {
                        plr->diplomacy_message[target_player] = incident + 0x26 /* '&' */;
                    } else {
                        plr->diplomacy_message[target_player] = incident + 0x12;
                    }
                    return;
                }

                if (threats == 2 && has_active_bond) {
                    int16_t p_idx = Get_Player_Diplomacy_Personality_(current_player, target_player);
                    int16_t demand_chance = diplodef::_threat_demand_chance[p_idx];
                    if (random::Random_(100) <= demand_chance) {
                        if (Get_Demands_(current_player, target_player, 6, 1, 1, 1, 1) == 1) {
                            plr->diplomacy_message[target_player] = incident + 0x1B;
                            Set_Demands_(current_player, target_player);
                            return;
                        }
                    }
                    Break_Treaties_(target_player, current_player);
                    Break_Research_(target_player, current_player);
                    Break_Trade_(target_player, current_player);
                    plr->diplomacy_message[target_player] = incident + 0x2F /* '/' */;
                    return;
                }
            } else {
                Sneak_Attack_Or_Declare_War_(current_player, target_player, (int)incident);
            }
        }
    }

    void __cdecl Peace_Proposal_(int16_t player_idx, int16_t target_idx) {
        s_player* plr = &MOX::_player[player_idx];
        uint8_t time_since_attack = plr->time_since_last_attack[target_idx];

        if (time_since_attack <= 9) {
            return;
        }

        int16_t base_chance = (int16_t)time_since_attack - 9;

        int32_t p1_attacks = aimove::Number_Of_Attacks_Underway_(player_idx, target_idx);
        int32_t p2_attacks = aimove::Number_Of_Attacks_Underway_(target_idx, player_idx);

        int16_t war_count_p2_p1 = Get_War_Count_(target_idx, player_idx);
        int16_t war_count_p1_p2 = Get_War_Count_(player_idx, target_idx);

        int16_t score = (int16_t)(((p1_attacks * 2 - p2_attacks) * 4 - war_count_p2_p1 + war_count_p1_p2 - 2) * 5);

        int16_t personality_idx = Get_Player_Diplomacy_Personality_(player_idx, target_idx);
        score -= (int16_t)(diplodef::_personality_relation_modifiers[personality_idx] / 5);

        score += Total_War_Loss_Comparison_(player_idx, target_idx) / 10;
        score += (int16_t)(Fleet_Strength_Comparison_(player_idx, target_idx) / 10);

        score += (int16_t)(plr->broken_treaties_modifier[target_idx] / 2);
        score += (int16_t)plr->trust_worthiness[target_idx];

        for (int16_t p = 0; p < (int16_t)MOX::_NUM_PLAYERS; ++p) {
            if (p != player_idx && p != target_idx) {
                score += (int16_t)(aimove::Number_Of_Attacks_Underway_(p, target_idx) * 20);
            }
        }

        int16_t roll_threshold;
        if (score < 0) {
            roll_threshold = -(score * base_chance);
        } else {
            uint32_t root = isqrt_(score + 3);
            roll_threshold = (int16_t)((uint32_t)base_chance / root);
        }

        if (random::Random_(100) > roll_threshold) {
            return;
        }

        if (score < -10) {
            int16_t demand_val = (int16_t)((-10 - score) / 5 + 2);
            if (Get_Demands_(target_idx, player_idx, demand_val, 1, 1, 1, 1) == 1) {
                plr->diplomacy_message[target_idx] = 'U'; /* 0x55 */
                Set_Demands_(player_idx, target_idx);
            } else {
                plr->diplomacy_message[target_idx] = 'S'; /* 0x53 */
            }
        } else if (score > 10) {
            int16_t demand_val = (int16_t)((score - 10) / 5 + 2);
            if (Get_Demands_(player_idx, target_idx, demand_val, 1, 1, 1, 1) == 1) {
                plr->diplomacy_message[target_idx] = 'T'; /* 0x54 */
                Set_Demands_(player_idx, target_idx);
            } else {
                plr->diplomacy_message[target_idx] = 'S'; /* 0x53 */
            }
        } else {
            plr->diplomacy_message[target_idx] = 'S'; /* 0x53 */
        }

        plr->time_since_last_attack[target_idx] = 0;
    }

    void __cdecl Declare_Peace_(int16_t p1, int16_t p2) {
        MOX::_player[p1].treaty[p2] = 3 /* PEACE */;
        MOX::_player[p2].treaty[p1] = 3 /* PEACE */;

        // Improve relations by 50, but cap at 0 (neutral) if previously negative (war)
        MOX::_player[p1].relations[p2] += 50;
        if (MOX::_player[p1].relations[p2] > 0) {
            MOX::_player[p1].relations[p2] = 0;
        }
        MOX::_player[p2].relations[p1] = MOX::_player[p1].relations[p2];

        Get_Player_Diplomacy_Personality_(p1, p2);
        MOX::_player[p1].peace_duration[p2] = 30;

        Get_Player_Diplomacy_Personality_(p2, p1);
        MOX::_player[p2].peace_duration[p1] = 30;

        MOX::_player[p1].threats[p2] = 0;
        MOX::_player[p2].threats[p1] = 0;

        MOX::_player[p1].ship_war_losses[p2] = 0;
        MOX::_player[p2].ship_war_losses[p1] = 0;

        MOX::_player[p1].colony_war_losses[p2] = 0;
        MOX::_player[p2].colony_war_losses[p1] = 0;

        MOX::_player[p1].biological_weapons_flag[p2] = 0;
        MOX::_player[p2].biological_weapons_flag[p1] = 0;
    }

    /**
     * Checks if a player should propose an alliance-related war demand to another player.
     * If player_idx has a peace treaty with other_player_idx, there's a 1 in 10 chance 
     * they will demand other_player_idx go to war with one of player_idx's allies.
     */
    void __cdecl Required_Alliances_(int16_t player_idx, int16_t other_player_idx) {
        // 0x02: Likely TREATY_PEACE or TREATY_CEASEFIRE
        if (MOX::_player[player_idx].treaty[other_player_idx] == 2) {
            if (random::Random_(10) == 1) {
                int16_t target_ally_idx = -1;

                for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
                    // Cannot target the players involved in the primary check
                    if (i == player_idx || i == other_player_idx) {
                        continue;
                    }

                    // Check if player_idx is allied with i (0x04: TREATY_ALLIANCE)
                    if (MOX::_player[i].treaty[player_idx] >= 4) {
                        // If other_player_idx is in contact and range of the ally, 
                        // but is NOT allied with them.
                        if (MOX::_player[other_player_idx].contact[i] == 1 &&
                            MOX::_player[other_player_idx].within_range[i] == 1 &&
                            MOX::_player[other_player_idx].treaty[i] < 4) {
                            
                            target_ally_idx = i;
                        }
                    }
                }

                if (target_ally_idx != -1) {
                    // Set diplomacy message 'V' (likely a war demand code)
                    MOX::_player[player_idx].diplomacy_message[other_player_idx] = 'V';
                    MOX::_player[player_idx].diplomacy_proposal_war_player[other_player_idx] = (int8_t)target_ally_idx;
                }
            }
        }
    }

    void __cdecl Unprovoked_Good_Messages_(int16_t player_idx, int16_t target_idx) {
        s_player& player = MOX::_player[player_idx];
        s_player& target = MOX::_player[target_idx];

        if (player.treaty[target_idx] >= 4) return;

        if (npcdiplo::NPC_Good_Proposal_Check_(player_idx, target_idx) == 0) return;

        int choice = 0;
        for (int i = 0; i < 3; ++i) {
            int roll = random::Random_(20);
            if (roll > 0 && roll <= 19) {
                uint8_t case_idx = _unprovoked_message_mapping[roll - 1];
                switch (case_idx) {
                    case 1: choice = 5; break;
                    case 2: if (player.treaty[target_idx] == 0) choice = 2; break;
                    case 3: if (player.relations[target_idx] >= 0 && player.trade_treaty[target_idx] == 0) choice = 1; break;
                    case 4: if (player.relations[target_idx] >= 0 && player.research_treaty[target_idx] == 0) choice = 4; break;
                    case 5: if (player.treaty[target_idx] == 1) choice = 3; break;
                    case 6:
                        if (player.treaty[target_idx] != 2 && random::Random_(3) == 1 && player.relations[target_idx] >= 0) {
                            choice = 6;
                        }
                        break;
                }
            }
        }

        if (choice == 5) {
            npcdiplo::NPC_Tech_Exchange_Check_(player_idx, target_idx);
            if (player.diplomacy_message[target_idx] != 0) {
                npcdiplo::Clear_Last_Proposal_Turn_(player_idx, target_idx);
            }
            return;
        }

        if (choice == 2) {
            int threshold = (diplodef::_personality_relation_modifiers[(uint8_t)player.personality] + (10 - MOX::_settings.game_difficulty) * 25) * 2;
            if (random::Random_(threshold) >= Fleet_Strength_Comparison_(player_idx, target_idx)) {
                player.diplomacy_message[target_idx] = 'a' /* PROPOSAL_PEACE */;
                npcdiplo::Clear_Last_Proposal_Turn_(player_idx, target_idx);
                return;
            }
        }

        if (choice == 3) {
            int threshold = (diplodef::_personality_relation_modifiers[(uint8_t)player.personality] + (7 - MOX::_settings.game_difficulty) * 25) * 2;
            if (random::Random_(threshold) >= Fleet_Strength_Comparison_(player_idx, target_idx)) {
                player.diplomacy_message[target_idx] = 'b' /* PROPOSAL_ALLIANCE */;
                npcdiplo::Clear_Last_Proposal_Turn_(player_idx, target_idx);
                return;
            }
        }

        if (choice == 4) {
            int threshold = Tech_Strength_Comparison_(player_idx, target_idx) + (MOX::_settings.game_difficulty * 25 - diplodef::_personality_relation_modifiers[(uint8_t)player.personality]) * 2;
            if (player.treaty[target_idx] == 1) threshold -= 25;
            if (player.treaty[target_idx] == 2) threshold -= 50;
            if (random::Random_(300) >= threshold) {
                player.diplomacy_message[target_idx] = 'd' /* PROPOSAL_RESEARCH_TREATY */;
                npcdiplo::Clear_Last_Proposal_Turn_(player_idx, target_idx);
                return;
            }
        }

        if (choice == 1) {
            int threshold = Maintenance_Strength_Comparison_(player_idx, target_idx) + (MOX::_settings.game_difficulty * 25 - diplodef::_personality_relation_modifiers[(uint8_t)player.personality]) * 2;
            if (player.treaty[target_idx] == 1) threshold -= 25;
            if (player.treaty[target_idx] == 2) threshold -= 50;
            
            bool trade_offered = false;
            if (random::Random_(300) >= threshold) {
                player.diplomacy_message[target_idx] = 'c' /* PROPOSAL_TRADE_TREATY */;
                trade_offered = true;
            }

            if (trade_offered) {
                npcdiplo::Clear_Last_Proposal_Turn_(player_idx, target_idx);
                return;
            }

            // If trade treaty roll failed, check for potential common enemy proposal
            int16_t candidates[20];
            int16_t num_candidates = 0;
            for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
                if (i != player_idx && i != target_idx) {
                    if (player.contact[i] == 1 && target.contact[i] == 1 &&
                        target.treaty[i] < 4 && player.relations[i] < -50 &&
                        MOX::_player[i].ship_war_losses[player_idx] < player.ship_war_losses[i] &&
                        player.personality != 0 && player.time_since_last_attack[i] >= 20 &&
                        player.ship_war_losses[i] > 0) 
                    {
                        candidates[num_candidates++] = i;
                    }
                }
            }

            if (num_candidates > 0) {
                int16_t victim_idx = candidates[random::Random_(num_candidates)];
                int32_t ratio = player.ship_war_losses[victim_idx] / (MOX::_player[victim_idx].ship_war_losses[player_idx] + 1);
                if (ratio > 6) ratio = 6;

                if (Get_Demands_(target_idx, player_idx, (int16_t)ratio, 0, 1, 1, 0) == 1) {
                    player.diplomacy_message[target_idx] = '}' /* PROPOSAL_ATTACK_PLAYER */;
                    player.reward_amount[target_idx] = _diplomacy_proposal_gold_bribe;
                    player.reward_tech[target_idx] = _diplomacy_proposal_tech_bribe1;
                    player.reward_attack_player[target_idx] = (int8_t)victim_idx;
                    npcdiplo::Clear_Last_Proposal_Turn_(player_idx, target_idx);
                    Set_Demands_(player_idx, target_idx);
                    return;
                }
            }
        }

        npcdiplo::Clear_Last_Proposal_Turn_(player_idx, target_idx);
    }

    void __cdecl NPC_Required_Alliances_(int16_t p1_idx, int16_t p2_idx) {
        int active_plr = (int)p1_idx;
        int other_plr = (int)p2_idx;

        // Check if there is a Non-Aggression Pact (treaty == 2)
        if (MOX::_player[active_plr].treaty[other_plr] == 2) {
            if (random::Random_(10) == 1) {
                int16_t target_idx = -1;

                for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
                    if (i != p1_idx && i != p2_idx) {
                        // Check if potential target is allied (4 or 5) with other_plr,
                        // and active_plr knows them, can reach them, and is not already allied.
                        if ((MOX::_player[i].treaty[other_plr] == 4 || MOX::_player[i].treaty[other_plr] == 5) &&
                            (MOX::_player[active_plr].contact[i] == 1 &&
                             MOX::_player[active_plr].within_range[i] == 1 &&
                             MOX::_player[active_plr].treaty[i] < 4)) {
                            target_idx = i;
                        }
                    }
                }

                if (target_idx != -1) {
                    // Set diplomacy message to 'F' and the proposed war target
                    MOX::_player[active_plr].diplomacy_message[other_plr] = 0x46 /* 'F' */;
                    MOX::_player[active_plr].diplomacy_proposal_war_player[other_plr] = (int8_t)target_idx;

                    Declare_War_(p1_idx, target_idx);
                }
            }
        }
    }

    /**
     * Updates mutual diplomacy demands between two players using globally stored proposal values.
     */
    void __cdecl Set_Demands_(int16_t p1, int16_t p2) {
        s_player* players = MOX::_player;

        // Set demands for player 1 against player 2
        players[p1].diplomacy_proposal_tribute_bribe[p2] = _diplomacy_proposal_tech_bribe1;
        players[p1].diplomacy_proposal_tech_bribe1[p2] = _diplomacy_proposal_tech_bribe2;
        players[p1].diplomacy_proposal_gold_bribe[p2] = _diplomacy_proposal_gold_bribe;
        players[p1].diplomacy_proposal_system_bribe[p2] = _diplomacy_proposal_system_bribe;

        // Set demands for player 2 against player 1
        players[p2].diplomacy_proposal_tribute_bribe[p1] = _diplomacy_proposal_tech_bribe1;
        players[p2].diplomacy_proposal_tech_bribe1[p1] = _diplomacy_proposal_tech_bribe2;
        players[p2].diplomacy_proposal_gold_bribe[p1] = _diplomacy_proposal_gold_bribe;
        players[p2].diplomacy_proposal_system_bribe[p1] = _diplomacy_proposal_system_bribe;
    }

    void Diplomacy_Growth_() {
        // Phase 1: Basic relation changes based on treaties and random factors
        for (int p1 = 0; p1 < (int)MOX::_NUM_PLAYERS; ++p1) {
            for (int p2 = p1 + 1; p2 < (int)MOX::_NUM_PLAYERS; ++p2) {
                // Objectives 100 is typically used for specific internal logic (e.g., Orion)
                if (MOX::_player[p1].objectives != 100 || MOX::_player[p2].objectives != 100) {

                    if (random::Random_(2) == 1) {
                        if (MOX::_player[p1].relations[p2] < -50 && MOX::_player[p1].peace_duration[p2] > 0) {
                            Change_Relations_(1, p1, p2, 0, 0, 0);
                        }
                    }

                    if (MOX::_player[p1].treaty[p2] == 1 /* Peace */) {
                        if (random::Random_(100) <= (100 - MOX::_player[p1].relations[p2])) {
                            Change_Relations_(random::Random_(3), p1, p2, 0, 0, 0);
                        }
                    }

                    if (MOX::_player[p1].trade_treaty[p2] == 1) {
                        if (random::Random_(100) <= (100 - MOX::_player[p1].relations[p2])) {
                            Change_Relations_(random::Random_(3), p1, p2, 12 /* TRADE_TREATY */, 0, 0);
                        }
                    }

                    if (MOX::_player[p1].research_treaty[p2] == 1) {
                        if (random::Random_(100) <= (100 - MOX::_player[p1].relations[p2])) {
                            Change_Relations_(random::Random_(3), p1, p2, 13 /* RESEARCH_TREATY */, 0, 0);
                        }
                    }

                    if (MOX::_player[p1].treaty[p2] == 2 /* Alliance */) {
                        Change_Relations_(random::Random_(5), p1, p2, 0, 0, 0);
                    }

                    if (MOX::_player[p1].tribute_treaty[p2] == 1) {
                        if (random::Random_(100) <= (100 - MOX::_player[p1].relations[p2])) {
                            Change_Relations_(random::Random_(3), p1, p2, 14 /* TRIBUTE */, 0, 0);
                        }
                    }

                    if (MOX::_player[p1].tribute_treaty[p2] == 2) {
                        if (random::Random_(100) <= (100 - MOX::_player[p1].relations[p2])) {
                            Change_Relations_(random::Random_(8), p1, p2, 14 /* TRIBUTE */, 0, 0);
                        }
                    }

                    if (MOX::_player[p1].relations[p2] > 100) {
                        MOX::_player[p1].relations[p2] = 100;
                    }
                    if (MOX::_player[p2].relations[p1] > 100) {
                        MOX::_player[p2].relations[p1] = 100;
                    }
                }
            }
        }

        // Phase 2: Adjust relations if a specific objective player threatens a normal player
        for (int p1 = 0; p1 < (int)MOX::_NUM_PLAYERS; ++p1) {
            for (int p2 = 0; p2 < (int)MOX::_NUM_PLAYERS; ++p2) {
                if (MOX::_player[p1].objectives == 100 && MOX::_player[p2].objectives != 100 && MOX::_player[p1].treaty[p2] != 2) {
                    int star_id, threat_val;
                    int planet_id = -1;
                    // TODO: Fix me!
                    aimove::Player_Threatens_Player_(p1, p2, &star_id, &threat_val, &planet_id);

                    if (planet_id != -1) {
                        int16_t col_idx = MOX::_planet[planet_id].colony_index;
                        if (col_idx != -1) {
                            if (star_id != -1 && MOX::_colony[col_idx].n_pops > 4 && threat_val > 100) {
                                int amount = -((MOX::_colony[col_idx].n_pops - 4) + (threat_val / 50));
                                Change_Relations_(amount, p1, p2, 5, planet_id, 0);
                            }
                        }
                    }
                }
            }
        }

        // Phase 3: Drift relations back toward base relations
        for (int p1 = 0; p1 < (int)MOX::_NUM_PLAYERS; ++p1) {
            for (int p2 = 0; p2 < (int)MOX::_NUM_PLAYERS; ++p2) {
                int8_t drift = 0;
                int8_t current_rel = MOX::_player[p1].relations[p2];

                if (abs(current_rel) < (int)random::Random_(105)) {
                    if (random::Random_(4) == 1) {
                        drift = random::Random_(2) - 1; // drift is 0 or -1
                    }
                }

                if (p1 != p2 && MOX::_player[p1].last_turn_diplomacy_severity[p2] == 0) {
                    int8_t base_rel = MOX::_player[p1].base_relations[p2];

                    if (current_rel < base_rel) {
                        current_rel += drift;
                        if (current_rel > base_rel) {
                            current_rel = base_rel;
                        }
                    } else {
                        current_rel -= drift;
                        if (current_rel < base_rel) {
                            current_rel = base_rel;
                        }
                    }

                    // Clamping
                    if (current_rel < -100) current_rel = -100;
                    if (current_rel > 100) current_rel = 100;
                    MOX::_player[p1].relations[p2] = current_rel;
                }

                // Enforce minimum hostility if at war or broken treaties
                if (MOX::_player[p1].treaty[p2] >= 4 && MOX::_player[p1].relations[p2] > -90) {
                    MOX::_player[p1].relations[p2] = -90;
                    MOX::_player[p2].relations[p1] = -90;
                }
            }
        }

        // Phase 4: Synchronize relations between players
        for (int p1 = 0; p1 < (int)MOX::_NUM_PLAYERS; ++p1) {
            for (int p2 = 0; p2 < (int)MOX::_NUM_PLAYERS; ++p2) {
                if (p1 == p2) {
                    MOX::_player[p1].relations[p2] = 0;
                } else {
                    MOX::_player[p2].relations[p1] = MOX::_player[p1].relations[p2];
                }
            }
        }

        // Phase 5: Decrement diplomatic block/spy durations
        for (int p1 = 0; p1 < (int)MOX::_NUM_PLAYERS; ++p1) {
            for (int p2 = 0; p2 < (int)MOX::_NUM_PLAYERS; ++p2) {
                if (MOX::_player[p1].stop_spying_duration[p2] != 0) {
                    MOX::_player[p1].stop_spying_duration[p2]--;
                }
                if (MOX::_player[p1].stop_blockading_duration[p2] != 0) {
                    MOX::_player[p1].stop_blockading_duration[p2]--;
                }
            }
        }

        // Phase 6: Global war if council flag is set
        if (diplodef::_COUNCIL_FLAG == 3) {
            for (int p1 = 0; p1 < (int)MOX::_NUM_PLAYERS; ++p1) {
                for (int p2 = 0; p2 < (int)MOX::_NUM_PLAYERS; ++p2) {
                    if (p1 != p2) {
                        MOX::_player[p1].treaty[p2] = 6 /* TOTAL_WAR */;
                        MOX::_player[p2].treaty[p1] = 6;
                        MOX::_player[p1].relations[p2] = -100;
                        MOX::_player[p2].relations[p1] = -100;
                    }
                }
            }
        }
    }

    void __cdecl Adjust_Diplomat_Modifiers_(int16_t player_idx, int16_t target_idx) {
        // Decrement diplomatic modifiers for a player relative to a target player.
        // Array indexing into player data for player_idx and target_idx.
        MOX::_player[player_idx].trade_modifier[target_idx] -= 10;
        MOX::_player[player_idx].treaty_modifier[target_idx] -= 10;
        MOX::_player[player_idx].tech_exchange_modifier[target_idx] -= 10;
        MOX::_player[player_idx].peace_modifier[target_idx] -= 10;
    }

    void __cdecl Clear_Last_Proposal_Turn_(int16_t player_idx_1, int16_t player_idx_2) {
        MOX::_player[player_idx_1].last_diplomacy_threat_turn[player_idx_2] = 0;
        MOX::_player[player_idx_2].last_diplomacy_threat_turn[player_idx_1] = 0;
    }

    /**
     * Compares ship war losses between two players relative to target player's losses from allies.
     * Returns a score between -300 and 300 representing relative conflict intensity.
     */
    int16_t __cdecl Total_War_Loss_Comparison_(int16_t attacker_idx, int16_t target_idx) {
        int ally_losses_sum = 0;

        if ((int)MOX::_NUM_PLAYERS > 0) {
            for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
                // Check if player is active and has an alliance with the target player
                // 100 (0x64) is a status/objective flag, 4 is TREATY_ALLIANCE
                if (MOX::_player[i].objectives != 100 && MOX::_player[target_idx].treaty[i] >= 4) {
                    ally_losses_sum += MOX::_player[target_idx].ship_war_losses[i];
                }
            }
        }

        if (ally_losses_sum < 10) {
            ally_losses_sum = 10;
        }

        int attacker_losses_vs_target = MOX::_player[attacker_idx].ship_war_losses[target_idx];
        if (attacker_losses_vs_target < 10) {
            attacker_losses_vs_target = 10;
        }

        int result;
        if (attacker_losses_vs_target > ally_losses_sum) {
            // Attacker is suffering more losses than the target's allies are inflicting on the target
            result = 100 - (attacker_losses_vs_target * 100) / ally_losses_sum;
        } else {
            // Attacker is suffering fewer losses than the target's allies are inflicting on the target
            result = (ally_losses_sum * 100) / attacker_losses_vs_target - 100;
        }

        // Clamp result to [-300, 300]
        if (result > 300) {
            result = 300;
        } else if (result < -300) {
            result = -300;
        }

        return (int16_t)result;
    }

    void __cdecl Break_Research_(int16_t player1_idx, int16_t player2_idx) {
        if (player1_idx >= (int16_t)MOX::_NUM_PLAYERS || player2_idx >= (int16_t)MOX::_NUM_PLAYERS) {
            return;
        }

        int p1 = player1_idx;
        int p2 = player2_idx;

        if (MOX::_player[p1].research_treaty[p2] != 0) {
            char research_val = MOX::_player[p1].research_treaty[p2];
            MOX::_player[p2].last_broken_treaty[p1] = 8;

            if (MOX::_player[p1].relations[p2] < -100) {
                MOX::_player[p1].relations[p2] = -100;
            }
            MOX::_player[p2].relations[p1] = MOX::_player[p1].relations[p2];

            MOX::_player[p1].research_treaty[p2] = 0;
            MOX::_player[p2].research_treaty[p1] = 0;

            MOX::_player[p1].reward_attack_player[p2] = 0;
            MOX::_player[p2].reward_attack_player[p1] = 0;

            // Check if player2 is not in a significant alliance and the broken research treaty was active (val 1)
            if (MOX::_player[p2].treaty[p1] < 4 && research_val == 1) {
                if (MOX::_player[p1].personality == 4) {
                    MOX::_player[p1].broken_treaties_modifier[p2] -= 15;
                } else {
                    MOX::_player[p1].broken_treaties_modifier[p2] -= 5;
                }

                int penalty = random::Random_(20) + random::Random_(20);
                int new_rel = MOX::_player[p1].relations[p2] - penalty;
                if (new_rel < -100) {
                    new_rel = -100;
                }
                MOX::_player[p1].relations[p2] = (int8_t)new_rel;

                // Redundant safety check from original code
                if (MOX::_player[p1].relations[p2] < -100) {
                    MOX::_player[p1].relations[p2] = -100;
                }
                MOX::_player[p2].relations[p1] = MOX::_player[p1].relations[p2];

                int base_penalty = random::Random_(20);
                int new_base_rel = MOX::_player[p1].base_relations[p2] - base_penalty;
                if (new_base_rel < -100) {
                    new_base_rel = -100;
                }
                MOX::_player[p1].base_relations[p2] = (int8_t)new_base_rel;
                MOX::_player[p2].base_relations[p1] = MOX::_player[p1].base_relations[p2];

                MOX::_player[p1].last_broken_treaty_type[p2] = 8;

                // Penalize player1's reputation with all other players
                for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
                    if (i != p1 && i != p2) {
                        MOX::_player[i].broken_treaties_modifier[p1] -= 5;
                    }
                }
            }
        }
    }

    void __cdecl Break_Tribute_(int16_t p1_idx, int16_t p2_idx) {
        if (p2_idx >= (int16_t)MOX::_NUM_PLAYERS || p1_idx >= (int16_t)MOX::_NUM_PLAYERS) {
            return;
        }

        // Safety check for player array initialization
        if (&MOX::_player[p1_idx] == reinterpret_cast<s_player*>(std::uintptr_t{0xfffff9c1})) {
            return;
        }

        MOX::_player[p2_idx].threats[p1_idx]++;

        int16_t old_tribute = MOX::_player[p1_idx].tribute_treaty[p2_idx];

        // Ensure relations are within floor and synchronized
        if (MOX::_player[p1_idx].relations[p2_idx] < -100) {
            MOX::_player[p1_idx].relations[p2_idx] = -100;
        }
        MOX::_player[p2_idx].relations[p1_idx] = MOX::_player[p1_idx].relations[p2_idx];

        // Clear existing tribute and related reward targets
        MOX::_player[p1_idx].tribute_treaty[p2_idx] = 0;
        MOX::_player[p2_idx].tribute_treaty[p1_idx] = 0;
        MOX::_player[p1_idx].reward_attack_player[p2_idx] = 0;
        MOX::_player[p2_idx].reward_attack_player[p1_idx] = 0;

        // If there was an active tribute and no existing alliance, apply penalties
        if (MOX::_player[p2_idx].treaty[p1_idx] < 4 /* TREATY_ALLIANCE */ && old_tribute > 0) {
            if (MOX::_player[p1_idx].personality == 4 /* PERSONALITY_RUTHLESS */) {
                MOX::_player[p1_idx].broken_treaties_modifier[p2_idx] -= 20;
            } else {
                MOX::_player[p1_idx].broken_treaties_modifier[p2_idx] -= 10;
            }

            // Calculate a random hit to diplomatic relations
            int penalty = random::Random_(50) + random::Random_(50);
            int current_relations = MOX::_player[p1_idx].relations[p2_idx];

            if (current_relations - penalty < -100) {
                MOX::_player[p1_idx].relations[p2_idx] = -100;
            } else {
                MOX::_player[p1_idx].relations[p2_idx] -= (int8_t)penalty;
            }

            // Final clamp and sync
            if (MOX::_player[p1_idx].relations[p2_idx] < -100) {
                MOX::_player[p1_idx].relations[p2_idx] = -100;
            }
            MOX::_player[p2_idx].relations[p1_idx] = MOX::_player[p1_idx].relations[p2_idx];
        }
    }

    void __cdecl Modifier_Diplomacy_Adjustments_() {
        if (MOX::_NUM_PLAYERS == 0) {
            return;
        }

        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            for (int16_t j = 0; j < (int16_t)MOX::_NUM_PLAYERS; ++j) {
                // If the treaty is not in a specific state (likely contact lost or eliminated)
                if (MOX::_player[i].treaty[j] != 6) {

                    // Gradually improve treaty modifier if it's low or negative
                    if (MOX::_player[i].treaty_modifier[j] < 0) {
                        MOX::_player[i].treaty_modifier[j] += (int16_t)random::Random_(5);
                    }
                    if (MOX::_player[i].treaty_modifier[j] < 50) {
                        MOX::_player[i].treaty_modifier[j] += (int16_t)random::Random_(5);
                    }

                    // Gradually improve trade modifier if it's low or negative
                    if (MOX::_player[i].trade_modifier[j] < 0) {
                        MOX::_player[i].trade_modifier[j] += (int16_t)random::Random_(5);
                    }
                    if (MOX::_player[i].trade_modifier[j] < 50) {
                        MOX::_player[i].trade_modifier[j] += (int16_t)random::Random_(5);
                    }

                    // Gradually improve tech exchange modifier if it's low or negative
                    if (MOX::_player[i].tech_exchange_modifier[j] < 0) {
                        MOX::_player[i].tech_exchange_modifier[j] += (int16_t)random::Random_(5);
                    }
                    if (MOX::_player[i].tech_exchange_modifier[j] < 50) {
                        MOX::_player[i].tech_exchange_modifier[j] += (int16_t)random::Random_(5);
                    }

                    // Gradually improve peace modifier if it's low or negative
                    if (MOX::_player[i].peace_modifier[j] < 0) {
                        MOX::_player[i].peace_modifier[j] += (int16_t)random::Random_(5);
                    }
                    if (MOX::_player[i].peace_modifier[j] < 50) {
                        MOX::_player[i].peace_modifier[j] += (int16_t)random::Random_(5);
                    }

                    // Clamp peace modifier between -200 and 200
                    if (MOX::_player[i].peace_modifier[j] < -200) {
                        MOX::_player[i].peace_modifier[j] = -200;
                    }
                    if (MOX::_player[i].peace_modifier[j] > 200) {
                        MOX::_player[i].peace_modifier[j] = 200;
                    }
                }
            }
        }
    }
}