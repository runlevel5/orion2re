#include "deps.h"
#include <cstdlib>
#include "diplodef.h"
#include "npcdiplo.h"

namespace diplomac {
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

    void Clear_Diplomacy_Messages_() {
        NoOp2();
        Update_Diplomatic_Status_();

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
                            Determine_Bad_Message_(i, j, incident);
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
                                Declare_War(j, i);
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
                            Peace_Proposal_(i, j, 0);
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
}