#include "deps.h"
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

        diplodef::_COUNCIL_FLAG = 0;
        diplodef::_council_winner = -1;
        diplodef::_next_council_meeting_turn = 25;
    }
}