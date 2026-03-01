#include "pch.h"

namespace aimove {
    int __cdecl Sort_colonizable_(const void* a, const void* b) {
        auto p1 = static_cast<const s_ai_colonizable_planet*>(a);
        auto p2 = static_cast<const s_ai_colonizable_planet*>(b);
        return p1->contextual_value - p2->contextual_value;
    }

    void __cdecl Move_All_AI_() {
        for (int32_t i = 0; i < (int32_t)MOX::_NUM_PLAYERS; ++i) {
            s_player& plr = MOX::_player[i];

            if (plr.objectives != 100 && plr.ship_speed != 0) {
                MOX::Mox_Update_();

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

                if ((*AIDATA::_i_all_combat_ships)[_i_ai][(uint8_t)stage_point] != -1) {
                    int16_t ship_list[502];
                    s_ship_move_info move_info;
                    int16_t star_idx = MOX::_planet[target_planet_idx].star_index;

                    int ship_count = Gather_Player_Ships_At_Star_(_i_ai, (int16_t)stage_point, ship_list);
                    SHIPMOVE::Ships_Try_To_Move_To_(ship_list, star_idx, &move_info);

                    if (move_info.moving != 0) {
                        for (int i = 0; i < ship_count; ++i) {
                            MOX::_ship[ship_list[i]].mission = 6;
                        }
                        SHIPMOVE::Make_Ships_Move_To_(ship_list, star_idx, &move_info);
                        (*AIDATA::_i_all_combat_ships)[_i_ai][(uint8_t)stage_point] = -1;
                    }
                }
            }

            int16_t star_idx = MOX::_planet[_p_ai->sneak_attack_planet].star_index;
            if ((*AIDATA::_enroute_ship_strengths)[star_idx][_i_ai][target_player][1] != 0) {
                diplomac::Declare_War_(_i_ai, (int16_t)target_player);
            }
        }
    }

    void __cdecl Move_AI_(int32_t player_idx) {
        int8_t local_nonplayer_here[72];
        int8_t local_flee_stars[720];
        s_ai_colonizable_planet local_colonizable[360];

        MOX::Mox_Update_();

        _i_ai = (int16_t)player_idx;
        _colonizable = local_colonizable;
        _p_ai = &MOX::_player[player_idx];

        Get_Colonizable_Planets_(local_colonizable, &_n_colonizable);
        _qsort(_colonizable, _n_colonizable, sizeof(s_ai_colonizable_planet), Sort_colonizable_);

        _flee_stars = (int8_t(*)[720])local_flee_stars;
        memset(_flee_stars, 0, 720);
        Mark_Flee_Stars_(static_cast<int16_t>(player_idx));

        _nonplayer_here = (int8_t(*)[72])local_nonplayer_here;
        memset(_nonplayer_here, 0, 72);

        for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
            s_ship_data* ship = &MOX::_ship[i];
            // Check for aliens, monsters, or other non-player factions (owner >= 8) and currently not on a mission
            if (ship->owner >= 8 && ship->status == 0) {
                (*_nonplayer_here)[ship->location] = ship->owner;
            }
        }

        MOX::Mox_Update_();
        Delete_Already_Assigned_Planets_(_colonizable, &_n_colonizable);
        MOX::Mox_Update_();
        Launch_Sneak_Attack_();
        MOX::Mox_Update_();
        Launch_Attacks_(6, 2);
        MOX::Mox_Update_();
        Launch_Attacks_(1, 2);
        MOX::Mox_Update_();
        Intercept_Invaders_();
        MOX::Mox_Update_();
        Assign_New_Colony_Ship_Destinations_(_colonizable, &_n_colonizable);
        MOX::Mox_Update_();
        Launch_Attacks_(2, 2);
        MOX::Mox_Update_();
        Launch_Attacks_(6, 0);
        MOX::Mox_Update_();
        Launch_Attacks_(1, 0);
        MOX::Mox_Update_();
        Launch_Attacks_(2, 0);
        MOX::Mox_Update_();
        Move_Transports_();
        MOX::Mox_Update_();
        Move_To_Stage_Points_();
        MOX::Mox_Update_();

