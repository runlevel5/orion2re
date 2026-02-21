#include "deps.h"

namespace DIP_SCRN {
    /**
     * Iterates through all player pairs to process pending diplomatic commands 
     * (war, peace, breaking treaties) that were queued up.
     */
    void Resolve_Delayed_Diplomacy_Orders_() {
        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            for (int16_t j = 0; j < MOX::_NUM_PLAYERS; ++j) {
                if (i == j) {
                    continue;
                }

                // Only process diplomatic orders if both parties are still in the game
                if (MOX::_player[j].eliminated == 0 && MOX::_player[i].eliminated == 0) {
                    int8_t order = MOX::_player[i].delayed_diplomacy_orders[j];

                    if (order == 0) {
                        diplomac::Declare_War_(i, j);
                    } else if (order == 1) {
                        diplomac::Declare_Peace_(i, j);
                    } else if (order == 2) {
                        diplomac::Break_Treaties_(i, j);
                    }

                    // Reset the order to "no order" (-1) once processed or if invalid
                    MOX::_player[i].delayed_diplomacy_orders[j] = -1;
                }
            }
        }
    }

    /**
     * @brief Scans for potential opportunity attacks for each player against winning opponents.
     *
     * Iterates through all player combinations. If an active player (not eliminated, not winning)
     * is in range and contact with a winning player (objectives == 100), and no hyperspace flux
     * is active, it calculates and stores the best opportunity attack planet and its potential.
     */
    void __cdecl Set_Opportunity_Attacks_() {
        if (MOX::_NUM_PLAYERS <= 0) {
            return;
        }

        for (int16_t p_idx = 0; p_idx < (int16_t)MOX::_NUM_PLAYERS; ++p_idx) {
            for (int16_t target_idx = 0; target_idx < (int16_t)MOX::_NUM_PLAYERS; ++target_idx) {
                s_player& player = MOX::_player[p_idx];
                s_player& target = MOX::_player[target_idx];

                bool valid_opportunity = false;

                // Condition for player looking for an opportunity:
                // Must not be eliminated, must not already have reached 100 objectives,
                // and hyperspace flux must not be present.
                if (player.eliminated == 0 && player.objectives != 100) {
                    if (EVENTS::Event_Check_Hyperspace_Flux_() == 0) {
                        // Condition for the target:
                        // Must have reached 100 objectives, must be a different player,
                        // must not be eliminated, and must be in contact and within range.
                        if (target.objectives == 100 && p_idx != target_idx && target.eliminated == 0 &&
                            player.contact[target_idx] == 1 && player.within_range[target_idx] == 1)
                        {
                            valid_opportunity = true;
                        }
                    }
                }

                if (valid_opportunity) {
                    int16_t planet_num = -1;
                    int16_t value = 0;
                    int32_t ratio = 0;

                    aimove::Find_Opportunity_Attack_(p_idx, target_idx, 100, &planet_num, &value, &ratio);

                    player.opportunity_planet_num[target_idx] = planet_num;
                    player.opportunity_planet_value[target_idx] = value;
                    player.opportunity_actual_attacker_ratio[target_idx] = ratio;
                } else {
                    player.opportunity_planet_num[target_idx] = -1;
                    player.opportunity_planet_value[target_idx] = 0;
                    player.opportunity_actual_attacker_ratio[target_idx] = 0;
                }
            }
        }
    }
}