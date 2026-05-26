#include "pch.h"

namespace TRADE {
    void __cdecl Process_Research_Agreement_(s_player* plr, int16_t target_plr_idx) {
        int32_t goal = Research_Agreement_Goal_(plr, target_plr_idx);
        int16_t current_level = plr->current_research_agreement_level[target_plr_idx];

        if (current_level < goal) {
            int16_t increment = 0;
            int16_t remainder = goal % 5;

            // Determine if an additional progress point is gained based on the remainder of the goal
            if (remainder != 0) {
                if ((int16_t)random::Random_(5) <= remainder) {
                    increment = 1;
                }
            }

            // Calculate the new level based on progress speed (goal / 5) and random increment
            int16_t next_level = (goal / 5) + current_level + increment;

            // Cap the progress at the goal
            if (next_level <= goal) {
                current_level = next_level;
            } else {
                current_level = goal;
            }
        } else {
            // If already at or past the goal, reset/cap to the current goal
            current_level = goal;
        }

        plr->research_agreement_goal[target_plr_idx] = goal;
        plr->current_research_agreement_level[target_plr_idx] = current_level;
    }

    void __cdecl Process_Trade_Agreement_(s_player* plr, int16_t target_plr_idx) {
        int16_t goal = Trade_Agreement_Goal_(plr, target_plr_idx);
        int16_t current_level = plr->current_trade_agreement_level[target_plr_idx];

        if (current_level < goal) {
            int16_t increment = 0;
            int16_t remainder = goal % 5;

            // Roll for a bonus increment based on the remainder of the goal
            if (remainder != 0) {
                if ((int16_t)random::Random_(5) <= remainder) {
                    increment = 1;
                }
            }

            // The trade level grows by 20% of the goal value per step plus a potential bonus
            current_level += (goal / 5) + increment;

            // Clamp the level to the goal
            if (current_level > goal) {
                current_level = goal;
            }
        } else {
            // If the goal is lower than current level, the level is set to the goal immediately
            current_level = goal;
        }

        plr->trade_agreement_goal[target_plr_idx] = goal;
        plr->current_trade_agreement_level[target_plr_idx] = current_level;
    }

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

    int32_t __cdecl Get_Combined_Research_Points_(s_player* plr, int16_t target_plr_idx) {
        int16_t target_research_pops = MOX::_player[target_plr_idx].n_research_pops;
        int16_t source_research_pops = plr->n_research_pops;

        int16_t min_research_pops = target_research_pops;
        if (source_research_pops < target_research_pops) {
            min_research_pops = source_research_pops;
        }

        return static_cast<int32_t>(min_research_pops) / 2;
    }

    int16_t __cdecl Trade_Agreement_Goal_(s_player* player, int16_t player_idx) {
        int trait_multiplier = 100;
        int16_t base_goal = Base_Trade_Agreement_Goal_(player, player_idx);

        // traits[0] refers to a specific charisma/personality trait
        if (player->traits[TRAIT_CURRENT_GOVERNMENT] == 4) {
            trait_multiplier = 150; // 0x96
        } else if (player->traits[TRAIT_CURRENT_GOVERNMENT] == 5) {
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

    int32_t __cdecl Research_Agreement_Goal_(s_player* plr, int16_t target_plr_idx) {
        int32_t combined_points = Get_Combined_Research_Points_(plr, target_plr_idx);
        // The original logic truncates the combined points to 16 bits before applying modifiers.
        int16_t base_value = (int16_t)combined_points;

        switch (plr->traits[TRAIT_CURRENT_GOVERNMENT]) {
            case 0:
                return static_cast<int16_t>(static_cast<int>(base_value) / 2);
            case 1:
                return static_cast<int16_t>((static_cast<int>(base_value) * 3) / 4);
            case 4:
                return static_cast<int16_t>((static_cast<int>(base_value) * 3) / 2);
            case 5:
                return static_cast<int16_t>((static_cast<int>(base_value) * 7) / 4);
            default:
                // For other traits (e.g., cases 2, 3 or values > 5), the original 32-bit result is returned.
                return combined_points;
        }
    }

    void __cdecl Compute_N_Trade_And_Research_Pops_() {
        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            MOX::_player[i].n_trade_pops = 0;
            MOX::_player[i].n_research_pops = 0;
        }

        for (int i = 0; i < (int)MOX::_NUM_COLONIES; ++i) {
            s_colony& colony = MOX::_colony[i];

            if (colony.owner != -1) {
                int16_t colony_trade_pops = 0;
                int16_t colony_research_pops = 0;
                uint8_t n_pops = (uint8_t)colony.n_pops;

                for (int j = 0; j < (int)n_pops; ++j) {
                    uint32_t pop_unit = colony.pop[j];
                    uint32_t type = pop_unit & 0x0F;

                    if (type < 8) {
                        colony_trade_pops++;
                        colony_research_pops++;
                    } else if (type == 8 && (pop_unit & 0x180) == 0x100) {
                        // Case specifically for certain research-contributing population types (e.g., androids or natives)
                        colony_research_pops++;
                    }
                }

                MOX::_player[colony.owner].n_trade_pops += colony_trade_pops;
                MOX::_player[colony.owner].n_research_pops += colony_research_pops;
            }
        }
    }

    void __cdecl Start_Trade_Agreement_(s_player* player_a, int16_t player_b_idx) {
        int16_t base_goal = TRADE::Base_Trade_Agreement_Goal_(player_a, player_b_idx);
        // Calculate player_a index from pointer subtraction (s_player size is 0xEA9)
        int16_t player_a_idx = static_cast<int16_t>(player_a - MOX::_player);
        s_player* player_b = &MOX::_player[player_b_idx];

        player_a->trade_treaty[player_b_idx] = 1;
        player_a->current_trade_agreement_level[player_b_idx] = -base_goal;
        player_a->trade_agreement_goal[player_b_idx] = TRADE::Trade_Agreement_Goal_(player_a, player_b_idx);

        player_b->trade_treaty[player_a_idx] = 1;
        player_b->current_trade_agreement_level[player_a_idx] = -base_goal;
        player_b->trade_agreement_goal[player_a_idx] = TRADE::Trade_Agreement_Goal_(player_b, player_a_idx);

        COLCALC::Update_Player_Stats_(player_a);
        COLCALC::Update_Player_Stats_(player_b);
    }

    void __cdecl Start_Research_Agreement_(s_player* player_a, int16_t player_b_idx) {
        int16_t combined_pts;
        int16_t player_a_idx;
        s_player* player_b;
        int32_t goal;

        combined_pts = static_cast<int16_t>(TRADE::Get_Combined_Research_Points_(player_a, player_b_idx));

        player_a_idx = static_cast<int16_t>(player_a - MOX::_player);

        player_a->research_treaty[player_b_idx] = 1;
        player_a->current_research_agreement_level[player_b_idx] = -combined_pts;

        player_b = &MOX::_player[player_b_idx];

        goal = TRADE::Research_Agreement_Goal_(player_a, player_b_idx);
        player_a->research_agreement_goal[player_b_idx] = goal;

        player_b->research_treaty[player_a_idx] = 1;
        player_b->current_research_agreement_level[player_a_idx] = -combined_pts;

        goal = TRADE::Research_Agreement_Goal_(player_b, player_a_idx);
        player_b->research_agreement_goal[player_a_idx] = goal;

        COLCALC::Update_Player_Stats_(player_a);
        COLCALC::Update_Player_Stats_(player_b);
    }
}