        s_ai_player_info* ai_info = &(*AIDATA::_ai_player_info)[_i_ai];
        // BUG: prev_n_contacts not found in s_ai_player_info, using n_contacts as placeholder
        if (ai_info->n_contacts == ai_info->n_contacts || MOX::_settings.game_difficulty == 0) {
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
            // total_ship_strength is an array of 8 ints starting at offset 0x1A of s_ai_player_info
            int32_t* strength_array = reinterpret_cast<int32_t*>(reinterpret_cast<uintptr_t>(&(*AIDATA::_ai_player_info)[plr1]) + 0x1A);
            uint32_t current_strength = static_cast<uint32_t>(strength_array[plr2]);
            uint32_t possible_strength = static_cast<uint32_t>((*AIDATA::_possible_ship_strengths)[i][plr1][plr2][0]);

            // AI logic: check if existing strength ratio allows for a threat/attack opportunity
            if ((current_strength >> 2) <= (possible_strength / 10)) {
                int16_t found_planet;
                int16_t dummy_short;
                int32_t found_aux;

                Find_Opportunity_Attack_Aux_((int16_t)plr1, (int16_t)plr2, 150 /* 0x96 */, (int16_t)i, &found_planet, &dummy_short, &found_aux);

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
        s_ai_colonizable_planet colonizable_planets[360];
        int16_t best_planet_idx = -1;

        _i_ai = player_idx;
        _colonizable = colonizable_planets;
        _p_ai = &MOX::_player[player_idx];

        Get_Colonizable_Planets_(colonizable_planets, &_n_colonizable);

        _qsort(_colonizable, (uint32_t)_n_colonizable, sizeof(s_ai_colonizable_planet), Sort_colonizable_);

        Delete_Already_Assigned_Planets_(_colonizable, &_n_colonizable);

        if (_n_colonizable > 0) {
            best_planet_idx = _colonizable[_n_colonizable - 1].planet_idx;
        }

        _colonizable = nullptr;
        return best_planet_idx;
    }

    void __cdecl Mark_Flee_Stars_(int16_t player_idx) {
        // Iterate through all players to check for active blockading durations.
        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            // Check if player i is currently being asked to stop blockading by player_idx.
            if (MOX::_player[player_idx].stop_blockading_duration[i] != 0) {
                // If so, iterate through stars to identify retreat/flee targets.
                for (int star_idx = 0; star_idx < (int)MOX::_NUM_STARS; ++star_idx) {
                    // Each star entry in flee_stars is 10 bytes wide.
                    int8_t* star_flee_ptr = &(*_flee_stars)[star_idx * 10];

                    // Only mark the star if it hasn't been processed yet.
                    if (star_flee_ptr[0] == 0) {
                        // Check if player i has a colony at this star (has_colony is a bitmask).
                        if ((MOX::_star[star_idx].has_colony >> i) & 1) {
                            // Mark all 10 bytes for this star as fleeing locations.
                            memset(star_flee_ptr, 1, 10);
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
        int16_t planet_count = (int16_t)MOX::_planet_count;
        s_planet_data* planets = MOX::_planet;

        for (int i = 0; i < planet_count; ++i) {
            s_planet_data* planet = &planets[i];

            // Only consider uncolonized planets. 
            // planet_type == 3 is a standard colonizable planet (excluding gas giants, asteroids).
            if (planet->colony_index == -1 && planet->planet_type == 3) {
                // Each byte in the array represents a star, with bits corresponding to players
                // who have that star within their extended travel range.
                uint8_t star_range_mask = (*AIDATA::_star_in_extended_range_of_player)[(uint8_t)planet->star_index];

                if ((1 << (aimove::_i_ai & 0x1F)) & (uint32_t)star_range_mask) {
                    int16_t current_count = *count;

                    colonizable_planets[current_count].planet_idx = (int16_t)i;

                    // Lookup contextual value for the planet relative to the current AI player.
                    // The table contains 360 entries per player (2880 total).
                    int contextual_idx = i + (static_cast<int>(aimove::_i_ai) * 360);
                    uint16_t val = static_cast<uint16_t>((*AIDATA::_g_contextual_planet_values)[contextual_idx]);
                    colonizable_planets[current_count].contextual_value = static_cast<int32_t>(val);

                    *count = current_count + 1;
                }
            }
        }
    }

    void __cdecl Delete_Already_Assigned_Planets_(s_ai_colonizable_planet* colonizable_planets, int16_t* num_planets) {
        if (_p_ai->tech_applications[TECH_APP_HYPERSPACE_COMMUNICATIONS] != 3) {
            for (int i = 0; i < (int)MOX::_NUM_SHIPS; ++i) {
                s_ship_data* ship = &MOX::_ship[i];

                if (ship->owner == _i_ai &&
                    ship->d.ship_type == SHIP_TYPE_COLONY &&
                    ship->status == 1 &&
                    !SHIPMOVE::Can_Order_Ship_(ship))
                {
                    int16_t star_idx = ship->location - 500;
                    for (int j = *num_planets - 1; j >= 0; --j) {
                        if (MOX::_planet[colonizable_planets[j].planet_idx].star_index == star_idx) {
                            int16_t new_count = *num_planets - 1;
                            *num_planets = new_count;
                            memcpy(&colonizable_planets[j], &colonizable_planets[j + 1], (new_count - j) * sizeof(s_ai_colonizable_planet));
                            break;
                        }
                    }
                }
            }
        }

        if (MOX::_player[_i_ai].objectives != 100 /* 'd' */) {
            for (int i = 0; i < (int)MOX::_NUM_COLONIES; ++i) {
                s_colony* colony = &MOX::_colony[i];

                if (colony->owner == _i_ai && colony->producing[0] == 11 /* PROD_COLONY_SHIP */) {
                    int16_t colony_star_idx = MOX::_planet[colony->planet].star_index;
                    bool found = false;

                    for (int j = *num_planets - 1; j >= 0; --j) {
                        if (MOX::_planet[colonizable_planets[j].planet_idx].star_index == colony_star_idx) {
                            int16_t new_count = *num_planets - 1;
                            *num_planets = new_count;
                            memcpy(&colonizable_planets[j], &colonizable_planets[j + 1], (new_count - j) * sizeof(s_ai_colonizable_planet));
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        colony->producing[0] = -2;
                    }
                }
            }
        }
    }

    void __cdecl Get_Enemy_Strength_On_Turn_(int16_t star_id, int* turn_strengths, int* cumulative_strengths) {
        uint8_t in_nebula = MOX::_star[star_id].in_nebula;

        memset(turn_strengths, 0, 10 * sizeof(int));

        s_ship_data* ships = MOX::_ship;
        int num_ships = MOX::_NUM_SHIPS;

        for (int i = 0; i < num_ships; ++i) {
            s_ship_data* ship = &ships[i];

            if (ship->owner != _i_ai && ship->status < 3) {
                if (ship->location == star_id) {
                    int16_t (*strength_table)[500][8] = in_nebula ? AIDATA::_ship_strengths_in_nebula : AIDATA::_ship_strengths;
                    turn_strengths[0] += (*strength_table)[i][_i_ai];
                } else {
                    int32_t location_diff = (int32_t)ship->location - (int32_t)star_id;
                    // Location diffs of 500 and 1000 relate to travel/wormhole logic
                    if ((location_diff == 500 || location_diff == 1000) && ship->turns_left < 10) {
                        int16_t (*strength_table)[500][8] = in_nebula ? AIDATA::_ship_strengths_in_nebula : AIDATA::_ship_strengths;
                        turn_strengths[ship->turns_left] += (*strength_table)[i][_i_ai];
                    }
                }
            }
        }

        cumulative_strengths[0] = turn_strengths[0];
        for (int t = 1; t < 10; ++t) {
            cumulative_strengths[t] = turn_strengths[t] + cumulative_strengths[t - 1];
        }
    }
}