#include "pch.h"

namespace AIBUILD {
    int16_t __cdecl Mean_Ship_Strength_(s_ship_data* ship) {
        int ship_idx = (int)(ship - MOX::_ship);
        int owner = (int)ship->owner;
        int16_t result_strength = 0;

        if ((*AIDATA::_ai_player_info)[owner].n_reachable_players == 0) {
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
            uint32_t count_human = 0;
            uint32_t total_human = 0;
            uint32_t min_human = 0xffffffff;

            uint32_t count_other = 0;
            uint32_t total_other = 0;
            uint32_t min_other = 0xffffffff;

            for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
                if (i != owner && !MOX::_player[i].eliminated && (*AIDATA::_ai_player_info)[owner].reachable_players[i]) {
                    uint32_t str = (uint16_t)(*AIDATA::_ship_strengths)[ship_idx][i];

                    if (MOX::_player[i].objectives == 100 /* 'd' */) {
                        total_human += str;
                        if (str < min_human) {
                            min_human = str;
                        }
                        count_human++;
                    } else {
                        total_other += str;
                        if (str < min_other) {
                            min_other = str;
                        }
                        count_other++;
                    }
                }
            }

            if (count_human == 0) {
                if (count_other != 0) {
                    result_strength = (int16_t)(total_other / count_other);
                }
            } else if (count_other == 0) {
                result_strength = (int16_t)(total_human / count_human);
            } else {
                if (min_other < min_human) {
                    result_strength = (int16_t)min_other;
                } else {
                    uint32_t bias = (uint8_t)AIBUILD::_difficulty_intel_bias[MOX::_settings.game_difficulty];
                    result_strength = (int16_t)((min_other + bias * min_human) / (bias + 1));
                }
            }
        }

