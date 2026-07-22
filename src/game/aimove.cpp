#include "pch.h"

namespace AIMOVE {
    s_player* _p_ai;
    s_ai_colonizable_planet* _colonizable;
    int8_t (*_nonplayer_here)[MAX_STARS];
    int8_t* _flee_stars;
    int16_t _i_ai;
    int16_t _n_colonizable;

    int __cdecl Sort_colonizable_(const void* a, const void* b) {
        auto p1 = (const s_ai_colonizable_planet*)a;
        auto p2 = (const s_ai_colonizable_planet*)b;
        return p1->contextual_value - p2->contextual_value;
    }

    void __cdecl Move_All_AI_() {
        for (int32_t i = 0; i < (int32_t)MOX::_NUM_PLAYERS; ++i) {
            s_player& plr = MOX::_player[i];

            if (plr.objectives != PLAYER_OBJECTIVE_HUMAN && plr.ship_speed != 0) {
                RUSS::Mox_Update_();

                if (plr.traits[TRAIT_TRANS_DIMENSIONAL] != 0 || EVENTS::Event_Check_Hyperspace_Flux_() == 0) {
                    Move_AI_(i);
                }
            }
        }

        Move_NPCs_();
    }

    void __cdecl Launch_Sneak_Attack_() {
        if (_p_ai->sneak_attack_planet != -1) {
            if (AIBUILD::Player_War_Level_(_i_ai) != 0) {
                _p_ai->sneak_attack_planet = -1;
            }
        }

        if (_p_ai->sneak_attack_planet != -1) {
            int16_t target_planet_idx = _p_ai->sneak_attack_planet;
            int16_t colony_idx = MOX::_planet[target_planet_idx].colony_index;
            uint8_t target_player = (uint8_t)_p_ai->sneak_attack_player;

            if (colony_idx != -1 && MOX::_colony[colony_idx].owner == target_player) {
                int8_t stage_point = MOX::_player[_i_ai].offensive_stagepoint;

                if ((*AIDATA::_i_all_combat_ships)[_i_ai][stage_point] != -1) {
                    int16_t ship_list[SHIP_LIST_CAPACITY];
                    s_ship_move_info move_info;
                    int16_t star_idx = MOX::_planet[target_planet_idx].star_index;

                    int ship_count = Gather_Player_Ships_At_Star_(_i_ai, (int16_t)stage_point, ship_list);
                    SHIPMOVE::Ships_Try_To_Move_To_(ship_list, star_idx, &move_info);

                    if (move_info.moving != 0) {
                        for (int i = 0; i < ship_count; ++i) {
                            MOX::_ship[ship_list[i]].mission = 6;
                        }
                        SHIPMOVE::Make_Ships_Move_To_(ship_list, star_idx, &move_info);
                        (*AIDATA::_i_all_combat_ships)[_i_ai][stage_point] = -1;
                    }
                }
            }

            int16_t star_idx = MOX::_planet[_p_ai->sneak_attack_planet].star_index;
            if ((*AIDATA::_enroute_ship_strengths)[star_idx][_i_ai][target_player][1] != 0) {
                diplomac::Declare_War_(_i_ai, (int16_t)target_player);
            }
        }
    }

    void __cdecl Move_AI_(int16_t player_idx) {
        int8_t nonplayer_presence_storage[MAX_STARS];
        int8_t flee_star_storage[MAX_STARS * ENEMY_STRENGTH_TURN_WINDOW];
        s_ai_colonizable_planet colonizable_planet_storage[MAX_PLANETS];

        RUSS::Mox_Update_();

        _i_ai = player_idx;
        _colonizable = colonizable_planet_storage;
        _p_ai = &MOX::_player[player_idx];

        Get_Colonizable_Planets_(colonizable_planet_storage, &_n_colonizable);

        qsort(_colonizable, _n_colonizable, 6, Sort_colonizable_);

        _flee_stars = flee_star_storage;
        memset(_flee_stars, 0, sizeof(flee_star_storage));

        Mark_Flee_Stars_(player_idx);

        _nonplayer_here = (int8_t(*)[MAX_STARS])nonplayer_presence_storage;
        memset(_nonplayer_here, 0, sizeof(nonplayer_presence_storage));

        s_ship_data* ship = MOX::_ship;
        if (ship < MOX::_ship + MOX::_NUM_SHIPS) {
            do {
                if (ship->owner >= MAX_PLAYERS && ship->status == 0) {
                    (*_nonplayer_here)[ship->location] = ship->owner;
                }
                ship++;
            } while (ship < MOX::_ship + MOX::_NUM_SHIPS);
        }

        RUSS::Mox_Update_();
        Delete_Already_Assigned_Planets_(_colonizable, &_n_colonizable);

        RUSS::Mox_Update_();
        Launch_Sneak_Attack_();

        RUSS::Mox_Update_();
        Launch_Attacks_(6, 2);

        RUSS::Mox_Update_();
        Intercept_Invaders_();

        RUSS::Mox_Update_();
        Launch_Attacks_(1, 2);

        RUSS::Mox_Update_();
        Assign_New_Colony_Ship_Destinations_(_colonizable, &_n_colonizable);

        RUSS::Mox_Update_();
        Launch_Attacks_(2, 2);

        RUSS::Mox_Update_();
        Launch_Attacks_(6, 0);

        RUSS::Mox_Update_();
        Launch_Attacks_(1, 0);

        RUSS::Mox_Update_();
        Launch_Attacks_(2, 0);

        RUSS::Mox_Update_();
        Move_Transports_();

        RUSS::Mox_Update_();
        Move_To_Stage_Points_();

        RUSS::Mox_Update_();
        if ((*AIDATA::_ai_player_info)[_i_ai].prev_n_contacts ==
            (*AIDATA::_ai_player_info)[_i_ai].n_contacts ||
            MOX::_settings.game_difficulty == 0) {
            Move_To_Unexplored_Stars_();
        }

        _colonizable = nullptr;
        _flee_stars = nullptr;
        _nonplayer_here = nullptr;
    }

    void __cdecl Player_Threatens_Player_(int plr1, int plr2, int* star_idx, int* aux_info, int* planet_idx) {
        *planet_idx = -1;

        // Check if player is Trans-Dimensional or if Hyperspace Flux is active
        if (MOX::_player[plr1].traits[TRAIT_TRANS_DIMENSIONAL] == 0) {
            if (EVENTS::Event_Check_Hyperspace_Flux_() != 0) {
                return;
            }
        }

        if (MOX::_NUM_STARS < 1) {
            return;
        }

        for (int i = 0; i < (int)MOX::_NUM_STARS; ++i) {
            uint32_t current_strength = (uint32_t)(*AIDATA::_ai_player_info)[plr1].total_ship_strength[plr2];
            uint32_t possible_strength = (uint32_t)(*AIDATA::_possible_ship_strengths)[i][plr1][plr2][0];

            // AI logic: check if existing strength ratio allows for a threat/attack opportunity
            if ((current_strength >> 2) <= (possible_strength / 10)) {
                int16_t found_planet;
                int16_t dummy_short;
                int32_t found_aux;

                AIMOVE::Find_Opportunity_Attack_Aux_((int16_t)plr1, (int16_t)plr2, 150 /* 0x96 */, (int16_t)i, &found_planet, &dummy_short, &found_aux);

                if (found_planet != -1) {
                    *star_idx = i;
                    *aux_info = found_aux;
                    *planet_idx = (int)found_planet;
                    return;
                }
            }
        }
    }

    int16_t __cdecl Best_Uncolonized_Undestined_Planet_(int16_t player_idx) {
        s_ai_colonizable_planet colonizable_planets[MAX_PLANETS];
        int16_t best_planet_idx = -1;

        _i_ai = player_idx;
        _colonizable = colonizable_planets;
        _p_ai = &MOX::_player[player_idx];

        Get_Colonizable_Planets_(colonizable_planets, &_n_colonizable);

        qsort(_colonizable, (uint32_t)_n_colonizable, sizeof(s_ai_colonizable_planet), Sort_colonizable_);

        Delete_Already_Assigned_Planets_(_colonizable, &_n_colonizable);

        if (_n_colonizable > 0) {
            best_planet_idx = _colonizable[_n_colonizable - 1].planet_idx;
        }

        _colonizable = nullptr;
        return best_planet_idx;
    }

    void __cdecl Move_To_Unexplored_Stars_() {
        int8_t star_flags_0[MAX_STARS];
        int32_t enemy_strength_1[ENEMY_STRENGTH_TURN_WINDOW];
        int32_t enemy_strength_2[ENEMY_STRENGTH_TURN_WINDOW];
        int16_t ship_indices[MAX_SHIPS];
        s_ship_move_info move_info;
        int16_t single_ship_list[2];

        memset(star_flags_0, 0, sizeof(star_flags_0));

        int32_t num_ships_selected = 0;
        if (MOX::_NUM_SHIPS > 0) {
            for (int32_t i = 0; i < (int32_t)MOX::_NUM_SHIPS; ++i) {
                if (MOX::_ship[i].owner == _i_ai) {
                    int8_t status = MOX::_ship[i].status;
                    int16_t loc = MOX::_ship[i].location;

                    if (status == 0) {
                        if ((MOX::_ship[i].mission == 0 || MOX::_ship[i].mission == 3) &&
                            MOX::_ship[i].d.ship_type == SHIP_TYPE_COMBAT) {
                            ship_indices[num_ships_selected] = (int16_t)i;
                            num_ships_selected++;
                        }
                        star_flags_0[loc] = 1;
                    } else if (status == 1) {
                        star_flags_0[loc - SHIP_LOCATION_MOVING_OFFSET] = 1;
                    } else if (status == 2) {
                        star_flags_0[loc - SHIP_LOCATION_WORMHOLE_OFFSET] = 1;
                    }
                }
            }
        }

        if (MOX::_NUM_STARS > 0) {
            for (int32_t i = 0; i < (int32_t)MOX::_NUM_STARS; ++i) {
                if ((((*AIDATA::_star_in_extended_range_of_player)[i] >> (uint8_t)_i_ai) & 1) != 0 &&
                    ((MOX::_star[i].visited >> (uint8_t)_i_ai) & 1) == 0 &&
                    star_flags_0[i] == 0 &&
                    (MOX::_settings.game_difficulty != 0 || MOX::_star[i].has_colony == 0)) {
                    
                    int32_t best_ship_idx = -1;
                    int32_t best_star = -1;
                    int32_t min_turns = 255;
                    
                    Get_Enemy_Strength_On_Turn_((int16_t)i, enemy_strength_1, enemy_strength_2);
                    
                    if (num_ships_selected == 0) {
                        return;
                    }
                    
                    for (int32_t j = 0; j < num_ships_selected; ++j) {
                        int16_t ship_idx = ship_indices[j];
                        bool has_device = struct_::Test_Bit_Field_(MOX::_ship[ship_idx].d.special_device_flags, 11);
                        
                        if (has_device || (((*AIDATA::_star_in_range_of_player)[i] >> (uint8_t)_i_ai) & 1) != 0) {
                            int16_t loc = MOX::_ship[ship_idx].location;
                            uint8_t turns = (*AIDATA::_n_turns_from_to)[loc][i][_i_ai];
                            
                            if (turns < min_turns && turns < ENEMY_STRENGTH_TURN_WINDOW && enemy_strength_2[turns] == 0) {
                                min_turns = turns;
                                best_ship_idx = j;
                                best_star = (int32_t)i;
                            }
                        }
                    }
                    
                    if (best_ship_idx != -1) {
                        single_ship_list[0] = ship_indices[best_ship_idx];
                        single_ship_list[1] = -1;
                        num_ships_selected--;
                        ship_indices[best_ship_idx] = ship_indices[num_ships_selected];
                        
                        SHIPMOVE::Ships_Try_To_Move_To_(single_ship_list, (int16_t)best_star, &move_info);
                        MOX::_ship[single_ship_list[0]].mission = 7;
                        SHIPMOVE::Make_Ships_Move_To_(single_ship_list, (int16_t)best_star, &move_info);
                    }
                }
            }
        }
    }

    void __cdecl Launch_Attacks_(int mission_type, int min_pop) {
        if (MOX::_NUM_STARS > 0) {
            for (int16_t star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
                if ((*AIDATA::_i_all_combat_ships)[AIMOVE::_i_ai][star_idx] != -1) {
                    AIMOVE::Find_Best_Attack_From_Star_(star_idx, nullptr, mission_type, min_pop);
                }
            }
        }
    }

