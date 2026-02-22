#include <cstring>
#include "deps.h"
#include "aibuild.h"

namespace AIBUILD {
    int16_t __cdecl Mean_Ship_Strength_(s_ship_data* ship) {
        int ship_idx = (int)(ship - MOX::_ship);
        int owner = (int)ship->owner;
        int16_t result_strength = 0;

        if ((*AIDATA::_ai_player_info)[owner].n_missing_contacts == 0) {
            uint32_t total_str = 0;
            uint32_t count = 0;

            for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
                if (i != owner && !MOX::_player[i].eliminated && MOX::_player[i].objectives == 100 /* 'd' */) {
                    count++;
                    total_str += (uint16_t)(*AIDATA::_ship_strengths)[ship_idx][i];
                }
            }

            if (count != 0) {
                result_strength = (int16_t)(total_str / count);
            }
        } else {
            uint32_t count_darlok = 0;
            uint32_t total_darlok = 0;
            uint32_t min_darlok = 0xffffffff;

            uint32_t count_other = 0;
            uint32_t total_other = 0;
            uint32_t min_other = 0xffffffff;

            for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
                if (i != owner && !MOX::_player[i].eliminated && (*AIDATA::_ai_player_info)[owner].reachable_players[i]) {
                    uint32_t str = (uint16_t)(*AIDATA::_ship_strengths)[ship_idx][i];

                    if (MOX::_player[i].objectives == 100 /* 'd' */) {
                        total_darlok += str;
                        if (str < min_darlok) {
                            min_darlok = str;
                        }
                        count_darlok++;
                    } else {
                        total_other += str;
                        if (str < min_other) {
                            min_other = str;
                        }
                        count_other++;
                    }
                }
            }

