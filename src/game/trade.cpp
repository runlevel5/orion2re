#include "deps.h"

namespace TRADE {
    /**
     * Iterates through all players and their active treaties, processing 
     * incremental trade and research benefits.
     */
    void __cdecl Process_Trade_And_Research_Agreements_() {
        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            // MOX::_player is an array of s_player structures (size 0xEA9)
            s_player* current_player = &MOX::_player[i];

            for (int16_t other_idx = 0; other_idx < MOX::_NUM_PLAYERS; ++other_idx) {
                // If a trade treaty exists with the other player
                if (current_player->trade_treaty[other_idx] != 0) {
                    Process_Trade_Agreement_(current_player, other_idx);
                }

                // If a research treaty exists with the other player
                if (current_player->research_treaty[other_idx] != 0) {
                    Process_Research_Agreement_(current_player, other_idx);
                }
            }
        }

        // Finalize population counts related to trade/research
        Compute_N_Trade_And_Research_Pops_();
    }

    /**
     * @address 00466160
     * Calculates the base goal value for a trade agreement between two players.
     * The goal is half the minimum of the trade population of both players,
     * further halved if either player is experiencing pirate activity.
     */
    int16_t __cdecl Base_Trade_Agreement_Goal_(s_player* player, int16_t player_idx) {
        int16_t pops1 = MOX::_player[player_idx].n_trade_pops;
        int16_t pops2 = player->n_trade_pops;

        int16_t min_pops = (pops1 < pops2) ? pops1 : pops2;
        int16_t goal = min_pops / 2;

        uint8_t p1_idx = static_cast<uint8_t>(player - MOX::_player);
        uint8_t p2_idx = static_cast<uint8_t>(player_idx);

        if (EVENTS::Event_Check_Pirate_Activity_(p1_idx) != 0 ||
            EVENTS::Event_Check_Pirate_Activity_(p2_idx) != 0) {
            goal /= 2;
        }

        return goal;
    }

    /**
     * @address 004664c0
     * Calculates the total trade agreement goal, applying multipliers from player 
     * traits (Federation, Confederation, Fantastic Traders) and the best applicable 
     * leader bonus (Trader or Diplomat skills).
     */
    int16_t __cdecl Trade_Agreement_Goal_(s_player* player, int16_t player_idx) {
        int trait_multiplier = 100;
        int16_t base_goal = Base_Trade_Agreement_Goal_(player, player_idx);

        // traits[0] refers to a specific charisma/personality trait
        if (player->traits[0] == 4) {
            trait_multiplier = 150; // 0x96
        } else if (player->traits[0] == 5) {
            trait_multiplier = 175; // 0xaf
        }

        if (player->traits[TRAIT_FANTASTIC_TRADERS] != 0) {
            trait_multiplier += 50; // 0x32
        }

        int16_t max_leader_bonus = 0;
        int current_player_index = static_cast<int>(player - MOX::_player);

        for (int i = 0; i < MAX_LEADERS; ++i) {
            s_leader_data* leader = &MOX::_leaders[i];

            if (static_cast<int>(leader->player_index) == current_player_index &&
                leader->status >= 0 && leader->status < 3) {

                int leader_bonus = 0;
                if (leader->general_skills & 0x80000) { // Trader skill
                    int16_t level = OFFICER::Owned_Officer_Level_(i);
                    leader_bonus = (level * 3 + 3) * 5;
                } else if (leader->general_skills & 0x40000) { // Diplomat skill
                    int16_t level = OFFICER::Owned_Officer_Level_(i);
                    leader_bonus = (level * 5 + 5) * 2;
                }

                if (max_leader_bonus < static_cast<int16_t>(leader_bonus)) {
                    max_leader_bonus = static_cast<int16_t>(leader_bonus);
                }
            }
        }

        return (static_cast<int>(base_goal) * (max_leader_bonus + trait_multiplier)) / 100;
    }
}