    void __cdecl Mark_Flee_Stars_(int16_t player_idx) {
        // Iterate through all players to check for active blockading durations.
        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            // Check if player i is currently being asked to stop blockading by player_idx.
            if (MOX::_player[player_idx].stop_blockading_duration[i] != 0) {
                // If so, iterate through stars to identify retreat/flee targets.
                for (int star_idx = 0; star_idx < (int)MOX::_NUM_STARS; ++star_idx) {
                    // Each star entry in flee_stars is 10 bytes wide.
                    int8_t* star_flee_ptr = &_flee_stars[star_idx * ENEMY_STRENGTH_TURN_WINDOW];

                    // Only mark the star if it hasn't been processed yet.
                    if (star_flee_ptr[0] == 0) {
                        // Check if player i has a colony at this star (has_colony is a bitmask).
                        if ((MOX::_star[star_idx].has_colony >> i) & 1) {
                            // Mark all 10 bytes for this star as fleeing locations.
                            memset(star_flee_ptr, 1, ENEMY_STRENGTH_TURN_WINDOW);
                        }
                    }
                }
            }
        }
    }

    int __cdecl Gather_Player_Ships_At_Star_(int16_t player_id, int16_t star_id, int16_t* ships_list) {
        int index = (*AIDATA::_i_all_combat_ships)[player_id][star_id];
        int16_t ship_id = (*AIDATA::_all_ships)[index];

        ships_list[0] = ship_id;
        int count = 1;

        while (true) {
            index++;
            if (index >= (int)AIDATA::_n_all_ships) {
                break;
            }

            ship_id = (*AIDATA::_all_ships)[index];
            s_ship_data* ship = &MOX::_ship[ship_id];

            if ((int16_t)ship->owner != player_id || ship->location != star_id) {
                break;
            }

            // Skip ships on mission 5 (potentially 'In transit') unless we are at an offensive stagepoint
            if ((int16_t)_p_ai->offensive_stagepoint == star_id || ship->mission != 5) {
                ships_list[count++] = ship_id;
            }
        }

        ships_list[count] = -1; // End of list marker
        return count;
    }

    void __cdecl Get_Colonizable_Planets_(s_ai_colonizable_planet* colonizable_planets, int16_t* count) {
        *count = 0;
        s_planet_data* planet = MOX::_planet;
        s_planet_data* end = planet + (int16_t)MOX::_planet_count;
        int16_t i = 0;

        while (planet < end) {

            // Only consider uncolonized planets. 
            // PLANET_TYPE_PLANET is a standard colonizable planet (excluding gas giants, asteroids).
            if (planet->colony_index == -1 && planet->planet_type == PLANET_TYPE_PLANET) {
                // Each byte in the array represents a star, with bits corresponding to players
                // who have that star within their extended travel range.
                int32_t star_idx = (int32_t)planet->star_index;
                uint8_t star_range_mask = (*AIDATA::_star_in_extended_range_of_player)[star_idx];

                if (((1u << ((uint8_t)AIMOVE::_i_ai & 0x1F)) & (uint32_t)star_range_mask) != 0) {
                    int16_t current_count = *count;

                    colonizable_planets[current_count].planet_idx = i;

                    // Lookup contextual value for the planet relative to the current AI player.
                    // The table contains MAX_PLANETS entries per player (2880 total).
                    int contextual_idx = i + AIMOVE::_i_ai * MAX_PLANETS;
                    colonizable_planets[current_count].contextual_value =
                        (int32_t)(uint16_t)(*AIDATA::_contextual_planet_values)[contextual_idx];

                    *count = current_count + 1;
                }
            }

            ++planet;
            ++i;
        }
    }

    void __cdecl Delete_Already_Assigned_Planets_(s_ai_colonizable_planet* colonizable_planets, int16_t* num_planets) {
        if (_p_ai->tech_applications[TECH_APP_HYPERSPACE_COMMUNICATIONS] != TECH_RESEARCH_STATUS_RESEARCHED) {
            for (int i = 0; i < (int)MOX::_NUM_SHIPS; ++i) {
                s_ship_data* ship = &MOX::_ship[i];

                if (ship->owner == _i_ai &&
                    ship->d.ship_type == SHIP_TYPE_COLONY &&
                    ship->status == 1 &&
                    !SHIPMOVE::Can_Order_Ship_(ship))
                {
                    int16_t star_idx = ship->location - SHIP_LOCATION_MOVING_OFFSET;
                    for (int j = *num_planets - 1; j >= 0; --j) {
                        if (MOX::_planet[colonizable_planets[j].planet_idx].star_index == star_idx) {
                            int16_t new_count = *num_planets - 1;
                            *num_planets = new_count;
                            memmove(&colonizable_planets[j], &colonizable_planets[j + 1], (new_count - j) * sizeof(s_ai_colonizable_planet));
                            break;
                        }
                    }
                }
            }
        }

        if (MOX::_player[_i_ai].objectives != PLAYER_OBJECTIVE_HUMAN) {
            for (int i = 0; i < (int)MOX::_NUM_COLONIES; ++i) {
                s_colony* colony = &MOX::_colony[i];

                if (colony->owner == _i_ai && colony->producing[0] == BUILDING_COLONY_BASE) {
                    int16_t colony_star_idx = MOX::_planet[colony->planet].star_index;
                    bool found = false;

                    for (int j = *num_planets - 1; j >= 0; --j) {
                        if (MOX::_planet[colonizable_planets[j].planet_idx].star_index == colony_star_idx) {
                            int16_t new_count = *num_planets - 1;
                            *num_planets = new_count;
                            memmove(&colonizable_planets[j], &colonizable_planets[j + 1], (new_count - j) * sizeof(s_ai_colonizable_planet));
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        colony->producing[0] = COLONY_PRODUCTION_TRADE_GOODS;
                    }
                }
            }
        }
    }

    void __cdecl Get_Enemy_Strength_On_Turn_(int16_t star_id, int* turn_strengths, int* cumulative_strengths) {
        int star_idx = (int)(int16_t)star_id;
        uint8_t in_nebula = MOX::_star[star_idx].in_nebula;

        memset(turn_strengths, 0, sizeof(int[ENEMY_STRENGTH_TURN_WINDOW]));

        s_ship_data* ship = MOX::_ship;
        s_ship_data* ship_end = MOX::_ship + (int16_t)MOX::_NUM_SHIPS;

        while (ship < ship_end) {
            int ship_idx = (int)(ship - MOX::_ship);

            if (ship->owner != _i_ai && ship->status < 3) {
                if (ship->location == star_idx) {
                    uint16_t(*strength_table)[MAX_SHIPS][MAX_PLAYERS] = in_nebula ? AIDATA::_ship_strengths_in_nebula : AIDATA::_ship_strengths;
                    turn_strengths[0] += (*strength_table)[ship_idx][_i_ai];
                } else {
                    int location_diff = (int)(int16_t)ship->location - star_idx;
                    if ((location_diff == SHIP_LOCATION_MOVING_OFFSET || location_diff == SHIP_LOCATION_WORMHOLE_OFFSET) && ship->turns_left < ENEMY_STRENGTH_TURN_WINDOW) {
                        uint16_t(*strength_table)[MAX_SHIPS][MAX_PLAYERS] = in_nebula ? AIDATA::_ship_strengths_in_nebula : AIDATA::_ship_strengths;
                        turn_strengths[ship->turns_left] += (*strength_table)[ship_idx][_i_ai];
                    }
                }
            }

            ship++;
        }

        cumulative_strengths[0] = turn_strengths[0];
        for (int t = 1; t < ENEMY_STRENGTH_TURN_WINDOW; ++t) {
            cumulative_strengths[t] = turn_strengths[t] + cumulative_strengths[t - 1];
        }
    }

    uint32_t __cdecl Ship_Rough_Strength_At_Star_(int16_t ship_idx, int16_t star_idx) {
        uint8_t in_nebula = MOX::_star[star_idx].in_nebula;
        uint32_t min_val1 = 0xffffffff;
        uint32_t min_val2 = 0xffffffff;
        uint32_t sum1 = 0;
        uint32_t sum2 = 0;
        int32_t count1 = 0;

        if (MOX::_NUM_PLAYERS > 0) {
            for (int16_t i = 0; i < MOX::_NUM_PLAYERS; i++) {
                if (_i_ai != i) {
                    if (((*AIDATA::_star_in_extended_range_of_player)[star_idx] >> i) & 1) {
                        if ((((*AIDATA::_star_in_range_of_player)[star_idx] >> i) & 1) ||
                            MOX::_player[i].objectives == PLAYER_OBJECTIVE_HUMAN) {

                            uint16_t strength;
                            if (in_nebula) {
                                strength = (uint16_t)(*AIDATA::_ship_strengths_in_nebula)[ship_idx][i];
                            } else {
                                strength = (uint16_t)(*AIDATA::_ship_strengths)[ship_idx][i];
                            }

                            count1++;
                            sum1 += strength;
                            if (strength < min_val1) {
                                min_val1 = strength;
                            }
                        }
                    }

                    if ((*AIDATA::_ai_player_info)[_i_ai].reachable_players[i] != 0) {
                        uint16_t strength;
                        if (in_nebula) {
                            strength = (uint16_t)(*AIDATA::_ship_strengths_in_nebula)[ship_idx][i];
                        } else {
                            strength = (uint16_t)(*AIDATA::_ship_strengths)[ship_idx][i];
                        }

                        sum2 += strength;
                        if (strength < min_val2) {
                            min_val2 = strength;
                        }
                    }
                }
            }
        }

        if ((*_nonplayer_here)[star_idx] != 0) {
            uint32_t strength;
            if (in_nebula) {
                strength = AIPOWER::Ship_Strength_Vs_Ships_In_Nebula_(ship_idx, (int16_t)(*_nonplayer_here)[star_idx]);
            } else {
                strength = AIPOWER::Ship_Strength_Vs_Ships_(ship_idx, (int16_t)(*_nonplayer_here)[star_idx]);
            }

            strength &= 0xffff;
            if (strength < min_val1) {
                min_val1 = strength;
            }
            return min_val1;
        }

        if (count1 > 0) {
            return (((min_val1 + 2) * (uint32_t)count1) + sum1 - 1) / ((uint32_t)count1 * 2);
        }

        int16_t n_reachable = (*AIDATA::_ai_player_info)[_i_ai].n_reachable_players;
        if (n_reachable > 0) {
            return (((min_val2 + 2) * (uint32_t)n_reachable) + sum2 - 1) / ((uint32_t)n_reachable * 2);
        }

        return (uint32_t)((int32_t)MOX::_ship[ship_idx].d.size + 1);
    }

    void __cdecl Find_Opportunity_Attack_(int16_t player_idx, int16_t target_idx, int16_t threshold, int16_t* out_planet, int16_t* out_value, int32_t* out_ratio) {
        *out_planet = -1;
        if (AIDATA::_i_all_combat_ships != nullptr) {
            if (MOX::_player[player_idx].traits[TRAIT_TRANS_DIMENSIONAL] == 0) {
                if (EVENTS::Event_Check_Hyperspace_Flux_() != 0) {
                    return;
                }
            }
            Find_Opportunity_Attack_Aux_(
                player_idx,
                target_idx,
                threshold,
                (int16_t)MOX::_player[player_idx].offensive_stagepoint,
                out_planet,
                out_value,
                out_ratio
            );
        }
    }

    void __cdecl Move_Ships_With_Possible_Intermediate_(int16_t* ship_array, int16_t dest_star, int16_t* out_star, int16_t* out_turns) {
        int16_t ship = *ship_array;
        int16_t location = MOX::_ship[ship].location;
        int16_t x = MOX::_ship[ship].x;
        int16_t y = MOX::_ship[ship].y;

        *out_star = -1;
        *out_turns = 0x7fff;

        s_ship_move_info move_info;
        SHIPMOVE::Ships_Try_To_Move_To_(ship_array, dest_star, &move_info);

        if (move_info.moving != 0) {
            *out_turns = (uint8_t)move_info.turns_left;
            *out_star = dest_star;
            return;
        }

        int16_t best_eta = 0x7fff;
        int16_t best_star_idx = -1;

        s_colony* colony_ptr = MOX::_colony;
        if (colony_ptr < MOX::_colony + MOX::_NUM_COLONIES) {
            do {
                if (colony_ptr->owner == _i_ai) {
                    int16_t current_star_idx = MOX::_planet[colony_ptr->planet].star_index;

                    if (location != current_star_idx && current_star_idx != dest_star) {
                        SHIPMOVE::Ships_Try_To_Move_To_(ship_array, current_star_idx, &move_info);

                        if (move_info.moving != 0) {
                            // Bug in original game: compares 'moving' flag with 'best_eta' instead of using 'turns_left'
                            if (move_info.moving < best_eta) {
                                int i = 0;
                                int16_t leg1_turns_left = (uint8_t)move_info.turns_left;

                                if (ship_array[0] != -1) {
                                    do {
                                        int16_t ship_idx = ship_array[i];
                                        i++;
                                        MOX::_ship[ship_idx].location = current_star_idx;
                                        MOX::_ship[ship_idx].x = MOX::_star[current_star_idx].x;
                                        MOX::_ship[ship_idx].y = MOX::_star[current_star_idx].y;
                                    } while (ship_array[i] != -1);
                                }

                                SHIPMOVE::Ships_Try_To_Move_To_(ship_array, dest_star, &move_info);

                                if (i > 0) {
                                    do {
                                        i--;
                                        int16_t ship_idx = ship_array[i];
                                        MOX::_ship[ship_idx].location = location;
                                        MOX::_ship[ship_idx].x = x;
                                        MOX::_ship[ship_idx].y = y;
                                    } while (i != 0);
                                }

                                if (move_info.moving != 0) {
                                    leg1_turns_left += (uint8_t)move_info.turns_left;

                                    if (leg1_turns_left < best_eta) {
                                        best_eta = leg1_turns_left;
                                        best_star_idx = current_star_idx;
                                    }
                                }
                            }
                        }
                    }
                }
                colony_ptr++;
            } while (colony_ptr < MOX::_colony + MOX::_NUM_COLONIES);
        }

        if (best_star_idx != -1) {
            if (best_eta < *out_turns) {
                *out_turns = best_eta;
                *out_star = best_star_idx;
            }
        }
    }

    int __cdecl Number_Of_Attacks_Underway_(int16_t player_idx, int16_t target_player_idx) {
        // attacked_stars tracks which stars have an active threat from the player.
        // The array is sized to the game's maximum number of stars.
        uint8_t attacked_stars[MAX_STARS];
        memset(attacked_stars, 0, sizeof(attacked_stars));

        s_ship_data* ship = MOX::_ship;
        s_ship_data* ship_end = MOX::_ship + (int16_t)MOX::_NUM_SHIPS;

        while (ship < ship_end) {
            // Only consider ships owned by the player that are combat ships (type 0)
            if ((int16_t)ship->owner == player_idx && ship->d.ship_type == SHIP_TYPE_COMBAT) {
                int star_idx = -1;

                // Determine the current or destination star index based on ship status.
                // Status 0: At star
                // Status 1: Traveling (location offset by SHIP_LOCATION_MOVING_OFFSET)
                // Status 2: Orbiting/Wormhole (location offset by SHIP_LOCATION_WORMHOLE_OFFSET)
                if (ship->status == 0) {
                    star_idx = (int)ship->location;
                }
                else if (ship->status == 1) {
                    star_idx = (int)ship->location - SHIP_LOCATION_MOVING_OFFSET;
                }
                else if (ship->status == 2) {
                    star_idx = (int)ship->location - SHIP_LOCATION_WORMHOLE_OFFSET;
                }

                if (star_idx != -1) {
                    // Check if the target player has a colony at this star via bitmask.
                    uint8_t has_colony_mask = MOX::_star[star_idx].has_colony;
                    if ((has_colony_mask >> (target_player_idx & 0x1F)) & 1) {
                        attacked_stars[star_idx] = 1;
                    }
                }
            }

            ship++;
        }

        int total_attacks = 0;
        int num_stars = (int16_t)MOX::_NUM_STARS;
        for (int i = 0; i < num_stars; ++i) {
            total_attacks += attacked_stars[i];
        }

        return total_attacks;
    }

    void __cdecl Move_To_Stage_Points_() {
        uint8_t warp_beast_caution;
        if (EVENTS::Event_Check_Warp_Beast_() != 0 && _p_ai->traits[TRAIT_LUCKY] == 0) {
            warp_beast_caution = 1;
        } else {
            warp_beast_caution = 0;
        }

        int32_t total_stage_weight = 0;
        int16_t num_stage_points = 0;
        int16_t num_ships = 0;
        int8_t stage_point_map[MAX_STARS];
        s_ai_stage_point stage_points[MAX_STARS];
        int16_t ship_indices[MAX_SHIPS];

        for (int i = 0; i < MAX_STARS; ++i) {
            stage_point_map[i] = -1;
        }

        for (int16_t star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
            if (_flee_stars[star_idx * 10 + 9] == 0) {
                if ((MOX::_star[star_idx].is_stagepoint >> _i_ai) & 1) {
                    stage_point_map[star_idx] = (int8_t)num_stage_points;
                    stage_points[num_stage_points].star_idx = (uint8_t)star_idx;
                    stage_points[num_stage_points].strength = 0;
                    total_stage_weight += (*AIDATA::_star_stage_weights)[star_idx][_i_ai];
                    num_stage_points++;
                }
            }
        }

        int8_t offensive_stagepoint = _p_ai->offensive_stagepoint;
        int16_t war_level = AIBUILD::Player_War_Level_(_i_ai);
        (*AIDATA::_star_stage_weights)[offensive_stagepoint][_i_ai] += (war_level + 1) * total_stage_weight;

        for (int16_t ship_idx = 0; ship_idx < (int16_t)MOX::_NUM_SHIPS; ++ship_idx) {
            s_ship_data* ship = &MOX::_ship[ship_idx];
            if (ship->owner == _i_ai &&
                ship->status <= 2 &&
                ship->d.ship_type == SHIP_TYPE_COMBAT) {

                if (ship->mission != 1 && ship->mission != 2 &&
                    ship->mission != 4 && ship->mission != 5 &&
                    ship->mission != 6 && ship->mission != 7) {

                    if (SHIPMOVE::Can_Order_Ship_(ship)) {
                        ship_indices[num_ships] = ship_idx;
                        num_ships++;
                    } else {
                        int16_t dest_star = SHIPMOVE::Ship_Destination_Star_(ship);
                        int8_t sp_idx = stage_point_map[dest_star];
                        if (sp_idx != -1) {
                            stage_points[sp_idx].strength += (int16_t)Ship_Rough_Strength_At_Star_(ship_idx, dest_star);
                        }
                    }
                }
            }
        }

        if (num_stage_points == 0) {
            return;
        }

        if (num_stage_points == 1) {
            int16_t ship_list[2];
            ship_list[1] = -1;
            uint8_t target_star = stage_points[0].star_idx;

            for (int16_t i = 0; i < num_ships; ++i) {
                int16_t ship_idx = ship_indices[i];
                bool has_navigator = struct_::Test_Bit_Field_((uint8_t*)MOX::_ship[ship_idx].d.special_device_flags, 11);
                ship_list[0] = ship_idx;

                int16_t location = MOX::_ship[ship_idx].location;
                int16_t intermediate_star;

                if (location < MAX_STARS) {
                    AIDATA::Player_N_Turns_From_Star_To_Star_With_Intermediate_(
                        _i_ai, location, target_star, &intermediate_star, has_navigator);
                } else {
                    int16_t turns;
                    Move_Ships_With_Possible_Intermediate_(
                        ship_list, target_star, &intermediate_star, &turns);
                }

                if (intermediate_star != -1) {
                    MOX::_ship[ship_idx].mission = 3;
                    s_ship_move_info move_info;
                    SHIPMOVE::Ships_Try_To_Move_To_(ship_list, intermediate_star, &move_info);
                    uint8_t turns_left = move_info.turns_left;
                    if (turns_left <= 1 || warp_beast_caution == 0) {
                        SHIPMOVE::Make_Ships_Move_To_(ship_list, intermediate_star, &move_info);
                    }
                }
            }
            return;
        }

        RUSS::Shuffle_Sint_(ship_indices, num_ships);

        while (num_ships > 0 && num_stage_points > 0) {
            int32_t best_sp_idx = -1;
            uint32_t best_strength = 0xFFFFFFFF;
            uint32_t best_weight = 1;
            int32_t best_ship_i = -1;
            uint32_t best_ship_star = 0xFFFFFFFF;

            int32_t best_turns = (warp_beast_caution == 1) ? 2 : 255;

            RUSS::Mox_Update_();

            for (int16_t i = 0; i < num_stage_points; ++i) {
                uint32_t strength = stage_points[i].strength;
                uint8_t star_idx = stage_points[i].star_idx;
                uint32_t weight = (*AIDATA::_star_stage_weights)[star_idx][_i_ai];

                bool better = false;
                if (best_sp_idx == -1) {
                    better = true;
                } else {
                    int32_t cmp = RUSS::Compare_Ratios_(strength, weight, best_strength, best_weight);
                    if (cmp == -1) {
                        better = true;
                    } else if (cmp == 0 && best_weight < weight) {
                        better = true;
                    }
                }
                if (better) {
                    best_strength = strength;
                    best_sp_idx = i;
                    best_weight = weight;
                }
            }

            uint8_t target_star = stage_points[best_sp_idx].star_idx;
            int32_t best_ship_turns = best_turns;

            for (int16_t i = 0; i < num_ships; ++i) {
                int16_t ship_idx = ship_indices[i];
                bool has_navigator = struct_::Test_Bit_Field_((uint8_t*)MOX::_ship[ship_idx].d.special_device_flags, 11);
                int16_t location = MOX::_ship[ship_idx].location;

                if (location == target_star) {
                    best_ship_i = i;
                    best_ship_star = target_star;
                    best_ship_turns = 0;
                } else if (location < MAX_STARS) {
                    int16_t intermediate_star;
                    int16_t turns = AIDATA::Player_N_Turns_From_Star_To_Star_With_Intermediate_(
                        _i_ai, location, target_star, &intermediate_star, has_navigator);
                    if (best_ship_turns > turns) {
                        best_ship_i = i;
                        best_ship_star = intermediate_star;
                        best_ship_turns = turns;
                    }
                } else {
                    int16_t ship_list[2];
                    ship_list[0] = ship_idx;
                    ship_list[1] = -1;
                    int16_t intermediate_star;
                    int16_t turns;
                    Move_Ships_With_Possible_Intermediate_(
                        ship_list, target_star, &intermediate_star, &turns);
                    if (best_ship_turns > turns) {
                        best_ship_i = i;
                        best_ship_star = intermediate_star;
                        best_ship_turns = turns;
                    }
                }
            }

            if (best_ship_i == -1) {
                num_stage_points--;
                stage_point_map[best_sp_idx] = stage_point_map[num_stage_points];
            } else {
                int16_t ship_idx = ship_indices[best_ship_i];
                int16_t ship_list[2];
                ship_list[0] = ship_idx;
                ship_list[1] = -1;
                MOX::_ship[ship_idx].mission = 3;
                s_ship_move_info temp_move_info;
                
                SHIPMOVE::Ships_Try_To_Move_To_(ship_list, best_ship_star, &temp_move_info);
                SHIPMOVE::Make_Ships_Move_To_(ship_list, best_ship_star, &temp_move_info);

                int16_t rough_strength = (int16_t)Ship_Rough_Strength_At_Star_(ship_idx, target_star);
                num_ships--;
                stage_points[best_sp_idx].strength += rough_strength;
                ship_indices[best_ship_i] = ship_indices[num_ships];
            }
        }
    }

    void __cdecl Move_NPCs_() {
        uint8_t star_flag1[MAX_STARS] = { 0 };
        uint8_t star_flag2[MAX_STARS] = { 0 };

        int32_t monster_count = -1;

        for (int16_t ship_idx = 0; ship_idx < MOX::_NUM_SHIPS; ++ship_idx) {
            s_ship_data* ship = &MOX::_ship[ship_idx];

            if (ship->owner < MAX_PLAYERS) {
                continue;
            }

            if (ship->d.speed == 0) {
                continue;
            }

            if (ship->status == 1) {
                star_flag2[ship->location - SHIP_LOCATION_MOVING_OFFSET] = 1;
                continue;
            }

            if (ship->status != 0) {
                continue;
            }

            bool try_move = false;

            if (ship->owner != 13) {
                try_move = true;
            } else {
                if (star_flag1[ship->location] != 0) {
                    try_move = true;
                }

                star_flag1[ship->location] = 1;
                star_flag2[ship->location] = 1;

                ship->d.date_of_design++;

                if (ship->d.date_of_design == 30) {
                    int32_t dest = AIMOVE::Npc_Dest_(ship_idx, star_flag2);
                    ship->d.date_of_design = 0;

                    // Assembly uses d.date_of_design at ship offset +0x61 as the 30-turn eel timer.
                    // Original bug preserved: if dest == -1 and monster_count is still -1,
                    // the spawn gate below still passes because -1 < 4.
                    if (dest != -1) {
                        if (monster_count == -1) {
                            monster_count = 0;
                            for (int16_t i = 0; i < MOX::_NUM_SHIPS; ++i) {
                                s_ship_data* s = &MOX::_ship[i];
                                if (s->owner == 13 && s->d.speed > 0 && s->status < 5) {
                                    monster_count++;
                                }
                            }
                        }
                    }

                    if (monster_count < 4) {
                        s_ship_design eel_design = {};
                        INITSHIP::Load_Eel_Ship_Design_(&eel_design, 1);

                        s_star_data* star = &MOX::_star[ship->location];
                        int32_t new_ship_idx = SHIPMOVE::Create_Nonplayer_Ship_(13, &eel_design, star->x, star->y);

                        if (new_ship_idx != -1) {
                            s_MSG msg = {};
                            msg.msg_type = MSG_TYPE_EEL_SPAWNED;
                            msg.u.eel.star_idx = (uint8_t)ship->location;
                            MSG_::Add_Msg_All_(msg);

                            // Assembly clears the same +0x61 timer field on the spawned eel.
                            MOX::_ship[new_ship_idx].d.date_of_design = 0;
                        }
                        monster_count++;
                    }
                }
            }

            if (try_move) {
                int32_t dest = AIMOVE::Npc_Dest_(ship_idx, star_flag2);

                if (dest == -1) {
                    s_MSG msg = {};
                    msg.msg_type = MSG_TYPE_EEL_VANISHED;
                    msg.u.eel.star_idx = (uint8_t)ship->location;
                    MSG_::Add_Msg_All_(msg);

                    SHIPS::Kill_Ship_(ship_idx);
                } else {
                    int16_t ships_arr[2];
                    ships_arr[0] = ship_idx;
                    ships_arr[1] = -1;

                    s_ship_move_info move_info = {};
                    SHIPMOVE::Ships_Try_To_Move_To_(ships_arr, (int16_t)dest, &move_info);
                    SHIPMOVE::Make_Ships_Move_To_(ships_arr, (int16_t)dest, &move_info);
                }
            }
        }
    }

    /**
     * @brief Finds the colony ship that can reach the destination in the minimum number of turns 
     * and moves it, potentially via intermediate stars.
     * 
     * @param ship_indices Pointer to an array of ship indices to evaluate.
     * @param ship_count Pointer to the number of ship indices in the array.
     * @param destination The target destination star index.
     */
    void __cdecl Get_Best_Unescorted_Colony_Ship_(int16_t* ship_indices, int16_t* ship_count, int16_t destination) {
        int16_t best_ship_array_idx = -1;
        int16_t min_turns = 0x7fff;
        int16_t best_dest = -1;
        int16_t ship_list[2];
        s_ship_move_info move_info;

        ship_list[1] = -1;

        for (int16_t i = 0; i < *ship_count; ++i) {
            ship_list[0] = ship_indices[i];
            int16_t found_dest;
            int16_t turns;

            Move_Ships_With_Possible_Intermediate_(ship_list, destination, &found_dest, &turns);

            if (found_dest != -1 && turns < min_turns) {
                min_turns = turns;
                best_ship_array_idx = i;
                best_dest = found_dest;
            }
        }

        if (best_ship_array_idx != -1) {
            ship_list[0] = ship_indices[best_ship_array_idx];

            SHIPMOVE::Ships_Try_To_Move_To_(ship_list, best_dest, &move_info);
            SHIPMOVE::Make_Ships_Move_To_(ship_list, best_dest, &move_info);

            // Remove the ship from the array by replacing it with the last element
            int16_t last_idx = *ship_count - 1;
            *ship_count = last_idx;
            ship_indices[best_ship_array_idx] = ship_indices[last_idx];
        }
    }

    bool __cdecl Star_Has_Monster_(int star_idx) {
        for (int i = 0; i < (int)MOX::_NUM_SHIPS; ++i) {
            s_ship_data* ship = &MOX::_ship[i];

            if (ship->owner >= MAX_PLAYERS && (int)ship->location == star_idx && ship->status == 0) {
                return true;
            }
        }

        return false;
    }

    int16_t __cdecl I_Soonest_Enemy_Colony_Ship_(int16_t* ship_indices, int16_t count, int16_t target_location) {
        int16_t best_idx = -1;
        uint16_t min_turns = 0x7fff;

        for (int16_t i = 0; i < count; ++i) {
            int16_t ship_idx = ship_indices[i];
            s_ship_data* ship = &MOX::_ship[ship_idx];

            if (ship->location == target_location) {
                return i;
            }

            int32_t diff = (int32_t)ship->location - (int32_t)target_location;
            if (diff == SHIP_LOCATION_MOVING_OFFSET || diff == SHIP_LOCATION_WORMHOLE_OFFSET) {
                uint16_t turns = (uint8_t)ship->turns_left;
                if (turns < min_turns) {
                    min_turns = turns;
                    best_idx = i;
                }
            }
        }

        return best_idx;
    }

    int32_t __cdecl N_Enemy_Stars_Reachable_(int16_t player_idx, int16_t player_bit, int16_t source_star_idx) {
        int32_t reachable_count = 0;

        for (int i = 0; i < (int)MOX::_NUM_STARS; ++i) {
            if ((MOX::_star[i].has_colony >> (player_bit & 0x1f)) & 1) {
                int16_t ship_range = MOX::_player[player_idx].ship_range;
                int16_t distance = GEO::Parsecs_Between_Stars_(i, source_star_idx);

                if (distance <= ship_range) {
                    if (!struct_::Test_Bit_Field_(MOX::_star[i].black_hole_blocks, source_star_idx)) {
                        reachable_count++;
                    }
                }
            }
        }

        return reachable_count;
    }

    int32_t __cdecl Enemy_Colony_Worth_To_Player_(int16_t colony_idx, int16_t player_idx) {
        int8_t owner = MOX::_colony[colony_idx].owner;
        int32_t weight_player;
        int32_t weight_owner;

        if (MOX::_player[player_idx].treaty[owner] == 4) {
            weight_player = 2;
            weight_owner = 4;
        } else if (MOX::_player[player_idx].treaty[owner] == 6) {
            weight_player = 0;
            weight_owner = 6;
        } else {
            weight_player = 1;
            weight_owner = 5;
        }

        if (MOX::_player[player_idx].traits[TRAIT_TELEPATHIC] != 0 && MOX::_player[owner].traits[TRAIT_TELEPATHIC] == 0) {
            weight_player++;
            weight_owner--;
        }

        int16_t planet_idx = MOX::_colony[colony_idx].planet;

        return (
            (*AIDATA::_base_planet_values)[player_idx][planet_idx] * weight_player +
            (*AIDATA::_base_planet_values)[owner][planet_idx] * weight_owner
        ) / 6;
    }

    void __cdecl Move_Transports_() {
        int16_t ai_idx = _i_ai;
        uint8_t ai_shift = (uint8_t)_i_ai;
        uint8_t dest_turns_colony[MAX_STARS];
        uint8_t dest_turns_outpost[MAX_STARS];
        int8_t dest_star_arr[MAX_STARS];

        for (int i = 0; i < MAX_STARS; i++) {
            dest_turns_colony[i] = 0;
            dest_turns_outpost[i] = 0;
            dest_star_arr[i] = -1;
        }

        int16_t transport_ships[MAX_SHIPS];
        int16_t num_transports = 0;

        for (int16_t i = 0; i < MOX::_NUM_SHIPS; i++) {
            s_ship_data *ship = &MOX::_ship[i];

            if (ship->owner == ai_idx && ship->status < 3) {
                uint8_t ship_type = ship->d.ship_type;

                if (ship_type == SHIP_TYPE_COMBAT) {
                    if (ship->mission == 1) {
                        if (ship->location < MAX_STARS) {
                            dest_turns_colony[ship->location] = 1;
                            dest_star_arr[ship->location] = (int8_t)ship->location;
                        } else {
                            int16_t dest = SHIPMOVE::Ship_Destination_Star_(ship);
                            uint8_t turns = ship->turns_left;
                            if (dest_turns_colony[dest] == 0) {
                                dest_turns_colony[dest] = turns + 1;
                            } else if (turns + 1 < dest_turns_colony[dest]) {
                                dest_turns_colony[dest] = turns + 1;
                            }
                        }
                    } else if (ship->mission == 6) {
                        if (ship->location < MAX_STARS) {
                            dest_turns_outpost[ship->location] = 1;
                            dest_star_arr[ship->location] = (int8_t)ship->location;
                        } else {
                            int16_t dest = SHIPMOVE::Ship_Destination_Star_(ship);
                            uint8_t turns = ship->turns_left;
                            if (dest_turns_outpost[dest] == 0) {
                                dest_turns_outpost[dest] = turns + 1;
                            } else if (turns + 1 < dest_turns_outpost[dest]) {
                                dest_turns_outpost[dest] = turns + 1;
                            }
                        }
                    }
                } else if (ship_type == SHIP_TYPE_TRANSPORT) {
                    int16_t loc = ship->location;
                    if (loc < MAX_STARS) {
                        RUSS::Mox_Update_();
                        if (MOX::_settings.game_difficulty > 0) {
                            if (((MOX::_star[loc].has_colony >> ai_shift) & 1) != 0) {
                                if (MOX::_settings.game_difficulty >= 2 || _p_ai->objectives == PLAYER_OBJECTIVE_MILITARIST) {
                                    int16_t ship_idx = i;
                                    for (int j = 4; j >= 0; j--) {
                                        int16_t planet_idx = MOX::_star[loc].planet_index[j];
                                        if (planet_idx != -1) {
                                            int16_t colony_idx = MOX::_planet[planet_idx].colony_index;
                                            if (colony_idx != -1) {
                                                s_colony *colony = &MOX::_colony[colony_idx];
                                                if (colony->owner == ai_idx && colony->outpost_flag == 0) {
                                                    if ((*AIDATA::_ai_colony_info)[colony_idx].threatened != 0 ||
                                                        colony->military[0] < (*AIDATA::_ai_colony_info)[colony_idx].n_flagged_pops * 2) {
                                                        if (invasion::Unload_Transports_(colony, &ship_idx, 1) == 1) {
                                                            break;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if (ship->status != 5) {
                        transport_ships[num_transports] = i;
                        num_transports++;
                    }
                }
            }
        }

        while (num_transports > 0) {
            num_transports--;
            int16_t ship_idx = transport_ships[num_transports];
            int16_t ship_arr[2];
            ship_arr[0] = ship_idx;
            ship_arr[1] = -1;

            s_ship_data *ship = &MOX::_ship[ship_idx];
            int16_t loc = ship->location;
            RUSS::Mox_Update_();

            if (loc < MAX_STARS) {
                uint8_t need_to_move = 1;
                int16_t dest_star = dest_star_arr[loc];

                if (dest_star != -1) {
                    int16_t out_dest;
                    int16_t out_eta;
                    Move_Ships_With_Possible_Intermediate_(ship_arr, dest_star, &out_dest, &out_eta);

                    if (out_dest != -1) {
                        if (out_eta >= dest_turns_outpost[dest_star] - 1) {
                            s_ship_move_info move_info;
                            SHIPMOVE::Ships_Try_To_Move_To_(ship_arr, out_dest, &move_info);
                            SHIPMOVE::Make_Ships_Move_To_(ship_arr, out_dest, &move_info);
                        } else if (dest_turns_colony[dest_star] - out_eta == 1) {
                            s_ship_move_info move_info;
                            SHIPMOVE::Ships_Try_To_Move_To_(ship_arr, out_dest, &move_info);
                            SHIPMOVE::Make_Ships_Move_To_(ship_arr, out_dest, &move_info);
                        }
                        need_to_move = 0;
                    }
                }

                if (need_to_move) {
                    bool found_better = false;
                    int16_t best_dest = -1;
                    int16_t best_eta = 0x7FFF;

                    for (int16_t i = 0; i < MOX::_NUM_STARS; i++) {
                        uint8_t t_colony = dest_turns_colony[i];
                        uint8_t t_outpost = dest_turns_outpost[i];

                        if (t_colony == 0 && t_outpost == 0) continue;

                        if (t_colony < 11 && dest_turns_colony[loc] >= 2) {
                            if (_flee_stars[i * 10 + t_colony - 1] != 0) continue;
                        }
                        if (t_outpost < 11 && dest_turns_outpost[loc] >= 2) {
                            if (_flee_stars[i * 10 + t_outpost - 1] != 0) continue;
                        }

                        int16_t out_dest;
                        int16_t out_eta;
                        Move_Ships_With_Possible_Intermediate_(ship_arr, i, &out_dest, &out_eta);
                        if (out_dest == -1) continue;

                        bool better = false;
                        bool worse = false;

                        if (out_eta < (int16_t)t_outpost - 1) {
                            better = true;
                            out_eta = (int16_t)t_colony - 1;
                        } else {
                            int16_t t_colony_minus_1 = (int16_t)t_colony - 1;
                            if (out_eta < t_colony_minus_1) {
                                better = true;
                                out_eta = (int16_t)t_colony - 1;
                            } else {
                                if ((t_colony_minus_1 < out_eta && t_outpost == 0) || out_eta >= 10 || _flee_stars[i * 10 + out_eta] != 0) {
                                    worse = true;
                                }
                            }
                        }

                        if (!worse && out_eta < best_eta) {
                            best_dest = out_dest;
                            best_eta = out_eta;
                            found_better = better;
                        }
                    }

                    if (best_dest != -1) {
                        if (!found_better) {
                            s_ship_move_info move_info;
                            SHIPMOVE::Ships_Try_To_Move_To_(ship_arr, best_dest, &move_info);
                            SHIPMOVE::Make_Ships_Move_To_(ship_arr, best_dest, &move_info);
                        }
                    } else {
                        int16_t stagepoint = _p_ai->offensive_stagepoint;
                        need_to_move = 0;
                        if (_flee_stars[stagepoint * 10 + 9] == 0 &&
                            ((MOX::_star[stagepoint].blockaded >> ai_shift) & 1) == 0 &&
                            loc != stagepoint) {
                            s_ship_move_info move_info;
                            SHIPMOVE::Ships_Try_To_Move_To_(ship_arr, stagepoint, &move_info);
                            if (move_info.moving != 0) {
                                SHIPMOVE::Make_Ships_Move_To_(ship_arr, stagepoint, &move_info);
                                need_to_move = 1;
                            }
                        }

                        if (!need_to_move && _flee_stars[loc * 10 + 1] != 0) {
                            int32_t best_eta_32 = 0x7FFF;
                            int32_t best_dest_32 = -1;
                            for (int16_t i = 0; i < MOX::_NUM_STARS; i++) {
                                if (_flee_stars[i * 10 + 9] == 0 &&
                                    ((MOX::_star[i].has_colony >> ai_shift) & 1) != 0) {
                                    s_ship_move_info move_info;
                                    SHIPMOVE::Ships_Try_To_Move_To_(ship_arr, i, &move_info);
                                    if (move_info.moving != 0) {
                                        if ((*AIDATA::_ai_star_info)[i].per_player[_i_ai].enemy_enroute_or_threat != 0) {
                                            move_info.turns_left -= 24;
                                        }
                                        if ((int32_t)move_info.turns_left < best_eta_32) {
                                            best_eta_32 = move_info.turns_left;
                                            best_dest_32 = i;
                                        }
                                    }
                                }
                            }

                            if (best_dest_32 != -1) {
                                s_ship_move_info move_info;
                                SHIPMOVE::Ships_Try_To_Move_To_(ship_arr, (int16_t)best_dest_32, &move_info);
                                SHIPMOVE::Make_Ships_Move_To_(ship_arr, (int16_t)best_dest_32, &move_info);
                            }
                        }
                    }
                }
            }
        }
    }

    void __cdecl Gather_Intercepting_Ships_(int16_t* ships_list, int32_t* ship_count, int8_t* intercept_map) {
        *ship_count = 0;

        for (int32_t i = 0; i < (int32_t)AIDATA::_n_all_ships; ++i) {
            int16_t ship_idx = (*AIDATA::_all_ships)[i];
            s_ship_data* ship = &MOX::_ship[ship_idx];

            if ((int16_t)ship->owner == AIMOVE::_i_ai) {
                bool should_gather = false;

                if (SHIPMOVE::Can_Order_Ship_(ship)) {
                    // Check if mission is one of the interceptable/idle types
                    switch (ship->mission) {
                        case 0:
                        case 2:
                        case 3:
                        case 5:
                        case 7:
                            should_gather = true;
                            break;
                        default:
                            break;
                    }
                } else {
                    // Ship is already moving, check if it's arriving at a target star
                    int16_t dest_star = SHIPMOVE::Ship_Destination_Star_(ship);
                    if (dest_star != -1 && intercept_map[dest_star] != -1 /* 0xff */) {
                        should_gather = true;
                    }
                }

                if (should_gather) {
                    ships_list[*ship_count] = ship_idx;
                    *ship_count = *ship_count + 1;
                }
            }
        }
    }

    void __cdecl Assign_New_Colony_Ship_Destinations_(s_ai_colonizable_planet* planets, int16_t* count) {
        int16_t num_my_colony_ships = 0;
        int16_t my_colony_ships[MAX_SHIPS];

        int16_t enemy_colony_ships_buf[MAX_SHIPS];
        int16_t* enemy_colony_ships_ptr = &enemy_colony_ships_buf[MAX_SHIPS];

        for (int i = 0; i < MOX::_NUM_SHIPS; i++) {
            s_ship_data* ship = &MOX::_ship[i];
            if (ship->d.ship_type == SHIP_TYPE_COLONY) {
                if (ship->owner == AIMOVE::_i_ai) {
                    if (SHIPMOVE::Can_Order_Ship_(ship)) {
                        my_colony_ships[num_my_colony_ships++] = i;
                    } else if (ship->status < 3) {
                        *(--enemy_colony_ships_ptr) = i;
                    }
                } else if (ship->status < 3) {
                    *(--enemy_colony_ships_ptr) = i;
                }
            }
        }

        int16_t num_enemy_colony_ships = (int16_t)(&enemy_colony_ships_buf[MAX_SHIPS] - enemy_colony_ships_ptr);

        while (*count > 0) {
            if (num_my_colony_ships <= 0) return;

            int16_t target_idx = *count;
            int16_t star_idx = MOX::_planet[planets[target_idx - 1].planet_idx].star_index;

            if ((~((1 << AIMOVE::_i_ai)) & (*AIDATA::_star_in_extended_range_of_player)[star_idx]) == 0 &&
                (*AIDATA::_ai_star_info)[star_idx].per_player[AIMOVE::_i_ai].enemy_enroute_or_threat == 0 &&
                AIMOVE::Star_Has_Monster_(star_idx) == 0)
            {
                if (MOX::_settings.game_difficulty >= 2 ||
                    (MOX::_settings.game_difficulty >= 1 && AIMOVE::_p_ai->objectives == PLAYER_OBJECTIVE_EXPANSIONIST) ||
                    (((*AIDATA::_star_in_range_of_player)[star_idx] >> AIMOVE::_i_ai) & 1) != 0)
                {
                    AIMOVE::Get_Best_Unescorted_Colony_Ship_(my_colony_ships, &num_my_colony_ships, star_idx);
                }
            } else {
                int16_t enemy_ship_idx = AIMOVE::I_Soonest_Enemy_Colony_Ship_(enemy_colony_ships_ptr, num_enemy_colony_ships, star_idx);
                int16_t max_eta = 0x7FFF;

                if (enemy_ship_idx != -1) {
                    max_eta = (uint8_t)MOX::_ship[enemy_colony_ships_ptr[enemy_ship_idx]].turns_left;
                }

                if (AIMOVE::Get_Best_Escorted_Colony_Ship_(my_colony_ships, &num_my_colony_ships, star_idx, max_eta) == 0) {
                    if (enemy_ship_idx != -1) {
                        enemy_colony_ships_ptr[enemy_ship_idx] = enemy_colony_ships_ptr[0];
                        enemy_colony_ships_ptr++;
                        num_enemy_colony_ships--;
                    }
                }
            }

            (*count)--;
        }

        if (num_my_colony_ships > 0) {
            uint8_t star_added[MAX_STARS];
            memset(star_added, 0, sizeof(star_added));

            int16_t num_candidates = 0;
            uint8_t candidate_stars[MAX_STARS];

            for (int i = 0; i < MOX::_NUM_STARS; i++) {
                s_star_data* star = &MOX::_star[i];
                if (((star->is_stagepoint >> AIMOVE::_i_ai) & 1) != 0 &&
                    ((star->blockaded >> AIMOVE::_i_ai) & 1) == 0)
                {
                    if (star_added[i] == 0) {
                        candidate_stars[num_candidates++] = (uint8_t)i;
                        star_added[i] = 1;
                    }
                }
            }

            for (int i = 0; i < num_my_colony_ships; i++) {
                RUSS::Shuffle_Char_((int8_t*)candidate_stars, num_candidates);

                qsort(candidate_stars, num_candidates, 1, AIMOVE::Sort_candidate_stars_);

                if (num_candidates > 0) {
                    for (int j = 0; j < num_candidates; j++) {
                        uint8_t star_idx = candidate_stars[j];
                        int16_t ship_idx_list[2] = { my_colony_ships[i], -1 };
                        s_ship_move_info move_info{};

                        SHIPMOVE::Ships_Try_To_Move_To_(ship_idx_list, star_idx, &move_info);

                        if (move_info.moving != 0) {
                            num_candidates--;
                            SHIPMOVE::Make_Ships_Move_To_(ship_idx_list, star_idx, &move_info);
                            candidate_stars[j] = candidate_stars[num_candidates];
                            break;
                        }
                    }
                }
            }
        }
    }

    void __cdecl Interceptions_(s_aimove_intercept_entry* intercepts, int* count, int8_t* star_to_intercept_idx, int is_desperate) {
        int num_intercepting_ships;
        int16_t intercepting_ships[MAX_SHIPS];
        int8_t knocked_out_stars[MAX_STARS];
        int num_knocked_out;

        Gather_Intercepting_Ships_(intercepting_ships, &num_intercepting_ships, star_to_intercept_idx);
        Knock_Out_Impossible_Interceptions_(intercepts, count, knocked_out_stars, &num_knocked_out, is_desperate);

        while (true) {
            if (*count <= 0) {
                return;
            }

            RUSS::Mox_Update_();

            int8_t ship_target_star[MAX_SHIPS];
            for (int i = 0; i < MAX_SHIPS; ++i) {
                ship_target_star[i] = -1;
            }

            int8_t ship_intermediate_star[MAX_SHIPS];
            for (int i = 0; i < MAX_SHIPS; ++i) {
                ship_intermediate_star[i] = -1;
            }

            uint32_t strength_totals[MAX_STARS * 10];
            for (int i = 0; i < MAX_STARS * 10; ++i) {
                strength_totals[i] = 0;
            }

            int8_t target_min_eta[MAX_STARS];
            int8_t target_best_eta[MAX_STARS];

            if (num_intercepting_ships > 0) {
                uint8_t ship_min_eta[MAX_SHIPS];
                int16_t star_min_strength[MAX_STARS];

                for (int ship_list_index = 0; ship_list_index < num_intercepting_ships; ++ship_list_index) {
                    int16_t ship_id = intercepting_ships[ship_list_index];
                    bool has_special_bit_11 = struct_::Test_Bit_Field_(MOX::_ship[ship_id].d.special_device_flags, 11);
                    int best_target = -1;
                    ship_min_eta[ship_id] = 10;

                    if (*count > 0) {
                        for (int target_index = 0; target_index < *count; ++target_index) {
                            s_aimove_intercept_entry* target = &intercepts[target_index];
                            int star_idx = target->star_idx;

                            star_min_strength[star_idx] = (int16_t)Ship_Min_Strength_Versus_Players_At_Star_(ship_id, target->player_mask, star_idx);

                            int16_t ship_intermediate_star_tmp;
                            int eta;

                            if (MOX::_ship[ship_id].location < MAX_STARS) {
                                eta = AIDATA::Player_N_Turns_From_Star_To_Star_With_Intermediate_(
                                    _i_ai,
                                    MOX::_ship[ship_id].location,
                                    star_idx,
                                    &ship_intermediate_star_tmp,
                                    has_special_bit_11
                                );
                            } else {
                                eta = AIDATA::Ship_N_Turns_To_Star_With_Intermediate_(
                                    ship_id,
                                    star_idx,
                                    &ship_intermediate_star_tmp
                                );
                            }

                            if (eta < 10) {
                                bool update_target = false;

                                if (eta < ship_min_eta[ship_id]) {
                                    update_target = true;
                                } else if (eta == ship_min_eta[ship_id]) {
                                    if (best_target != -1) {
                                        int e = eta;
                                        while (e < 10 && target->strength[e] == 0 && intercepts[best_target].strength[e] == 0) {
                                            e++;
                                        }

                                        if (target->strength[e] == 0) {
                                            // Condition implicitly fails, update_target remains false
                                        } else if (intercepts[best_target].strength[e] == 0) {
                                            update_target = true;
                                        } else {
                                            int cmp = RUSS::Compare_Ratios_(
                                                strength_totals[e + target->star_idx * 10],
                                                target->strength[e],
                                                strength_totals[e + intercepts[best_target].star_idx * 10],
                                                intercepts[best_target].strength[e]
                                            );
                                            if (cmp < 0) {
                                                update_target = true;
                                            }
                                        }
                                    } else {
                                        update_target = true;
                                    }
                                }

                                if (update_target) {
                                    best_target = target_index;
                                    ship_min_eta[ship_id] = (uint8_t)eta;
                                    ship_target_star[ship_id] = (char)target->star_idx;
                                    ship_intermediate_star[ship_id] = (char)ship_intermediate_star_tmp;
                                }
                            }
                        }
                    }

                    if (best_target != -1) {
                        uint8_t min_eta = ship_min_eta[ship_id];
                        if (min_eta < 10) {
                            int rem = 10 - min_eta;
                            uint32_t* ptr = &strength_totals[min_eta + intercepts[best_target].star_idx * 10];
                            int16_t min_str = star_min_strength[intercepts[best_target].star_idx];

                            do {
                                *ptr += min_str;
                                ptr++;
                                rem--;
                            } while (rem != 0);
                        }
                    }
                }
            }

            bool loop_again = false;
            if (*count > 0) {
                for (int target_index = 0; target_index < *count; ++target_index) {
                    s_aimove_intercept_entry* target = &intercepts[target_index];
                    int star_idx = target->star_idx;
                    bool target_met = false;
                    
                    target_min_eta[star_idx] = 10;

                    for (int e = 0; e < 10; ++e) {
                        uint32_t strength_needed = (uint32_t)target->strength[e];
                        
                        if (strength_needed != 0 || target->strength[9] == 0) {
                            if (strength_totals[e + star_idx * 10] >= strength_needed) {
                                if (!target_met) {
                                    target_best_eta[star_idx] = (char)e;
                                }
                                target_met = true;
                                if (strength_totals[e + star_idx * 10] > strength_needed * 4 + 8) {
                                    target_min_eta[star_idx] = (char)e;
                                    break;
                                }
                            }
                        }
                    }

                    if (!target_met && is_desperate == 0) {
                        target_best_eta[star_idx] = -1;
                        loop_again = true;
                    }
                }
            }

            if (loop_again) {
                (*count)--;
            } else {
                if (num_intercepting_ships > 0) {
                    for (int ship_list_index = 0; ship_list_index < num_intercepting_ships; ++ship_list_index) {
                        int16_t ship_id = intercepting_ships[ship_list_index];

                        if (ship_intermediate_star[ship_id] != -1 && SHIPMOVE::Can_Order_Ship_(&MOX::_ship[ship_id])) {
                            s_ship_move_info move_info;
                            int16_t ship_list[2] = { ship_id, -1 };
                            int16_t target_star = (int16_t)(uint8_t)ship_intermediate_star[ship_id];
                            SHIPMOVE::Ships_Try_To_Move_To_(ship_list, target_star, &move_info);

                            if (move_info.turns_left <= target_min_eta[(int)(uint8_t)ship_target_star[ship_id]]) {
                                int16_t ship_loc = HAROLD::XY_To_Star_Id_(MOX::_ship[ship_id].x, MOX::_ship[ship_id].y);
                                MOX::_ship[ship_id].mission = 5;

                                int16_t move_star = -1;
                                if (move_info.turns_left < target_best_eta[(int)(uint8_t)ship_target_star[ship_id]]) {
                                    if (MOX::_ship[ship_id].status == 1) {
                                        move_star = (int16_t)(uint8_t)ship_intermediate_star[ship_id];
                                    } else if (MOX::_ship[ship_id].status == 2) {
                                        MOX::_ship[ship_id].location = ship_loc;
                                        MOX::_ship[ship_id].status = 0;
                                    }
                                } else {
                                    move_star = (int16_t)(uint8_t)ship_target_star[ship_id];
                                }

                                if (move_star != -1) {
                                    SHIPMOVE::Make_Ships_Move_To_(ship_list, move_star, &move_info);
                                    (*AIDATA::_i_all_combat_ships)[_i_ai][ship_loc] = -1;
                                }
                            }
                        }
                    }
                }

                return;
            }
        }
    }

    void Intercept_Invaders_()
    {
        int i = 0;
        s_aimove_intercept_entry intercepts[MAX_STARS];

        // Compiler artifact: initialization immediately overwritten by memset
        intercepts[0].star_idx = -1;
        memset(intercepts, 0, sizeof(intercepts));

        int8_t star_to_intercept_idx[MAX_STARS];
        memset(star_to_intercept_idx, -1, sizeof(star_to_intercept_idx));

        if (MOX::_NUM_STARS > 0)
        {
            for (int star_idx = 0; star_idx < MOX::_NUM_STARS; star_idx++)
            {
                if ((MOX::_star[star_idx].has_colony >> _i_ai) & 1)
                {
                    for (int p = 0; p < MOX::_NUM_PLAYERS; p++)
                    {
                        if (p != _i_ai)
                        {
                            // [9] acts as an indicator/total for enroute ships
                            if ((*AIDATA::_enroute_ships)[star_idx][p][9] != 0 &&
                                ((*AIDATA::_enroute_ships)[star_idx][p][0] != 0 ||
                                    (*AIDATA::_enroute_nonstealthy_ships)[star_idx][p] != 0))
                            {
                                int eta = 0;
                                while ((*AIDATA::_enroute_ships)[star_idx][p][eta] == 0)
                                {
                                    eta++;
                                    if (eta >= 10) break;
                                }

                                if (eta < 10)
                                {
                                    // Keep UI responsive during heavy AI computation
                                    RUSS::Mox_Update_();

                                    int idx = star_to_intercept_idx[star_idx];
                                    if (idx == -1)
                                    {
                                        intercepts[i].star_idx = star_idx;
                                        intercepts[i].min_eta = 0xFF;
                                        star_to_intercept_idx[star_idx] = i;
                                        idx = i;
                                        i++;
                                    }

                                    intercepts[idx].player_mask |= (1 << p);
                                    if (eta < intercepts[idx].min_eta)
                                    {
                                        intercepts[idx].min_eta = eta;
                                    }

                                    if (eta < 10)
                                    {
                                        for (int j = 0; j < 10 - eta; j++)
                                        {
                                            intercepts[idx].strength[eta + j] +=
                                                (*AIDATA::_enroute_ship_strengths)[star_idx][p][_i_ai][eta + j];
                                        }
                                    }

                                    intercepts[idx].players[p].present = 1;
                                    intercepts[idx].players[p].eta = eta;
                                }
                            }
                        }
                    }

                    int idx = star_to_intercept_idx[star_idx];
                    if (idx != -1)
                    {
                        if (intercepts[idx].min_eta < 10)
                        {
                            for (int j = 0; j < 10 - intercepts[idx].min_eta; j++)
                            {
                                if (intercepts[idx].strength[intercepts[idx].min_eta + j] == 0)
                                {
                                    intercepts[idx].strength[intercepts[idx].min_eta + j] = 1;
                                }
                            }
                        }
                    }
                }
            }
        }

        s_aimove_alien_strength alien_str[MAX_STARS][7];
        memset(alien_str, 0, sizeof(alien_str));

        bool aliens_present = false;

        for (int s = 0; s < MOX::_NUM_SHIPS; s++)
        {
            s_ship_data* ship = &MOX::_ship[s];
            if (ship->owner >= MAX_PLAYERS && ship->d.speed != 0)
            {
                if (ship->status == 0) // In orbit
                {
                    int star_idx = ship->location;
                    if ((MOX::_star[star_idx].has_colony >> _i_ai) & 1)
                    {
                        uint32_t strength = (uint16_t)AIPOWER::Ship_Strength_Vs_Ships_(s, _i_ai);
                        alien_str[star_idx][ship->owner - MAX_PLAYERS].strength += strength;
                        if (alien_str[star_idx][ship->owner - MAX_PLAYERS].strength == 0)
                        {
                            alien_str[star_idx][ship->owner - MAX_PLAYERS].strength = 1;
                        }
                        alien_str[star_idx][ship->owner - MAX_PLAYERS].eta = 0;
                        aliens_present = true;
                    }
                }
                else if (ship->status > 0 && ship->status <= 2) // En-route
                {
                    int star_idx = ship->location - (ship->status == 1 ? SHIP_LOCATION_MOVING_OFFSET : SHIP_LOCATION_WORMHOLE_OFFSET);
                    if ((MOX::_star[star_idx].has_colony >> _i_ai) & 1)
                    {
                        uint32_t strength = (uint16_t)AIPOWER::Ship_Strength_Vs_Ships_(s, _i_ai);
                        alien_str[star_idx][ship->owner - MAX_PLAYERS].strength += strength;
                        if (alien_str[star_idx][ship->owner - MAX_PLAYERS].strength == 0)
                        {
                            alien_str[star_idx][ship->owner - MAX_PLAYERS].strength = 1;
                        }

                        // Original bug: alien_str.eta is initialized to 0, so turns_left is never < eta
                        if (ship->turns_left < alien_str[star_idx][ship->owner - MAX_PLAYERS].eta)
                        {
                            alien_str[star_idx][ship->owner - MAX_PLAYERS].eta = ship->turns_left;
                        }
                        aliens_present = true;
                    }
                }
            }
        }

        if (aliens_present)
        {
            for (int o = 0; o < 7; o++)
            {
                for (int star_idx = 0; star_idx < MOX::_NUM_STARS; star_idx++)
                {
                    if (alien_str[star_idx][o].strength != 0)
                    {
                        int idx = star_to_intercept_idx[star_idx];
                        if (idx == -1)
                        {
                            intercepts[i].star_idx = star_idx;
                            intercepts[i].min_eta = 0xFF;
                            star_to_intercept_idx[star_idx] = i;
                            idx = i;
                            i++;
                        }

                        intercepts[idx].player_mask |= (1 << (o + MAX_PLAYERS));

                        if (alien_str[star_idx][o].eta < intercepts[idx].min_eta)
                        {
                            intercepts[idx].min_eta = alien_str[star_idx][o].eta;
                        }

                        if (intercepts[idx].min_eta < 10)
                        {
                            for (int j = 0; j < 10 - intercepts[idx].min_eta; j++)
                            {
                                intercepts[idx].strength[intercepts[idx].min_eta + j] += alien_str[star_idx][o].strength;
                            }
                        }
                    }
                }
            }
        }

        if (i > 0)
        {
            bool is_desperate = ((*AIDATA::_ai_player_info)[_i_ai].n_stars_with_colony <= 3);
            AIMOVE::Interceptions_(intercepts, &i, star_to_intercept_idx, is_desperate);
        }
    }

    void __cdecl Find_Best_Attack_From_Star_(int16_t star_idx, s_attack_info* target_info, int mission_type, int min_pop)
    {
        s_attack_info target_info_copy;
        s_attack_info* puVar8 = nullptr;
        uint16_t max_score = 0;
        bool valid_target_found = false;
        
        s_ship_move_info move_info;
        uint8_t visited_stars[MAX_STARS];
        int8_t mission_array[252];
        uint16_t scores[MAX_COLONIES];
        int16_t ship_list[SHIP_LIST_CAPACITY];

        // Initialize copy of target_info
        if (target_info != nullptr) {
            memcpy(&target_info_copy, target_info, sizeof(target_info_copy));

            puVar8 = &target_info_copy;
        }

        for (int i = 0; i < MAX_STARS; i++) {
            visited_stars[i] = 0;
        }

        for (int i = 0; i < MAX_COLONIES; i++) {
            scores[i] = 0;
        }

        int16_t ship_count = Gather_Player_Ships_At_Star_(_i_ai, star_idx, ship_list);

        if (MOX::_NUM_COLONIES > 0) {
            for (int i = 0; i < MOX::_NUM_COLONIES; i++) {
                int8_t owner = MOX::_colony[i].owner;
                
                if (owner != -1) {
                    if (target_info == nullptr) {
                        if (!AI_May_Attack_Player_(_i_ai, owner)) {
                            continue;
                        }
                    } else {
                        if (target_info->target_player != owner) {
                            continue;
                        }
                    }

                    int16_t planet_idx = MOX::_colony[i].planet;
                    int target_star = (int)MOX::_planet[planet_idx].star_index;

                    if (visited_stars[target_star] == 0) {
                        if ((((*AIDATA::_star_in_range_of_player)[target_star] >> _i_ai) & 1) != 0) {
                            SHIPMOVE::Ships_Try_To_Move_To_(ship_list, (int16_t)target_star, &move_info);
                            
                            if (move_info.moving != 0 && (uint8_t)move_info.turns_left < 10) {
                                uint16_t score = Compute_Attack_Worthiness_(i, ship_list, (uint8_t)move_info.turns_left, &mission_array[i], &visited_stars[target_star], puVar8, min_pop);
                                scores[i] = score;

                                if (score != 0) {
                                    int8_t mission = mission_array[i];
                                    
                                    if (mission == 1) {
                                        if (mission_type != 1) {
                                            scores[i] = 0;
                                            continue;
                                        }
                                        if (max_score < score) {
                                            max_score = score;
                                            if (puVar8 != nullptr && puVar8->target_planet != -1) {
                                                target_info->target_planet = puVar8->target_planet;
                                                target_info->value_a = puVar8->value_a;
                                                target_info->value_b = puVar8->value_b;
                                            }
                                        }
                                    } else if (mission == 2) {
                                        if (mission_type != 2) {
                                            scores[i] = 0;
                                            continue;
                                        }
                                        if (max_score < score) {
                                            max_score = score;
                                        }
                                    } else if (mission == 6) {
                                        if (mission_type != 6) {
                                            scores[i] = 0;
                                            continue;
                                        }
                                        if (max_score < score) {
                                            max_score = score;
                                        }
                                    } else {
                                        // Bypasses validation setup entirely, leaves initial calculated score intact
                                        continue;
                                    }

                                    valid_target_found = true;
                                    continue;
                                }
                            }
                        }
                    }
                }
                scores[i] = 0;
            }
        }

        if (valid_target_found && target_info == nullptr) {
            for (int i = 0; i < MAX_COLONIES; i++) {
                // Drop scores below 2/3 of maximum
                if ((uint32_t)scores[i] * 3 < (uint32_t)max_score * 2) {
                    scores[i] = 0;
                }
            }

            int16_t best_colony_idx = RUSS::Get_Weighted_Choice_Short_(scores, MAX_COLONIES);
            if (best_colony_idx == -1) {
                return;
            }

            int8_t best_mission = mission_array[best_colony_idx];
            int16_t best_planet_idx = MOX::_colony[best_colony_idx].planet;
            int8_t best_star_idx = MOX::_planet[best_planet_idx].star_index;

            (*AIDATA::_i_all_combat_ships)[_i_ai][star_idx] = -1;

            SHIPMOVE::Ships_Try_To_Move_To_(ship_list, best_star_idx, &move_info);
            SHIPMOVE::Make_Ships_Move_To_(ship_list, best_star_idx, &move_info);

            while (ship_count > 0) {
                ship_count--;
                int16_t ship_idx = ship_list[ship_count];
                MOX::_ship[ship_idx].mission = best_mission;
            }
        }
    }

    void __cdecl Knock_Out_Impossible_Interceptions_(
        s_aimove_intercept_entry* intercept_entries,
        int* num_entries,
        int8_t* knocked_out_stars,
        int* num_knocked_out,
        int keep_one_entry
    ) {
        *num_knocked_out = 0;

        for (int entry_idx = *num_entries - 1; entry_idx >= 0; --entry_idx) {
            s_aimove_intercept_entry* current_entry = &intercept_entries[entry_idx];
            bool can_intercept = false;
            int32_t star_idx = current_entry->star_idx;

            for (int class_idx = 0; class_idx < 10; ++class_idx) {
                uint32_t min_possible = 0xFFFFFFFF;
                uint32_t total_enroute = 0;

                if (MOX::_NUM_PLAYERS > 0) {
                    for (int p = 0; p < MOX::_NUM_PLAYERS; ++p) {
                        if (current_entry->players[p].present) {
                            uint32_t possible = (uint32_t)(
                                (*AIDATA::_possible_ship_strengths)[star_idx][_i_ai][p][class_idx]
                            );

                            if (possible < min_possible) {
                                min_possible = possible;
                            }

                            total_enroute += (uint32_t)(
                                (*AIDATA::_enroute_ship_strengths)[star_idx][p][_i_ai][class_idx]
                            );
                        }
                    }
                }

                if (total_enroute < min_possible) {
                    can_intercept = true;
                    break;
                }
            }

            if (!can_intercept && (keep_one_entry == 0 || *num_entries > 1)) {
                (*num_entries)--;
                int num_to_move = *num_entries - entry_idx;

                memmove(current_entry, current_entry + 1, num_to_move * sizeof(s_aimove_intercept_entry));

                knocked_out_stars[*num_knocked_out] = (int8_t)star_idx;
                (*num_knocked_out)++;

                memset(&_flee_stars[star_idx * ENEMY_STRENGTH_TURN_WINDOW], 1, ENEMY_STRENGTH_TURN_WINDOW);
            }
        }
    }

    int __cdecl Sort_npc_dests_(const void* a_ptr, const void* b_ptr) {
        auto a = (const s_aimove_npc_dest*)a_ptr;
        auto b = (const s_aimove_npc_dest*)b_ptr;
        int result;

        result = ((int)a->has_lucky - (int)a->has_unlucky) - ((int)b->has_lucky - (int)b->has_unlucky);
        if (result != 0) {
            return result;
        }

        result = (int)(a->dist >> 3) - (int)(b->dist >> 3);
        if (result != 0) {
            return result;
        }

        result = (int)b->unlucky_strength - (int)a->unlucky_strength;
        if (result != 0) {
            return result;
        }

        result = (int)b->lucky_strength - (int)a->lucky_strength;
        if (result != 0) {
            return result;
        }

        return (int)a->dist - (int)b->dist;
    }

    int16_t __cdecl Npc_Dest_(int16_t ship_idx, uint8_t* star_flags) {
        int32_t dest_star_idx = -1;
        int32_t ship_location = MOX::_ship[ship_idx].location;
        uint32_t player_str[MAX_PLAYERS] = { 0 };
        s_aimove_npc_dest npc_dest[MAX_STARS];

        memset(npc_dest, 0, sizeof(npc_dest));

        for (int i = 0; i < MOX::_NUM_PLAYERS; i++) {
            if (MOX::_player[i].eliminated == 0) {
                uint32_t str_val = special::isqrt_(MOX::_player[i].bc);
                str_val = special::isqrt_(str_val);
                player_str[i] = str_val +
                    MOX::_player[i].total_colonies * 4 +
                    (int16_t)(MOX::_player[i].total_research / 4) +
                    MOX::_player[i].total_ships;
            }
        }

        for (int i = 0; i < MOX::_NUM_COLONIES; i++) {
            int8_t owner = MOX::_colony[i].owner;
            if (owner != -1) {
                int16_t planet_idx = MOX::_colony[i].planet;
                int16_t star_idx = MOX::_planet[planet_idx].star_index;

                if (ship_location != star_idx && star_flags[star_idx] == 0) {
                    if (MOX::_player[owner].traits[TRAIT_LUCKY] == 0) {
                        npc_dest[star_idx].has_unlucky = 1;
                        if (npc_dest[star_idx].unlucky_strength < player_str[owner]) {
                            npc_dest[star_idx].unlucky_strength = player_str[owner];
                        }
                    }
                    else {
                        npc_dest[star_idx].has_lucky = 1;
                        if (npc_dest[star_idx].lucky_strength < player_str[owner]) {
                            npc_dest[star_idx].lucky_strength = player_str[owner];
                        }
                    }
                    npc_dest[star_idx].star_idx = (uint8_t)star_idx;
                    npc_dest[star_idx].dist = (uint8_t)GEO::Parsecs_Between_Stars_(ship_location, star_idx);
                }
            }
        }

        qsort(npc_dest, MOX::_NUM_STARS, sizeof(s_aimove_npc_dest), Sort_npc_dests_);

        int32_t valid_count = 0;
        for (int i = 0; i < MOX::_NUM_STARS; i++) {
            if (valid_count >= 7) {
                break;
            }
            if (npc_dest[i].dist != 0 && npc_dest[i].dist < 15) {
                valid_count++;
                if (game_random::Random_(valid_count) == 1) {
                    dest_star_idx = npc_dest[i].star_idx;
                }
            }
        }

        return (int16_t)dest_star_idx;
    }

    int8_t __cdecl Player_Is_Hostile_To_Player_(int player_idx_a, int player_idx_b) {
        int8_t result = 2;
        s_player* p_a = &MOX::_player[player_idx_a];

        // Check if a specific high-level treaty exists (e.g., Non-Aggression or Alliance)
        if (p_a->treaty[player_idx_b] >= 4 && p_a->treaty[player_idx_b] <= 6) {
            result = 1; /* Not Hostile */
        }

        if (result == 2) {
            switch (p_a->personality) {
                case PLAYER_PERSONALITY_XENOPHOBIC:
                case PLAYER_PERSONALITY_RUTHLESS:
                case PLAYER_PERSONALITY_AGGRESSIVE:
                case PLAYER_PERSONALITY_ERRATIC:
                    result = 1; /* Not Hostile */
                    break;
                case PLAYER_PERSONALITY_HONORABLE:
                case PLAYER_PERSONALITY_PACIFIST:
                    result = 0; /* Hostile */
                    break;
                default:
                    // Keep result as 2 to proceed to objective check
                    break;
            }

            if (result != 2) {
                return result;
            }

            // Fallback to checking objectives if personality was not recognized or is neutral
            if (p_a->objectives == PLAYER_OBJECTIVE_MILITARIST) {
                result = 1;
            }
        }

        // Final safety fallback: default to Not Hostile (1)
        if (result == 2) {
            result = 1;
        }

        return result;
    }

    int8_t __cdecl AI_May_Attack_Player_(int16_t player_idx, int16_t target_idx) {
        int8_t may_attack = 0;

        if (target_idx != player_idx) {
            s_player* plr = &MOX::_player[player_idx];

            if (plr->stop_blockading_duration[target_idx] == 0 &&
                plr->trade_treaty[target_idx] == 0 &&
                plr->research_treaty[target_idx] == 0 &&
                plr->treaty[target_idx] >= 4 &&
                plr->treaty[target_idx] <= 6) {
                may_attack = 1;
            }
        }

        return may_attack;
    }

    uint16_t __cdecl Compute_Attack_Worthiness_(
        int16_t colony_idx,
        int16_t* ship_list,
        uint8_t eta,
        int8_t* out_attack_type,
        uint8_t* out_flee,
        s_attack_info* target_info,
        int32_t min_pop
    ) {
        s_colony* colony = &MOX::_colony[colony_idx];
        int8_t owner = colony->owner;
        int16_t planet_idx = colony->planet;
        int8_t star_idx = MOX::_planet[planet_idx].star_index;

        uint32_t attack_worthiness = 0;
        uint32_t total_space_strength = 0;
        uint32_t total_bomb_strength = 0;
        bool in_nebula = MOX::_star[star_idx].in_nebula != 0;
        uint32_t hostility_bonus = (uint8_t)Player_Is_Hostile_To_Player_(_i_ai, owner);
        uint32_t total_ship_size = 0;

        bool can_attack = true;
        uint32_t n_pops = (uint8_t)colony->n_pops;
        if ((int32_t)n_pops < min_pop && colony->outpost_flag == 0 && _p_ai->personality != PLAYER_PERSONALITY_RUTHLESS && n_pops != 0) {
            for (uint32_t i = 0; i < n_pops; ++i) {
                if (POP::Get_Race(colony->pop[i]) == (uint32_t)_i_ai) {
                    can_attack = false;
                }
            }
        }

        if (!can_attack) {
            return 0;
        }

        int16_t shield = COLCALC::Colony_Planetary_Shield_(colony);

        int ship_idx_idx = 0;
        int16_t ship_idx = ship_list[0];
        if (ship_idx != -1) {
            do {
                uint16_t space_str = in_nebula ?
                    (*AIDATA::_ship_strengths_in_nebula)[ship_idx][owner] :
                    (*AIDATA::_ship_strengths)[ship_idx][owner];
                total_space_strength += space_str;

                uint16_t bomb_str = (*AIDATA::_ship_bomb_strengths)[ship_idx][shield];
                total_bomb_strength += bomb_str;

                total_ship_size += (1 << (MOX::_ship[ship_idx].d.size & 0x1F));

                ship_idx_idx++;
                ship_idx = ship_list[ship_idx_idx];
            } while (ship_idx != -1);
        }

        if (target_info == nullptr && total_ship_size > 0x96) {
            uint32_t scaled = (total_space_strength / 0x96) * total_ship_size;
            if (total_space_strength < scaled) {
                total_space_strength = scaled;
            }
        }

        uint32_t possible_space_strength = (*AIDATA::_possible_ship_strengths)[star_idx][owner][_i_ai][eta];
        uint32_t added_strength = 0;
        int producing_idx = 0;
        int32_t current_bc = MOX::_player[owner].bc;
        int32_t spent = colony->production_spent;

        for (int i = 0; i < 7; ++i) {
            if (current_bc + spent < 1 || eta <= producing_idx) break;

            int16_t prod_id = colony->producing[i];
            if (COLBLDG::Colony_Production_Is_Queued_Ship_(prod_id)) {
                int16_t ship_design_idx = COLBLDG::Colony_Production_Ship_Index_(prod_id);
                if (MOX::_ship[ship_design_idx].d.ship_type == SHIP_TYPE_COMBAT) {
                    int32_t cost = MOX::_ship[ship_design_idx].d.cost;
                    if (cost > spent) {
                        int32_t buy_cost = COLCALC::Colony_Cost_To_Buy_Given_Product_(colony, prod_id, spent);
                        if (buy_cost <= current_bc) {
                            spent = 0;
                            added_strength += AIPOWER::Ship_Strength_Vs_Ships_(ship_design_idx, _i_ai);
                            producing_idx++;
                            current_bc -= buy_cost;
                        }
                    }
                    else {
                        added_strength += AIPOWER::Ship_Strength_Vs_Ships_(ship_design_idx, _i_ai);
                        spent -= cost;
                        producing_idx++;
                    }
                }
            }
        }

        possible_space_strength += added_strength;

        if (total_bomb_strength + total_space_strength == 0 || (total_space_strength * 3 < possible_space_strength * 4)) {
            *out_flee = 1;
            if (eta < 10) {
                for (int i = eta; i < 10; ++i) {
                    _flee_stars[star_idx * 10 + i] = 1;
                }
            }
        }
        else {
            uint32_t colony_worth = Enemy_Colony_Worth_To_Player_(colony_idx, _i_ai);
            uint32_t defending_space = possible_space_strength + (AIPOWER::Colony_Space_Strength_Vs_Player_(colony_idx, _i_ai) & 0xFFFF);
            uint32_t defending_ground = AIPOWER::Colony_Ground_Strength_Vs_Player_(colony_idx, _i_ai) & 0xFFFF;

            uint32_t attack_space = total_space_strength + hostility_bonus;
            uint32_t attack_ground = total_bomb_strength + hostility_bonus;

            bool success = true;
            if (attack_space <= defending_space || attack_ground <= defending_ground) {
                success = false;
            }

            if (success) {
                *out_attack_type = 1;
                if (defending_space * 3 < total_space_strength) {
                    total_space_strength = 3;
                    defending_space = 1;
                }
                else if (total_space_strength == 0) {
                    total_space_strength = 1;
                    defending_space = 1;
                }

                if (defending_ground * 3 < total_bomb_strength) {
                    total_bomb_strength = 3;
                    defending_ground = 1;
                }
                else if (total_bomb_strength == 0) {
                    total_bomb_strength = 1;
                    defending_ground = 1;
                }

                if (target_info == nullptr && total_space_strength == 3 && defending_space == 1) {
                    *out_attack_type = 6;
                }

                if (defending_space >= 0x10000) {
                    defending_space >>= 8;
                    total_space_strength >>= 8;
                }
                if (defending_ground >= 0x10000) {
                    defending_ground >>= 8;
                    total_bomb_strength >>= 8;
                }

                int32_t reachable = N_Enemy_Stars_Reachable_(_i_ai, owner, star_idx);
                uint32_t reachable_sq = special::isqrt_(reachable * 100);

                uint32_t attack_strength = (colony_worth * total_space_strength) / defending_space;
                attack_strength = (attack_strength * total_bomb_strength) / defending_ground;

                attack_worthiness = (reachable_sq * attack_strength) / 10;

                if (target_info != nullptr) {
                    uint32_t target_val = (total_space_strength * 100 / defending_space * total_bomb_strength) / defending_ground;

                    target_info->value_b = (int16_t)target_val;

                    if (target_info->min_value < (int16_t)target_val) {
                        target_info->target_planet = planet_idx;
                        target_info->value_a = (int16_t)colony_worth;
                    }
                    else {
                        attack_worthiness = 0;
                    }
                }
            }
            else {
                *out_attack_type = 2;
                if (possible_space_strength * 3 < total_space_strength) {
                    total_space_strength = 3;
                    possible_space_strength = 1;
                }
                else if (total_space_strength == 0) {
                    total_space_strength = 1;
                    possible_space_strength = 1;
                }

                attack_worthiness = (colony_worth * total_space_strength) / (possible_space_strength * 2);

                if ((MOX::_star[star_idx].blockaded >> owner) & 1) {
                    uint8_t blocked_by = MOX::_star[star_idx].blockaded_by[owner];
                    if ((blocked_by & ~(1 << _i_ai)) != 0 || ((blocked_by >> _i_ai) & 1) == 0) {
                        attack_worthiness = 0;
                    }
                }
            }
        }

        if (eta < 2) {
            attack_worthiness += attack_worthiness / 6;
        }

        if (attack_worthiness > 0xFFFF) {
            attack_worthiness = 0xFFFF;
        }

        return (uint16_t)attack_worthiness;
    }

    uint16_t __cdecl Ship_Min_Strength_Versus_Players_At_Star_(int16_t ship_idx, uint16_t player_mask, int32_t star_idx) {
        uint16_t min_strength = 0x7fff;
        bool in_nebula = MOX::_star[star_idx].in_nebula != 0;

        // Check empire players (indices 0 to 7)
        if ((uint8_t)player_mask != 0) {
            for (int32_t i = 0; i < (int32_t)MOX::_NUM_PLAYERS; ++i) {
                if ((player_mask >> i) & 1) {
                    uint16_t(*strengths)[MAX_SHIPS][MAX_PLAYERS];
                    if (in_nebula) {
                        strengths = AIDATA::_ship_strengths_in_nebula;
                    }
                    else {
                        strengths = AIDATA::_ship_strengths;
                    }

                    uint16_t strength = (*strengths)[ship_idx][i];
                    if (strength < min_strength) {
                        min_strength = strength;
                    }
                }
            }
        }

        // Check special entities or high-index players (indices 8 to 14)
        if ((uint8_t)(player_mask >> MAX_PLAYERS) != 0) {
            for (int32_t i = MAX_PLAYERS; i < 15; ++i) {
                if ((player_mask >> i) & 1) {
                    uint16_t strength;
                    if (in_nebula) {
                        strength = AIPOWER::Ship_Strength_Vs_Ships_In_Nebula_(ship_idx, (int16_t)i);
                    }
                    else {
                        strength = AIPOWER::Ship_Strength_Vs_Ships_(ship_idx, (int16_t)i);
                    }

                    if (strength < min_strength) {
                        min_strength = strength;
                    }
                }
            }
        }

        return min_strength;
    }

    int8_t __cdecl Get_Best_Escorted_Colony_Ship_(int16_t* colony_ships, int16_t* num_colony_ships, int16_t star_idx, int16_t max_turns)
    {
        int32_t enemy_strength[ENEMY_STRENGTH_TURN_WINDOW] = { 0 };
        int32_t enemy_strength2[ENEMY_STRENGTH_TURN_WINDOW];
        int32_t total_enemy_strength[ENEMY_STRENGTH_TURN_WINDOW];

        for (int i = 0; i < ENEMY_STRENGTH_TURN_WINDOW; ++i) {
            total_enemy_strength[i] = 0;
        }

        Get_Enemy_Strength_On_Turn_(star_idx, enemy_strength, enemy_strength2);

        if (MOX::_NUM_PLAYERS > 0) {
            for (int p = 0; p < MOX::_NUM_PLAYERS; ++p) {
                if (p != AIMOVE::_i_ai) {
                    int8_t treaty = AIMOVE::_p_ai->treaty[p];
                    if (treaty < 1 || treaty > 3 || MOX::_player[p].objectives == PLAYER_OBJECTIVE_HUMAN) {
                        for (int t = 0; t < ENEMY_STRENGTH_TURN_WINDOW; ++t) {
                            total_enemy_strength[t] += (*AIDATA::_possible_ship_strengths)[star_idx][p][AIMOVE::_i_ai][t];
                        }
                    }
                }
            }
        }

        int32_t best_max_turn = 0x7FFF;
        int8_t success = 0;
        int32_t best_colony_ship_idx = -1;
        int32_t best_strategy = -1;
        int32_t best_dest = -1;
        int32_t best_num_escorts = -1;
        int32_t best_num_escorts_total = -1;
        int32_t c = 0;

        int16_t fleet[SHIP_LIST_CAPACITY];

        if (*num_colony_ships > 0) {
            do {
                int32_t num_escorts_total = 0;
                int32_t num_escorts = 1;
                fleet[0] = colony_ships[c];
                int16_t location = MOX::_ship[fleet[0]].location;

                if (location < MAX_STARS) {
                    int16_t first_combat_ship = (*AIDATA::_i_all_combat_ships)[AIMOVE::_i_ai][location];
                    if (first_combat_ship != -1) {
                        int32_t total_escort_strength = 0;
                        bool is_valid_strength = false;
                        if (max_turns < ENEMY_STRENGTH_TURN_WINDOW || enemy_strength2[ENEMY_STRENGTH_TURN_WINDOW - 1] != 0) {
                            is_valid_strength = true;
                        }

                        int16_t all_ship_idx = first_combat_ship;
                        while (all_ship_idx < (int16_t)AIDATA::_n_all_ships) {
                            int16_t s_idx = (*AIDATA::_all_ships)[all_ship_idx];
                            if (MOX::_ship[s_idx].owner == AIMOVE::_i_ai && MOX::_ship[s_idx].location == location) {
                                all_ship_idx++;
                                total_escort_strength += (int16_t)AIMOVE::Ship_Rough_Strength_At_Star_(s_idx, star_idx);
                                num_escorts_total++;
                            }
                            else {
                                break;
                            }
                        }

                        all_ship_idx = first_combat_ship;
                        int32_t strength = 0;
                        while (all_ship_idx < (int16_t)AIDATA::_n_all_ships) {
                            int16_t s_idx = (*AIDATA::_all_ships)[all_ship_idx];
                            if (MOX::_ship[s_idx].owner == AIMOVE::_i_ai && MOX::_ship[s_idx].location == location) {
                                all_ship_idx++;
                                strength += (int16_t)AIMOVE::Ship_Rough_Strength_At_Star_(s_idx, star_idx);
                                fleet[num_escorts] = s_idx;
                                fleet[num_escorts + 1] = -1;
                                num_escorts++;

                                int16_t dest_star;
                                int16_t turns;
                                AIMOVE::Move_Ships_With_Possible_Intermediate_(fleet, star_idx, &dest_star, &turns);

                                if (dest_star == -1 || turns > max_turns) {
                                    break;
                                }

                                if (is_valid_strength || total_escort_strength <= strength * 2) {
                                    if (turns < ENEMY_STRENGTH_TURN_WINDOW && turns < best_max_turn) {
                                        int32_t t2 = turns + 2;
                                        if (t2 >= ENEMY_STRENGTH_TURN_WINDOW) {
                                            t2 = ENEMY_STRENGTH_TURN_WINDOW - 1;
                                        }
                                        if (enemy_strength2[t2] < strength) {
                                            bool can_escort = true;
                                            int32_t t = turns + 1;
                                            if (total_enemy_strength[turns] > strength && total_escort_strength > strength) {
                                                can_escort = false;
                                            }
                                            if (t < ENEMY_STRENGTH_TURN_WINDOW) {
                                                for (; t < ENEMY_STRENGTH_TURN_WINDOW; ++t) {
                                                    if (enemy_strength[t] > strength) {
                                                        can_escort = false;
                                                    }
                                                }
                                            }
                                            if (can_escort) {
                                                best_max_turn = turns;
                                                best_colony_ship_idx = c;
                                                best_num_escorts = num_escorts - 1;
                                                best_strategy = 2;
                                                best_dest = dest_star;
                                                best_num_escorts_total = num_escorts_total;
                                            }
                                        }
                                    }
                                }

                                if (!is_valid_strength && total_escort_strength <= strength * 2) {
                                    break;
                                }
                            }
                            else {
                                break;
                            }
                        }
                    }
                }
                else {
                    int32_t strength = 0;
                    for (int16_t i = 0; i < (int16_t)MOX::_NUM_SHIPS; ++i) {
                        s_ship_data* ship = &MOX::_ship[i];
                        if (ship->owner == AIMOVE::_i_ai && ship->location == MOX::_ship[fleet[0]].location) {
                            if (ship->status < 3 && ship->x == MOX::_ship[fleet[0]].x && ship->y == MOX::_ship[fleet[0]].y) {
                                strength += (int16_t)AIMOVE::Ship_Rough_Strength_At_Star_(i, star_idx);
                                fleet[num_escorts] = i;
                                num_escorts++;
                            }
                        }
                    }
                    fleet[num_escorts] = -1;

                    if (MOX::_ship[fleet[0]].location - star_idx == SHIP_LOCATION_MOVING_OFFSET) {
                        uint8_t turns = MOX::_ship[fleet[0]].turns_left;
                        if (turns < ENEMY_STRENGTH_TURN_WINDOW && turns <= max_turns) {
                            if (turns < best_max_turn && enemy_strength2[turns] < strength) {
                                bool can_escort = true;
                                for (int32_t t = turns + 1; t < ENEMY_STRENGTH_TURN_WINDOW; ++t) {
                                    if (enemy_strength[t] > strength) {
                                        can_escort = false;
                                        break;
                                    }
                                }
                                if (can_escort) {
                                    best_max_turn = turns;
                                    best_dest = star_idx;
                                    best_strategy = 0;
                                    best_colony_ship_idx = c;
                                    best_num_escorts = num_escorts - 1;
                                    best_num_escorts_total = num_escorts - 1;
                                }
                            }
                        }
                    }
                    else {
                        int16_t dest_star;
                        int16_t turns;
                        AIMOVE::Move_Ships_With_Possible_Intermediate_(fleet, star_idx, &dest_star, &turns);
                        if (dest_star != -1 && turns < ENEMY_STRENGTH_TURN_WINDOW && turns <= max_turns) {
                            if (turns < best_max_turn && enemy_strength2[turns] < strength) {
                                bool can_escort = true;
                                for (int32_t t = turns + 1; t < ENEMY_STRENGTH_TURN_WINDOW; ++t) {
                                    if (enemy_strength[t] > strength) {
                                        can_escort = false;
                                        break;
                                    }
                                }
                                if (can_escort) {
                                    best_max_turn = turns;
                                    best_dest = dest_star;
                                    best_strategy = 1;
                                    best_colony_ship_idx = c;
                                    best_num_escorts = num_escorts - 1;
                                    best_num_escorts_total = num_escorts - 1;
                                }
                            }
                        }
                    }
                }
                c++;
            } while (c < *num_colony_ships);
        }

        if (best_colony_ship_idx != -1) {
            success = 1;
            fleet[0] = colony_ships[best_colony_ship_idx];

            if (best_strategy == 1) {
                int32_t ship_cnt = 1;
                for (int16_t i = 0; i < (int16_t)MOX::_NUM_SHIPS; ++i) {
                    s_ship_data* ship = &MOX::_ship[i];
                    if (ship->owner == AIMOVE::_i_ai && ship->location == MOX::_ship[fleet[0]].location) {
                        if (ship->status < 3 && ship->x == MOX::_ship[fleet[0]].x && ship->y == MOX::_ship[fleet[0]].y) {
                            ship->mission = 4;
                            fleet[ship_cnt++] = i;
                        }
                    }
                }
                fleet[ship_cnt] = -1;
            }
            else if (best_strategy == 2) {
                int16_t location = MOX::_ship[fleet[0]].location;
                int16_t all_ship_idx = (*AIDATA::_i_all_combat_ships)[AIMOVE::_i_ai][location];
                int32_t ship_cnt = 1;
                if (best_num_escorts > 0) {
                    for (int32_t i = 0; i < best_num_escorts; ++i) {
                        int16_t s_idx = (*AIDATA::_all_ships)[all_ship_idx++];
                        MOX::_ship[s_idx].mission = 4;
                        fleet[ship_cnt++] = s_idx;
                    }
                }
                fleet[ship_cnt] = -1;
                if (best_num_escorts < best_num_escorts_total) {
                    (*AIDATA::_i_all_combat_ships)[AIMOVE::_i_ai][location] += (int16_t)best_num_escorts;
                }
                else {
                    (*AIDATA::_i_all_combat_ships)[AIMOVE::_i_ai][location] = -1;
                }
            }

            if (best_strategy != 0) {
                s_ship_move_info move_info;
                SHIPMOVE::Ships_Try_To_Move_To_(fleet, (int16_t)best_dest, &move_info);
                SHIPMOVE::Make_Ships_Move_To_(fleet, (int16_t)best_dest, &move_info);
            }

            int16_t count = *num_colony_ships;
            *num_colony_ships = count - 1;
            colony_ships[best_colony_ship_idx] = colony_ships[count - 1];
        }

        return success;
    }

    void __cdecl Find_Opportunity_Attack_Aux_(
        int16_t player_id,
        int16_t target_player_idx,
        int16_t min_value,
        int16_t star_idx,
        int16_t* out_target_star,
        int16_t* out_value,
        int32_t* out_ratio
    ) {
        *out_target_star = -1;

        if ((*AIDATA::_i_all_combat_ships)[player_id][star_idx] != -1 &&
            MOX::_settings.game_difficulty > 1) {
            int8_t flee_stars[MAX_STARS * ENEMY_STRENGTH_TURN_WINDOW];

            s_attack_info attack_info;

            attack_info.target_player = target_player_idx;
            attack_info.min_value = min_value;
            attack_info.target_planet = -1;

            _flee_stars = flee_stars;
            _i_ai = player_id;
            _p_ai = &MOX::_player[player_id];

            Find_Best_Attack_From_Star_(star_idx, &attack_info, 1, 2);

            if (attack_info.target_planet != -1) {
                *out_target_star = attack_info.target_planet;
                *out_value = attack_info.value_a;
                *out_ratio = (int32_t)attack_info.value_b;
            }

            _flee_stars = nullptr;
        }
    }

    int __cdecl Sort_candidate_stars_(const void* a, const void* b) {
        const uint8_t* candidate_star_a = (uint8_t*)a;
        const uint8_t* candidate_star_b = (uint8_t*)b;
        uint32_t weakest_strength_a = 0xffffffffu;
        uint32_t weakest_strength_b = 0xffffffffu;

        if (MOX::_NUM_PLAYERS > 0) {
            for (int16_t player_idx = 0; player_idx < MOX::_NUM_PLAYERS; ++player_idx) {
                if (player_idx != AIMOVE::_i_ai) {
                    const uint8_t star_a = *candidate_star_a;
                    if (((*AIDATA::_star_in_range_of_player)[star_a] >> (player_idx & 0x1f)) & 1) {
                        const uint32_t strength_a =
                            (uint32_t)(*AIDATA::_possible_ship_strengths)[star_a][AIMOVE::_i_ai][player_idx][0];
                        if (strength_a < weakest_strength_a) {
                            weakest_strength_a = strength_a;
                        }

                        const uint8_t star_b = *candidate_star_b;
                        const uint32_t strength_b =
                            (uint32_t)(*AIDATA::_possible_ship_strengths)[star_b][AIMOVE::_i_ai][player_idx][0];
                        if (strength_b < weakest_strength_b) {
                            weakest_strength_b = strength_b;
                        }
                    }
                }
            }
        }

        if (weakest_strength_a == 0xffffffffu) {
            weakest_strength_a = 0;
        }
        if (weakest_strength_b == 0xffffffffu) {
            weakest_strength_b = 0;
        }

        if (weakest_strength_b > weakest_strength_a) {
            return -1;
        }

        return weakest_strength_b < weakest_strength_a;
    }
}