            if (count_darlok == 0) {
                if (count_other != 0) {
                    result_strength = (int16_t)(total_other / count_other);
                }
            } else if (count_other == 0) {
                result_strength = (int16_t)(total_darlok / count_darlok);
            } else {
                if (min_other < min_darlok) {
                    result_strength = (int16_t)min_other;
                } else {
                    int8_t bias = AIBUILD::_difficulty_intel_bias[MOX::_settings.game_difficulty];
                    result_strength = (int16_t)((min_other + (uint32_t)bias * min_darlok) / (bias + 1));
                }
            }
        }

        return result_strength;
    }

    int16_t __cdecl Refit_Cost_(int16_t player_idx, s_ship_design* old_design, s_ship_design* new_design) {
        // Minimum cost is half of the hull's available space
        int32_t min_cost = (int32_t)TECHDATA::_hull_data[(uint8_t)new_design->size].hull_space / 2;
        
        // Base refit cost is twice the difference in design costs
        int32_t refit_cost = ((int32_t)new_design->cost - (int32_t)old_design->cost) * 2;

        // Apply government-based cost reductions to the floor price
        DESIGN::Cost_Reduction_For_Govt_Type_(player_idx, &min_cost);

        // Result cannot be lower than the floor price
        if (refit_cost < min_cost) {
            refit_cost = min_cost;
        }

        return (int16_t)refit_cost;
    }

    /**
     * Calculates the highest aggressive treaty status level for a given player 
     * based on their diplomatic relationships with all other players they have contact with.
     */
    int16_t __cdecl Player_War_Level_(int16_t player_idx) {
        int16_t max_war_level = 0;

        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            // Skip self and ensure contact has been established
            if (player_idx != i && MOX::_player[player_idx].contact[i] != 0) {
                int8_t treaty_type = MOX::_player[player_idx].treaty[i];

                if (treaty_type == 4 /* War */) {
                    if (max_war_level < 1) {
                        max_war_level = 1;
                    }
                }
                else if (treaty_type == 5 /* Aggression */) {
                    if (max_war_level < 2) {
                        max_war_level = 2;
                    }
                }
                else if (treaty_type == 6 /* Truce */) {
                    if (max_war_level < 3) {
                        max_war_level = 3;
                    }
                }
            }
        }

        return max_war_level;
    }

    void __cdecl Compute_Empire_Building_Needs_() {
        MOX::Mox_Update_();

        memset(_player_total_strength_vs_ships, 0, sizeof(_player_total_strength_vs_ships));
        memset(_player_ship_sizes, 0, sizeof(_player_ship_sizes));
        memset(_player_total_strength_vs_colonies, 0, sizeof(_player_total_strength_vs_colonies));
        memset(_player_n_colony_ships, 0, sizeof(_player_n_colony_ships));
        memset(_player_n_transports, 0, sizeof(_player_n_transports));

        for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
            s_ship_data* ship = &MOX::_ship[i];

            if ((ship->status < 5 || ship->status == 6) && (uint8_t)ship->owner < 8) {
                int8_t owner = (int8_t)ship->owner;

                if (ship->d.ship_type == SHIP_TYPE_COMBAT) {
                    int strength = (int)Mean_Ship_Strength_(ship);
                    _player_total_strength_vs_ships[owner][0] += strength;

                    if (ship->status != 6) {
                        _player_total_strength_vs_ships[owner][1] += strength;
                    }

                    _player_ship_sizes[owner][(int)ship->d.size]++;
                    _player_ship_sizes[owner][13] += (int)_player_ship_size_table[(int)ship->d.size];
                    _player_ship_sizes[owner][11] += ((int)ship->d.size + 1);
                    _player_ship_sizes[owner][10]++;

                    if (ship->status != 6) {
                        _player_ship_sizes[owner][12] += ((int)ship->d.size + 1);
                    }
                } else if (ship->d.ship_type == SHIP_TYPE_COLONY) {
                    _player_n_colony_ships[owner][0]++;
                    if (ship->status == 1 || ship->status == 2) {
                        _player_n_colony_ships[owner][2]++;
                    }
                } else if (ship->d.ship_type == SHIP_TYPE_TRANSPORT) {
                    _player_n_transports[owner][0]++;
                }
            }
        }

        MOX::Mox_Update_();
        memset(_player_n_spies, 0, sizeof(_player_n_spies));

        if (MOX::_NUM_PLAYERS > 0) {
            for (int16_t owner = 0; owner < MOX::_NUM_PLAYERS; ++owner) {
                for (int16_t target = 0; target < MOX::_NUM_PLAYERS; ++target) {
                    int16_t spy_count = BILL::Get_Their_Spy_Number_(owner, target);
                    _player_n_spies[owner][0] += (int8_t)spy_count;
                }
            }
        }

        MOX::Mox_Update_();
        for (int16_t player = 0; player < MOX::_NUM_PLAYERS; ++player) {
            Compute_Needs_(player);
        }
    }

    void __cdecl Compute_Colony_Ship_Need_(int16_t player_num) {
        if (_player_n_colony_ships[player_num][0] == 0) {
            _player_n_colony_ships[player_num][1] = 1;
        }
    }

    /**
     * Calculates the number of freighter groups required for a player based on their current 
     * surplus and existing colony ship requirements.
     */
    void __cdecl Compute_Freighter_Need_(int16_t player_idx) {
        int32_t i = player_idx;
        int16_t surplus = MOX::_player[i].surplus_freighters;
        
        // Zero-extend byte value as per assembly sequence MOV AL, byte / AND EAX, 0xFFFF00FF / SUB CX, AX
        uint16_t colony_ship_need = static_cast<uint8_t>(_player_n_colony_ships[i][2]);
        
        _player_n_freighter_groups_wanted[i] = 0;
        
        int16_t diff = surplus - static_cast<int16_t>(colony_ship_need);
        
        // If the player has a deficit in freighters
        if (diff < 0) {
            // Calculate groups needed: (4 - deficit) / 5.
            // This logic performs a ceiling division of the absolute deficit by 5.
            _player_n_freighter_groups_wanted[i] = static_cast<int8_t>((4 - static_cast<int32_t>(diff)) / 5);
        }
    }

    void __cdecl Compute_Needs_(int16_t player_idx) {
        bool is_objective_1;
        int highest_ship_size_idx;
        int total_contacts;
        int total_transports;
        int max_transports;
        int sum_strength_vs_ships;
        int max_strength_vs_ships;
        int max_strength_vs_colonies;
        int max_spies;
        int max_ship_size_13;
        int strength_target;

        is_objective_1 = (MOX::_player[player_idx].objectives == 1);
        highest_ship_size_idx = 0;
        total_contacts = 0;
        max_transports = 0;
        total_transports = 0;
        max_strength_vs_ships = 0;
        sum_strength_vs_ships = 0;
        max_strength_vs_colonies = 0;
        max_spies = 0;
        max_ship_size_13 = 0;

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (player_idx != i) {
                bool aware = ((*AIDATA::_ai_player_info)[player_idx].reachable_players[i] != 0);
                if (!aware) {
                    if ((*AIDATA::_ai_player_info)[player_idx].n_missing_contacts == 0 && MOX::_player[i].objectives == 100 /* 'd' */) {
                        aware = true;
                    }
                }

                if (aware) {
                    total_contacts++;
                    int transports = (uint8_t)AIBUILD::_player_n_transports[i][0];
                    total_transports += transports;
                    if (max_transports < transports) {
                        max_transports = transports;
                    }

                    int ship_strength = AIBUILD::_player_total_strength_vs_ships[i][0];
                    sum_strength_vs_ships += ship_strength;
                    if (max_strength_vs_ships < ship_strength) {
                        max_strength_vs_ships = ship_strength;
                    }

                    if (max_strength_vs_colonies < AIBUILD::_player_total_strength_vs_colonies[i]) {
                        max_strength_vs_colonies = AIBUILD::_player_total_strength_vs_colonies[i];
                    }

                    int spies = (uint8_t)AIBUILD::_player_n_spies[i][0];
                    if (max_spies < spies) {
                        max_spies = spies;
                    }

                    if (max_ship_size_13 < AIBUILD::_player_ship_sizes[i][13]) {
                        max_ship_size_13 = AIBUILD::_player_ship_sizes[i][13];
                    }

                    for (int j = 0; j < 9; ++j) {
                        if (AIBUILD::_player_ship_sizes[i][j] != 0 && highest_ship_size_idx < j) {
                            highest_ship_size_idx = j;
                        }
                    }
                }
            }
        }

        AIBUILD::_player_ship_sizes[player_idx][9] = highest_ship_size_idx + 1;
        if (max_strength_vs_ships < AIBUILD::_player_total_strength_vs_ships[player_idx][0] || 
            (*AIDATA::_ai_player_info)[player_idx].n_missing_contacts == 0) {
            int rand_val = random::Random_(2);
            AIBUILD::_player_ship_sizes[player_idx][9] = highest_ship_size_idx + 1 + rand_val + (is_objective_1 ? 1 : 0) - 1;
        }

        if (total_contacts == 0) {
            total_contacts = 1;
        }

        AIBUILD::_player_n_transports[player_idx][1] = 0;
        // Index 189 in tech_applications is likely TECH_APP_TRANSPORT
        if (MOX::_player[player_idx].tech_applications[189] == 3 && AIBUILD::_player_ship_sizes[player_idx][13] > 1) {
            int colony_limit = ((*AIDATA::_ai_player_info)[player_idx].colony_count + 2) / 3;
            int transport_avg = (max_transports + 1 + total_transports / total_contacts) / 2;
            
            if (AIBUILD::_player_n_transports[player_idx][0] < transport_avg) {
                AIBUILD::_player_n_transports[player_idx][1] = (int8_t)(transport_avg - AIBUILD::_player_n_transports[player_idx][0]);
            }

            if ((*AIDATA::_ai_player_info)[player_idx].n_missing_contacts > 0) {
                int missing_need = (is_objective_1 ? 1 : 0) + (*AIDATA::_ai_player_info)[player_idx].n_missing_contacts + (int)isqrt_((*AIDATA::_ai_player_info)[player_idx].colony_count);
                if (AIBUILD::_player_n_transports[player_idx][0] < missing_need) {
                    int diff = missing_need - AIBUILD::_player_n_transports[player_idx][0];
                    if (AIBUILD::_player_n_transports[player_idx][1] < diff) {
                        AIBUILD::_player_n_transports[player_idx][1] = (int8_t)diff;
                    }
                }
            }

            if (AIBUILD::_player_n_transports[player_idx][0] < 20) {
                if (colony_limit < AIBUILD::_player_n_transports[player_idx][1]) {
                    AIBUILD::_player_n_transports[player_idx][1] = (int8_t)colony_limit;
                    if ((*AIDATA::_ai_player_info)[player_idx].n_missing_contacts == 0) {
                        AIBUILD::_player_n_transports[player_idx][1] >>= 2;
                    }
                }
            } else {
                AIBUILD::_player_n_transports[player_idx][1] = 0;
            }

            if (AIBUILD::_player_n_transports[player_idx][1] > 1 && MOX::_player[player_idx].command_points < MOX::_player[player_idx].command_points_used) {
                AIBUILD::_player_n_transports[player_idx][1] = 1;
            }
        }

        if (is_objective_1) {
            strength_target = max_strength_vs_ships;
        } else {
            strength_target = (max_strength_vs_ships + 1 + sum_strength_vs_ships / total_contacts) / 2;
        }

        AIBUILD::_player_total_strength_vs_ships[player_idx][2] = 0;
        if (AIBUILD::_player_total_strength_vs_ships[player_idx][0] < strength_target) {
            int diff = strength_target - AIBUILD::_player_total_strength_vs_ships[player_idx][0];
            AIBUILD::_player_total_strength_vs_ships[player_idx][2] = diff;
            if ((*AIDATA::_ai_player_info)[player_idx].n_missing_contacts == 0) {
                AIBUILD::_player_total_strength_vs_ships[player_idx][2] = diff / 2;
            }
        }

        int current_need = AIBUILD::_player_total_strength_vs_ships[player_idx][2];
        if (current_need == 0 || MOX::_player[player_idx].command_points_used < MOX::_player[player_idx].command_points) {
            if ((*AIDATA::_ai_player_info)[player_idx].n_missing_contacts > 0) {
                if (random::Random_(10 - (is_objective_1 ? 1 : 0)) == 1) {
                    AIBUILD::_player_total_strength_vs_ships[player_idx][2] += (is_objective_1 ? 4 : 0) + 1;
                }
            }
        } else if (MOX::_player[player_idx].bc < 50 || MOX::_player[player_idx].surplus_bc < 50) {
            AIBUILD::_player_total_strength_vs_ships[player_idx][2] = 0;
        } else if (MOX::_player[player_idx].surplus_bc < 1024) {
            int scaled_need = (isqrt_(MOX::_player[player_idx].surplus_bc) * current_need) >> 5;
            AIBUILD::_player_total_strength_vs_ships[player_idx][2] = scaled_need;
            if (Player_War_Level_(player_idx) == 0) {
                AIBUILD::_player_total_strength_vs_ships[player_idx][2] = isqrt_(scaled_need) >> 4;
            }
        }

        if (MOX::_player[player_idx].surplus_bc < 1024) {
            int divisor = AIBUILD::_player_ship_sizes[player_idx][13];
            if (max_ship_size_13 < divisor) {
                AIBUILD::_player_total_strength_vs_ships[player_idx][2] = (AIBUILD::_player_total_strength_vs_ships[player_idx][2] * max_ship_size_13) / divisor;
            }
        } else {
            if (AIBUILD::_player_total_strength_vs_ships[player_idx][2] <= 9 && AIBUILD::_player_ship_sizes[player_idx][12] != 0) {
                int ratio = AIBUILD::_player_total_strength_vs_ships[player_idx][1] / AIBUILD::_player_ship_sizes[player_idx][12];
                if (AIBUILD::_player_ship_sizes[player_idx][11] < MOX::_player[player_idx].command_points) {
                    AIBUILD::_player_total_strength_vs_ships[player_idx][2] = (MOX::_player[player_idx].command_points - AIBUILD::_player_ship_sizes[player_idx][11]) * ratio;
                } else {
                    AIBUILD::_player_total_strength_vs_ships[player_idx][2] = (MOX::_player[player_idx].surplus_bc * ratio) / 15;
                }
            }
        }

        AIBUILD::_player_n_spies[player_idx][1] = 0;
        if (AIBUILD::_player_total_strength_vs_ships[player_idx][0] > 0) {
            int spy_limit = 0x3F - BILL::Get_Their_Spy_Number_(player_idx, player_idx);
            if ((*AIDATA::_ai_player_info)[player_idx].n_contacts > 0 && spy_limit > 0) {
                int current_spies = (uint8_t)AIBUILD::_player_n_spies[player_idx][0];
                if (current_spies < max_spies) {
                    AIBUILD::_player_n_spies[player_idx][1] = (int8_t)(max_spies - current_spies);
                }

                if (current_spies < max_spies + 2 && random::Random_(100) <= MOX::_player[player_idx].traits[9]) {
                    AIBUILD::_player_n_spies[player_idx][1]++;
                }

                if (MOX::_player[player_idx].objectives == 0 && random::Random_(2) == 1) {
                    AIBUILD::_player_n_spies[player_idx][1]++;
                }

                int8_t spy_need = AIBUILD::_player_n_spies[player_idx][1];
                if (spy_need != 0) {
                    int cap = (*AIDATA::_ai_player_info)[player_idx].n_contacts * ((MOX::_player[player_idx].traits[9] > 0 ? 1 : 0) + 1) * 2;
                    if (cap < current_spies) {
                        if (spy_need < 2) {
                            if (MOX::_player[player_idx].traits[9] < 1 && random::Random_(4) != 1) {
                                AIBUILD::_player_n_spies[player_idx][1] = 0;
                            } else {
                                AIBUILD::_player_n_spies[player_idx][1] = 1;
                            }
                        } else {
                            AIBUILD::_player_n_spies[player_idx][1] = (int8_t)isqrt_((uint8_t)spy_need);
                        }
                    }
                }

                if (spy_limit < (uint8_t)AIBUILD::_player_n_spies[player_idx][1]) {
                    AIBUILD::_player_n_spies[player_idx][1] = (int8_t)spy_limit;
                }
            }
        }

        Compute_Colony_Ship_Need_(player_idx);
        Compute_Freighter_Need_(player_idx);
    }
}