        return result_strength;
    }

    int16_t __cdecl Refit_Cost_(int16_t player_idx, s_ship_design* old_design, s_ship_design* new_design) {
        // Minimum cost is half of the hull's available space
        int32_t min_cost = (int32_t)TECHDATA::_hull_data[(uint8_t)new_design->size].cost / 2;
        
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
        RUSS::Mox_Update_();

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

        RUSS::Mox_Update_();
        memset(_player_n_spies, 0, sizeof(_player_n_spies));

        if (MOX::_NUM_PLAYERS > 0) {
            for (int16_t owner = 0; owner < MOX::_NUM_PLAYERS; ++owner) {
                for (int16_t target = 0; target < MOX::_NUM_PLAYERS; ++target) {
                    int16_t spy_count = BILL::Get_Their_Spy_Number_(owner, target);
                    _player_n_spies[owner][0] += (int8_t)spy_count;
                }
            }
        }

        RUSS::Mox_Update_();
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
                    if ((*AIDATA::_ai_player_info)[player_idx].n_reachable_players == 0 && MOX::_player[i].objectives == 100 /* 'd' */) {
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
            (*AIDATA::_ai_player_info)[player_idx].n_reachable_players == 0) {
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

            if ((*AIDATA::_ai_player_info)[player_idx].n_reachable_players > 0) {
                int missing_need = (is_objective_1 ? 1 : 0) + (*AIDATA::_ai_player_info)[player_idx].n_reachable_players + (int)isqrt_((*AIDATA::_ai_player_info)[player_idx].colony_count);
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
                    if ((*AIDATA::_ai_player_info)[player_idx].n_reachable_players == 0) {
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
            if ((*AIDATA::_ai_player_info)[player_idx].n_reachable_players == 0) {
                AIBUILD::_player_total_strength_vs_ships[player_idx][2] = diff / 2;
            }
        }

        int current_need = AIBUILD::_player_total_strength_vs_ships[player_idx][2];
        if (current_need == 0 || MOX::_player[player_idx].command_points_used < MOX::_player[player_idx].command_points) {
            if ((*AIDATA::_ai_player_info)[player_idx].n_reachable_players > 0) {
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

    void __cdecl Empire_Buy_Outright_(int16_t player_idx) {
        s_player* player = &MOX::_player[player_idx];
        int16_t total_maintenance = *reinterpret_cast<int16_t*>(&player->total_maintenance);
        
        int32_t limit = (player->bc * 3) / 4;
        if (limit > 1500) {
            limit = 1500;
        } else if (limit < 50) {
            limit = 50;
        }

        int16_t chosen_colony;
        do {
            bool has_threatened = false;
            uint16_t max_weight_threatened = 0;
            uint16_t max_weight_safe = 0;
            uint16_t weights[250];
            memset(weights, 0, sizeof(weights));

            for (int16_t i = 0; i < (int16_t)MOX::_NUM_COLONIES; ++i) {
                s_colony* colony = &MOX::_colony[i];
                
                if (colony->owner != (int8_t)player_idx) continue;
                if (colony->bought_outright != 0) continue;

                int16_t planet_idx = colony->planet;
                int8_t star_idx = MOX::_planet[planet_idx].star_index;
                if ((MOX::_star[star_idx].blockaded >> (player_idx & 0x1F)) & 1) continue;

                int16_t producing = colony->producing[0];
                if (producing >= -3 && producing <= -1) continue;

                int32_t cost = COLCALC::Colony_Cost_To_Buy_Producing_Product_(colony);
                if (cost <= 0) continue;

                if ((int32_t)total_maintenance + cost >= player->bc) continue;

                s_ai_colony_info* ai_info = &(*AIDATA::_ai_colony_info)[i];
                if (ai_info->threatened == 0) {
                    if (player->bc - (int32_t)total_maintenance - cost < limit) continue;
                }

                int32_t turns = (int32_t)COLCALC::Colony_N_Turns_To_Produce_(colony);
                if (turns < 0 || turns > 50) {
                    turns = 50;
                }

                uint32_t weight = ((turns * turns * 1000) / (cost * cost)) + 1;
                if (weight > 0xFFFF) {
                    weight = 0xFFFF;
                }
                weights[i] = (uint16_t)weight;

                if (ai_info->threatened != 0) {
                    has_threatened = true;
                    if (weights[i] > max_weight_threatened) {
                        max_weight_threatened = weights[i];
                    }
                } else {
                    if (weights[i] > max_weight_safe) {
                        max_weight_safe = weights[i];
                    }
                }
            }

            if (max_weight_safe != 0 || max_weight_threatened != 0) {
                for (int16_t i = 0; i < (int16_t)MOX::_NUM_COLONIES; ++i) {
                    if (weights[i] == 0) continue;

                    if (has_threatened) {
                        s_ai_colony_info* ai_info = &(*AIDATA::_ai_colony_info)[i];
                        if (ai_info->threatened == 0 || (uint32_t)weights[i] * 4 < (uint32_t)max_weight_threatened * 3) {
                            weights[i] = 0;
                        }
                    } else {
                        if ((uint32_t)weights[i] * 4 < (uint32_t)max_weight_safe * 3) {
                            weights[i] = 0;
                        }
                    }
                }
            }

            chosen_colony = RUSS::Get_Weighted_Choice_Short_(weights, (int16_t)MOX::_NUM_COLONIES);
            if (chosen_colony != -1) {
                COLCALC::Colony_Buy_Outright_(chosen_colony);
            }

            RUSS::Mox_Update_();

        } while (chosen_colony != -1);
    }

    void __cdecl Assign_Colony_Building_(s_colony* colony) {
        if (colony->outpost_flag == 0 && colony->n_pops != 0) {
            int16_t current_production = colony->producing[0];

            // Check if producing specific special projects (values -38, or -3 to -1)
            if (current_production == -38 || (current_production >= -3 && current_production <= -1)) {
                Assign_Colony_New_Building_(colony);
            }
        }
    }

    void __cdecl Find_Buildable_Buildings_(s_colony* colony, uint16_t* buildable_flags) {
        for (int16_t product_id = 1; product_id < 0x31; ++product_id) {
            // Check if the specific colony can build the product identified by product_id
            if (COLCALC::Colony_Can_Build_Product_(colony, product_id)) {
                buildable_flags[product_id] = 1;
            }
        }
    }

    short __cdecl Colony_Building_Score_(s_colony* colony, short building_id) {
        int owner = (uint8_t)colony->owner;
        s_player* player = &MOX::_player[owner];
        int colony_idx = (int)(colony - MOX::_colony);
        s_ai_colony_info* ai_colony = &(*AIDATA::_ai_colony_info)[colony_idx];

        int obj_5 = (player->objectives == 5);
        int obj_4 = (player->objectives == 4);
        int obj_1 = (player->objectives == 1);
        int obj_3 = (player->objectives == 3);

        int bc_score = 0;
        if (player->bc >= 1500) {
            bc_score = (int)isqrt_((uint32_t)((player->surplus_bc + (player->surplus_bc < 0 ? 63 : 0)) >> 6));
        }
        if (bc_score > 10) bc_score = 10;

        s_planet_data* planet = &MOX::_planet[colony->planet];
        int star_idx = (uint8_t)planet->star_index;
        s_ai_star_info* ai_star = &(*AIDATA::_ai_star_info)[star_idx];
        s_ai_star_player_block* ps_star_player = &ai_star->per_player[owner];

        int trait_score = (uint8_t)ai_colony->trait_tolerant + (uint8_t)ai_colony->trait_environment_immune;
        
        bool is_industrial_focus = false;
        if (colony->auto_building == 0 && player->tech_applications[22 /* TECH_APP_AUTOMATED_FACTORIES */] == 3 && planet->mineral_class <= 2) {
            is_industrial_focus = true;
        }

        bool is_military_focus = false;
        int ground_trait = player->traits[0 /* TRAIT_GROUND_COMBAT */] / 2;
        if (ground_trait >= 0 && ground_trait <= 1) {
            if ((colony->buildings[22 /* BUILDING_MARINE_BARRACKS */] == 0 && player->tech_applications[103 /* TECH_APP_MARINE_BARRACKS */] == 3) ||
                (colony->buildings[2 /* BUILDING_ARMOR_BARRACKS */] == 0 && player->tech_applications[14 /* TECH_APP_ARMOR_BARRACKS */] == 3)) {
                is_military_focus = true;
            }
        }

        bool building_focus = is_industrial_focus || is_military_focus;
        int score = 0;

        switch (building_id) {
        case 2: {
            int threatened_score = ps_star_player->alliance_count * 3 + ai_colony->threatened * 2 + 
                                   ps_star_player->out_of_range_count + ps_star_player->neutral_count + ps_star_player->enemy_count;
            if (colony->n_pops > 2 || bc_score != 0) {
                if (threatened_score != 0) score = threatened_score + obj_1;
            }
            if (ground_trait >= 0 && ground_trait <= 1 && colony->buildings[22 /* BUILDING_MARINE_BARRACKS */] == 0) {
                score += 6;
            }
            if (Colony_Is_Conquered_(colony)) score += 1;
            bc_score /= 2;
            score += bc_score;
        } break;

        case 3:
            if (building_focus && ai_colony->threatened == 0) break;
            if (((MOX::_star[star_idx].has_warp_field_interdictor >> (owner & 0x1F)) & 1) == 0) {
                int threatened_score = ps_star_player->alliance_count * 3 + ai_colony->threatened * 10 + 
                                       ps_star_player->out_of_range_count + ps_star_player->neutral_count + ps_star_player->enemy_count;
                if (threatened_score != 0) score = threatened_score + obj_1;
            }
            bc_score /= 2;
            score += bc_score;
            break;

        case 4:
            score = 5;
            break;

        case 5:
            if (trait_score != 0 && colony->pollution > 5) {
                score = obj_5;
                if (colony->pollution >= 11) {
                    score = (int)isqrt_((uint32_t)colony->pollution) + bc_score;
                }
            }
            break;

        case 6:
            if (!building_focus && player->do_not_research == 0) {
                score = obj_3 * 4 + 11;
            }
            break;

        case 7:
            score = (uint8_t)colony->n_pops + 13 + obj_4 * 2;
            break;

        case 8: {
            if (building_focus && ai_colony->threatened == 0) break;
            int base_threat = (ps_star_player->out_of_range_count + (ps_star_player->neutral_count + ps_star_player->alliance_count * 2) * 2 + ps_star_player->enemy_count) * 3 + ai_colony->threatened * 10;
            if (player->command_points <= player->command_points_used) {
                base_threat += (player->command_points_used - player->command_points) + 1;
            }
            if (base_threat != 0) score = base_threat + obj_5 + bc_score;
        } break;

        case 9:
            if (ground_trait != 3 && player->home_planet_id == colony->planet) {
                score = 100;
            }
            break;

        case 10:
            if (!building_focus) {
                if (player->traits[5 /* TRAIT_POP_GROWTH_BONUS */] < 0) score = obj_5;
                bc_score /= 2;
                score += bc_score;
            }
            break;

        case 12:
            score = (uint8_t)colony->n_pops + 12 + obj_4 * 4;
            break;

        case 13:
            if (!building_focus && trait_score != 0 && colony->pollution > 5) {
                score = obj_5;
                if (colony->pollution >= 11) {
                    score = (int)isqrt_((uint32_t)colony->pollution) + bc_score;
                }
            }
            break;

        case 15:
            if (!building_focus) score = obj_5 + 18;
            break;

        case 16:
            if (ai_colony->trait_eats_minerals != 0) {
                score = (player->surplus_food < 0) ? 8 : 4;
                score += obj_5 + bc_score;
            }
            break;

        case 17:
            score = bc_score + obj_5;
            break;

        case 19:
            if (!building_focus && player->do_not_research == 0) score = 11;
            break;

        case 20:
            if (ground_trait != 3 && (colony->n_pops > 2 || (bc_score != 0 && colony->n_pops > 1))) {
                score = 10;
            }
            break;

        case 21:
            if (ai_colony->trait_eats_minerals != 0) {
                int base_val = 6;
                if (colony->food2_per_farmer == 0) base_val = 12;
                else if (colony->food2_per_farmer == 1) base_val = 11;
                else if (colony->food2_per_farmer == 2) base_val = 10;

                if (player->surplus_food < 0) base_val -= (int)player->surplus_food;
                score = base_val + obj_5 * 4;
            }
            break;

        case 22: {
            int threatened_score = ai_colony->threatened * 5 + (ps_star_player->alliance_count * 3 + ps_star_player->out_of_range_count) * 2 + ps_star_player->neutral_count * 3 + ps_star_player->enemy_count;
            if (colony->n_pops > 1 || bc_score != 0) {
                if (threatened_score != 0) score = threatened_score + obj_1;
            }
            if (colony->buildings[2 /* BUILDING_ARMOR_BARRACKS */] == 0 && ground_trait >= 0 && ground_trait <= 1) {
                score += 12;
            }
            if (Colony_Is_Conquered_(colony)) score += 3;
            score += bc_score;
        } break;

        case 23:
        case 24: {
            if (building_focus && ai_colony->threatened == 0) break;
            int threatened_score = ps_star_player->enemy_count + (ai_colony->threatened * 5 + (ps_star_player->alliance_count * 3 + ps_star_player->neutral_count) * 2 + ps_star_player->out_of_range_count) * 2;
            if (threatened_score != 0) score = threatened_score + obj_1;
            if (planet->climate == 1) score += obj_5 * 2;
            score += bc_score;
        } break;

        case 25: {
            int grav = planet->gravity_class;
            if (player->traits[10 /* TRAIT_HEAVY_G_WORLD */] != 0) {
                if (grav == 0) score = obj_5 + 3;
            } else if (player->traits[9 /* TRAIT_LOW_G_WORLD */] != 0) {
                if (grav == 1) score = obj_5 + 3;
                else if (grav == 2) score = obj_5 + 6;
            } else {
                if (grav == 0) score = obj_5 + 3;
                else if (grav == 2) score = obj_5 + 6;
            }
        } break;

        case 26:
        case 27: {
            if (building_focus && ai_colony->threatened == 0) break;
            int base_threat = ai_colony->threatened * 10 + (ps_star_player->out_of_range_count + (ps_star_player->neutral_count + ps_star_player->alliance_count * 2) * 2 + ps_star_player->enemy_count) * 4;
            if (base_threat != 0) score = base_threat + obj_5 + bc_score;
        } break;

        case 28: {
            if (building_focus && ai_colony->threatened == 0) break;
            int threatened_score = ai_colony->threatened * 10 + (ps_star_player->alliance_count * 3 + ps_star_player->neutral_count * 2 + ps_star_player->out_of_range_count + ps_star_player->enemy_count) * 4;
            if (threatened_score != 0) score = threatened_score + obj_1;
            if (planet->climate == 1) score += obj_5 * 2;
            score += bc_score;
        } break;

        case 29:
            if (!building_focus && colony->n_pops > 4) {
                score = (ai_colony->pop_limit + (uint8_t)colony->n_pops + obj_4) / 3;
            }
            break;

        case 30:
            if (!building_focus && player->do_not_research == 0) {
                score = obj_3 * 3 + 8;
            }
            break;

        case 31:
            if (ground_trait != 3 && (colony->n_pops > 2 || (bc_score != 0 && colony->n_pops > 1))) {
                score = 16;
            }
            break;

        case 32:
            if (trait_score != 0 && colony->pollution > 5) {
                score = obj_5;
                if (colony->pollution >= 11) {
                    score = (int)isqrt_((uint32_t)colony->pollution) + bc_score;
                }
            }
            break;

        case 33:
            score = (ai_colony->pop_limit + (uint8_t)colony->n_pops * 2) / 3 + (trait_score + obj_4 + obj_5) * 2;
            break;

        case 34:
            score = obj_4 * 2 + 12;
            break;

        case 35:
            if (!building_focus && player->do_not_research == 0) {
                score = obj_3 * 2 + 5;
            }
            break;

        case 36:
            score = (uint8_t)colony->n_pops + 5 + obj_4 * 2;
            break;

        case 37:
            if (!building_focus && ai_colony->trait_eats_minerals != 0 && colony->food2_per_farmer != 0) {
                int base_val = (player->surplus_food >= 0) ? 3 : 5;
                score = base_val + obj_5 * 2;
            }
            break;

        case 38:
            if (!building_focus && (colony->production[1] > 16 || colony->n_pops > 4 || bc_score != 0)) {
                score = (int)isqrt_((uint32_t)((int)colony->production[1] - 15));
            }
            break;

        case 39:
            if (!building_focus && colony->n_pops > 2) {
                score = (ai_colony->pop_limit + (uint8_t)colony->n_pops + obj_4) / 3;
            }
            break;

        case 40:
        case 41: {
            if (building_focus && ai_colony->threatened == 0) break;
            int base_threat = (building_id == 40) ?
                ps_star_player->out_of_range_count * 3 + (ps_star_player->enemy_count + (ps_star_player->neutral_count + ps_star_player->alliance_count * 2) * 2) * 4 + ai_colony->threatened * 10 :
                (ps_star_player->out_of_range_count + (ps_star_player->neutral_count + ps_star_player->alliance_count * 2) * 2 + ps_star_player->enemy_count) * 3 + ai_colony->threatened * 10;
            
            if (player->command_points <= player->command_points_used) {
                base_threat += (player->command_points_used - player->command_points) + 1;
            }
            if (base_threat != 0) score = base_threat + obj_5 + bc_score;
        } break;

        case 42: {
            if (building_focus && ai_colony->threatened == 0) break;
            int threatened_score = ai_colony->threatened * 10 + (ps_star_player->out_of_range_count + (ps_star_player->neutral_count + ps_star_player->alliance_count * 2) * 2 + ps_star_player->enemy_count) * 4;
            if (threatened_score != 0) score = threatened_score + obj_5 + bc_score;
        } break;

        case 43:
            if (!building_focus && ai_colony->trait_eats_minerals != 0) {
                int base_val = 7;
                if (colony->food2_per_farmer == 0) base_val = 13;
                else if (colony->food2_per_farmer == 1) base_val = 12;
                else if (colony->food2_per_farmer == 2) base_val = 10;

                if (player->surplus_food < 0) base_val -= (int)player->surplus_food;
                score = base_val + obj_5 * 3 + bc_score;
            }
            break;

        case 44:
            if (!building_focus) {
                int base_climate_score = 0;
                bool valid_climate = true;
                bool is_aquatic = (player->traits[11 /* TRAIT_AQUATIC */] != 0);
                switch (planet->climate) {
                case 2: base_climate_score = 2; break;
                case 3: base_climate_score = 1; break;
                case 4: if (is_aquatic) base_climate_score = 1; else valid_climate = false; break;
                case 5: if (!is_aquatic) base_climate_score = 4; else valid_climate = false; break;
                case 6: if (!is_aquatic) base_climate_score = 6; else valid_climate = false; break;
                case 7: base_climate_score = 1; break;
                default: valid_climate = false; break;
                }
                if (valid_climate) score = base_climate_score + obj_5 * 3 + bc_score;
            }
            break;

        case 45: {
            if (building_focus && ai_colony->threatened == 0) break;
            if (((MOX::_star[star_idx].has_warp_field_interdictor >> (owner & 0x1F)) & 1) == 0) {
                int threatened_score = ai_colony->threatened * 5 + (ps_star_player->enemy_count + ps_star_player->alliance_count * 2) * 2 + ps_star_player->neutral_count * 3 + ps_star_player->out_of_range_count;
                if (threatened_score != 0) score = threatened_score + obj_1;
            }
            bc_score /= 2;
            score += bc_score;
        } break;

        case 46:
            if (!building_focus && ai_colony->trait_eats_minerals != 0 && colony->food2_per_farmer != 0) {
                int base_val = (player->surplus_food >= 0) ? 5 : 10;
                score = base_val + obj_5 * 2;
            }
            break;

        case 47: {
            if (building_focus && ai_colony->threatened == 0) break;
            int threatened_score = ai_colony->threatened * 10 + (ps_star_player->out_of_range_count + (ps_star_player->neutral_count + ps_star_player->alliance_count * 2) * 2 + ps_star_player->enemy_count) * 4;
            if (threatened_score != 0) score = threatened_score + obj_5 + bc_score;
        } break;
        }

        if (score > 1000) score = 1000;
        if (score <= 0) return 0;
        return (short)score;
    }

    /**
     * Checks if a colony contains population units belonging to a non-eliminated player
     * who has an active high-level treaty (Research, Non-Aggression, or Alliance)
     * with the current colony owner.
     */
    bool __cdecl Colony_Is_Conquered_(s_colony* colony) {
        for (uint8_t i = 0; i < (uint8_t)colony->n_pops; ++i) {
            uint32_t pop_data = colony->pop[i];
            uint32_t pop_owner = pop_data & 0x0F;

            // Only check valid players (0-7) excluding the current colony owner
            if (pop_owner < 8 && (int8_t)pop_owner != colony->owner) {
                // Ensure the population owner is still in the game
                if (MOX::_player[pop_owner].eliminated == 0) {
                    int8_t treaty_status = MOX::_player[pop_owner].treaty[(int)colony->owner];

                    // Treaty range 4 to 6 likely corresponds to Research, Non-Aggression, and Alliance
                    if (treaty_status >= 4 && treaty_status <= 6) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    void __cdecl Assign_Colony_New_Building_(s_colony* colony) {
        uint16_t weights[52];
        short max_score = 0;

        memset(weights, 0, 102);

        Find_Buildable_Buildings_(colony, weights);

        for (int i = 4; i <= 48; ++i) {
            if (weights[i] > 0) {
                short score = Colony_Building_Score_(colony, i);
                weights[i] = score;
                if (score > max_score) {
                    max_score = score;
                }
            }
        }

        weights[50] = 1;

        if (max_score < 1) {
            max_score = 1;
        }

        int difficulty = MOX::_settings.game_difficulty;
        if (MOX::_player[static_cast<int>(colony->owner)].objectives == 100 /* ECON_INDUSTRY */) {
            difficulty = 4;
        }

        int threshold_multiplier = 6 - difficulty;

        for (int i = 0; i <= 50; ++i) {
            if (threshold_multiplier * weights[i] < max_score) {
                weights[i] = 0;
            }
        }

        int colony_idx = (int)(colony - MOX::_colony);
        int pop_limit = (*AIDATA::_ai_colony_info)[colony_idx].pop_limit;

        if (colony->n_pops * 3 <= pop_limit) {
            int housing_weight_base = (pop_limit - colony->n_pops) * 3;
            weights[49] = (short)isqrt_(housing_weight_base);
        }

        short choice = RUSS::Get_Weighted_Choice_Short_(weights, 51);

        if (choice < 49) {
            Delete_Prod_From_Colony_Queue_(colony_idx, choice);
            colony->producing[0] = choice;
        } else if (choice == 49) {
            colony->producing[0] = -3; // Housing
        } else if (choice == 50) {
            colony->producing[0] = -2; // Trade Goods
        }
    }

    void __cdecl Force_Produce_(uint8_t* id_list, int16_t index, int32_t* list_count, int16_t production_id) {
        // Retrieve the colony index from the provided list at the specified index
        uint8_t colony_idx = id_list[index];

        // Set the primary production item for the colony
        MOX::_colony[colony_idx].producing[0] = production_id;

        // Decrement the list counter
        *list_count -= 1;

        // Calculate the number of items to move to fill the gap in the list
        int32_t items_to_move = *list_count - index;

        // Shift the list left to remove the entry at the specified index
        memcpy(&id_list[index], &id_list[index + 1], items_to_move);
    }

    int16_t __cdecl Industry_Worth(s_colony* colony) {
        return (int16_t)(((uint16_t)colony->n_pops >> 3) - (uint16_t)colony->maintenance[1] + colony->production[1]);
    }

    /**
     * @brief Returns the name of the product being built.
     * @param product_id The ID of the building or project.
     * @return A pointer to the product name string.
     */
    char* __cdecl Product_Name_(int16_t product_id) {
        // 0xffdb is -37, which represents "Trade Goods" in AI contexts
        if (product_id == -37 /* 0xffdb */) {
            return s_AI_trade_goods_00557670;
        }

        return COLBLDG::Selection_Name_(product_id);
    }
}
