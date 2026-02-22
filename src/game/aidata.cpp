#include "deps.h"

namespace AIDATA {

    void Compute_AI_Data_() {
        RUSS::Assert_Settings_();
        allocate::Push_Block_(MOX::_screen_seg);
        _ship_strengths = (int16_t(*)[500][8])allocate::Allocate_Next_Block_(MOX::_screen_seg, 8000);
        _ship_strengths_in_nebula = (int16_t(*)[500][8])allocate::Allocate_Next_Block_(MOX::_screen_seg, 8000);
        _ship_bomb_strengths = (int32_t(*)[500][2])allocate::Allocate_Next_Block_(MOX::_screen_seg, 4000);
        _planet_stage_weights = (int16_t(*)[360][8])allocate::Allocate_Next_Block_(MOX::_screen_seg, 5760);
        _base_star_stage_weights = (int32_t(*)[72][8])allocate::Allocate_Next_Block_(MOX::_screen_seg, 2304);
        _star_stage_weights = (int32_t(*)[72][8])allocate::Allocate_Next_Block_(MOX::_screen_seg, 2304);
        _n_turns_from_to = (int8_t(*)[72][72][8])allocate::Allocate_Next_Block_(MOX::_screen_seg, 41472 /* 0xA200 */);
        _n_turns_from_to_intermediate = (int32_t(*)[72][72][8])allocate::Allocate_Next_Block_(MOX::_screen_seg, 165888 /* 0x28800 */);
        _ship_n_turns_to = (int8_t(*)[500][72])allocate::Allocate_Next_Block_(MOX::_screen_seg, 36000);
        _ship_n_turns_to_intermediate = (int16_t(*)[500][72])allocate::Allocate_Next_Block_(MOX::_screen_seg, 72000);
        _possible_ship_strengths = (int32_t(*)[72][8][8][10])allocate::Allocate_Next_Block_(MOX::_screen_seg, 184320);
        _ai_colony_info = (s_ai_colony_info(*)[250])allocate::Allocate_Next_Block_(MOX::_screen_seg, 1750 /* 0x6D6 */);
        _ai_player_info = (s_ai_player_info(*)[8])allocate::Allocate_Next_Block_(MOX::_screen_seg, 464 /* 0x1D0 */);
        _ai_star_info = (s_ai_star_info(*)[72])allocate::Allocate_Next_Block_(MOX::_screen_seg, 3528 /* 0xDC8 */);
        _star_in_range_of_player = (uint8_t(*)[72])allocate::Allocate_Next_Block_(MOX::_screen_seg, 72);
        _star_in_extended_range_of_player = (uint8_t(*)[72])allocate::Allocate_Next_Block_(MOX::_screen_seg, 72);
        _enroute_ship_strengths = (int32_t(*)[72][8][8][10])allocate::Allocate_Next_Block_(MOX::_screen_seg, 184320);
        _enroute_ships = (int8_t(*)[72][8][10])allocate::Allocate_Next_Block_(MOX::_screen_seg, 5760);
        _enroute_human_ships = (int8_t(*)[72][10])allocate::Allocate_Next_Block_(MOX::_screen_seg, 720);
        _enroute_nonstealthy_ships = (int8_t(*)[72][8])allocate::Allocate_Next_Block_(MOX::_screen_seg, 576);
        _all_ships = (int16_t(*)[500])allocate::Allocate_Next_Block_(MOX::_screen_seg, 1000);
        _base_planet_values = &_g_base_planet_values;
        _i_all_combat_ships = (int16_t(*)[8][73])allocate::Allocate_Next_Block_(MOX::_screen_seg, 1168);
        _aitech = (s_ai_tech_info*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 168);
        _airefit = (s_ai_refit_player(*)[8])allocate::Allocate_Next_Block_(MOX::_screen_seg, 24016 /* 0x5DD0 */);
        _ship_refit_desirabilities = (int32_t(*)[500])allocate::Allocate_Next_Block_(MOX::_screen_seg, 2000);

        memset(_ai_player_info, 0, sizeof(*_ai_player_info));
        MOX::Mox_Update_();
        memset(_ship_strengths, 0, sizeof(*_ship_strengths));
        memset(_ship_strengths_in_nebula, 0, sizeof(*_ship_strengths_in_nebula));
        memset(_ship_bomb_strengths, 0, sizeof(*_ship_bomb_strengths));
        memset(_enroute_ship_strengths, 0, sizeof(*_enroute_ship_strengths));
        memset(_enroute_ships, 0, sizeof(*_enroute_ships));
        memset(_enroute_human_ships, 0, sizeof(*_enroute_human_ships));
        memset(_enroute_nonstealthy_ships, 0, sizeof(*_enroute_nonstealthy_ships));

        for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
            s_ship_data& ship = MOX::_ship[i];
            if (ship.status < 3) {
                int owner = ship.owner;
                int star_idx = -1;
                int turn_idx = -1;
                if (ship.status == 0) {
                    star_idx = ship.location;
                    turn_idx = 0;
                } else if (ship.status == 1) {
                    star_idx = ship.location - 500;
                    turn_idx = (int)ship.turns_left;
                } else if (ship.status == 2) {
                    star_idx = HAROLD::XY_To_Star_Id_(ship.x, ship.y);
                    turn_idx = 0;
                }

                if (star_idx < MOX::_NUM_STARS && star_idx >= 0) {
                    bool in_nebula = MOX::_star[star_idx].in_nebula != 0;
                    if (owner < 8 && turn_idx < 10 && MOX::_player[owner].objectives == 100) {
                        (*_enroute_human_ships)[star_idx][turn_idx] = 1;
                    }
                    for (int p = 0; p < MOX::_NUM_PLAYERS; ++p) {
                        if (owner != p && MOX::_player[p].eliminated == 0) {
                            if (ship.d.ship_type == 0) {
                                int16_t str = aipower::Ship_Strength_Vs_Ships_(i, p);
                                (*_ship_strengths)[i][p] = str;
                                (*_ai_player_info)[p].total_ship_strength += str;
                                (*_ship_strengths_in_nebula)[i][p] = aipower::Ship_Strength_Vs_Ships_In_Nebula_(i, p);
                                aipower::Ship_Strength_Vs_Planetary_Shield_(i, &(*_ship_bomb_strengths)[i][0]);
                                MOX::Mox_Update_();
                                if (owner < 8 && turn_idx < 10) {
                                    int16_t cur_str = in_nebula ? (*_ship_strengths_in_nebula)[i][p] : (*_ship_strengths)[i][p];
                                    (*_enroute_ship_strengths)[star_idx][owner][p][turn_idx] += cur_str;
                                }
                            }
                            if (owner < 8) {
                                if (turn_idx < 10) {
                                    (*_enroute_ships)[star_idx][owner][turn_idx] = 1;
                                }
                                if (MOX::_player[owner].traits[TRAIT_STEALTHY_SHIPS] == 0 && (*_enroute_nonstealthy_ships)[star_idx][owner] == 0) {
                                    if (!SHIPSTAK::Ship_Has_Stealth_Device_(i)) {
                                        (*_enroute_nonstealthy_ships)[star_idx][owner] = 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        for (int p = 0; p < MOX::_NUM_PLAYERS; ++p) {
            memset(MOX::_player[p].total_ship_strength, 0, 32);
        }

        for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
            int owner = MOX::_ship[i].owner;
            if (owner < 8) {
                for (int p = 0; p < MOX::_NUM_PLAYERS; ++p) {
                    MOX::_player[owner].total_ship_strength[p] += (*_ship_strengths)[i][p];
                }
            }
        }

        MOX::Mox_Update_();
        for (int s = 0; s < MOX::_NUM_STARS; ++s) {
            for (int p_o = 0; p_o < MOX::_NUM_PLAYERS; ++p_o) {
                for (int t = 0; t < 9; ++t) {
                    for (int p_v = 0; p_v < MOX::_NUM_PLAYERS; ++p_v) {
                        (*_enroute_ship_strengths)[s][p_o][p_v][t + 1] += (*_enroute_ship_strengths)[s][p_o][p_v][t];
                        if ((*_enroute_ships)[s][p_o][t]) {
                            (*_enroute_ships)[s][p_o][t + 1] = 1;
                        }
                    }
                    if ((*_enroute_human_ships)[s][t]) {
                        (*_enroute_human_ships)[s][t + 1] = 1;
                    }
                }
            }
        }

        MOX::Mox_Update_();
        int32_t range_sq[8], ext_range_sq[8];
        for (int p = 0; p < MOX::_NUM_PLAYERS; ++p) {
            int32_t r = MOX::_player[p].ship_range;
            range_sq[p] = (r * 30) * (r * 30);
            int32_t ext_r = (r * 45) * (r * 45);
            ext_range_sq[p] = ext_r * ext_r;
        }

        memset(_star_in_range_of_player, 0, 72);
        memset(_star_in_extended_range_of_player, 0, 72);

        for (int c = 0; c < MOX::_NUM_COLONIES; ++c) {
            s_colony& col = MOX::_colony[c];
            if (col.planet != -1) {
                int star_idx = MOX::_planet[col.planet].star_index;
                int16_t sx = MOX::_star[star_idx].x;
                int16_t sy = MOX::_star[star_idx].y;
                for (int s = 0; s < MOX::_NUM_STARS; ++s) {
                    int32_t dx = sx - MOX::_star[s].x;
                    int32_t dy = sy - MOX::_star[s].y;
                    int32_t d2 = dx * dx + dy * dy;
                    if (d2 <= ext_range_sq[col.owner]) {
                        if (!struct_::Test_Bit_Field_(MOX::_star[star_idx].black_hole_blocks, s)) {
                            (*_star_in_extended_range_of_player)[s] |= (1 << col.owner);
                            if (d2 <= range_sq[(int)col.owner] && !struct_::Test_Bit_Field_(MOX::_star[star_idx].black_hole_blocks, s)) {
                                (*_star_in_range_of_player)[s] |= (1 << col.owner);
                            }
                        }
                    }
                }
            }
        }

        MOX::Mox_Update_();
        memset(_n_turns_from_to, -1, sizeof(*_n_turns_from_to));
        memset(_n_turns_from_to_intermediate, -1, sizeof(*_n_turns_from_to_intermediate));
        for (int s1 = 0; s1 < MOX::_NUM_STARS; ++s1) {
            for (int p = 0; p < MOX::_NUM_PLAYERS; ++p) {
                if (((*_star_in_extended_range_of_player)[s1] >> p) & 1) {
                    for (int s2 = 0; s2 < MOX::_NUM_STARS; ++s2) {
                        if (((*_star_in_extended_range_of_player)[s2] >> p) & 1) {
                            (*_n_turns_from_to)[s1][s2][p] = (int8_t)SHIPMOVE::Player_N_Turns_From_Star_To_Star_((int16_t)p, (int16_t)s1, (int16_t)s2);
                            MOX::Mox_Update_();
                        }
                    }
                }
            }
        }

        MOX::Mox_Update_();
        memset(_ship_n_turns_to, -1, sizeof(*_ship_n_turns_to));
        memset(_ship_n_turns_to_intermediate, -1, sizeof(*_ship_n_turns_to_intermediate));
        for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
            s_ship_data& ship = MOX::_ship[i];
            if (ship.status < 5 && ship.status == 1) {
                int dest_star = SHIPMOVE::Ship_Destination_Star_(&ship);
                (*_ship_n_turns_to)[i][dest_star] = ship.turns_left;
                if (SHIPMOVE::Can_Order_Ship_(&ship)) {
                    uint8_t mask = (1 << ship.owner);
                    int16_t move_info[2] = { (int16_t)i, -1 };
                    s_ship_move_info out_info;
                    for (int s = 0; s < MOX::_NUM_STARS; ++s) {
                        if (((*_star_in_extended_range_of_player)[s] & mask) && s != dest_star) {
                            SHIPMOVE::Ships_Try_To_Move_To_(move_info, (int16_t)s, &out_info);
                            (*_ship_n_turns_to)[i][s] = out_info.turns_left;
                            MOX::Mox_Update_();
                        }
                    }
                }
            }
        }

        MOX::Mox_Update_();
        memset(_ai_colony_info, 0, sizeof(*_ai_colony_info));
        for (int c = 0; c < MOX::_NUM_COLONIES; ++c) {
            s_colony& col = MOX::_colony[c];
            if (col.owner != -1) {
                s_ai_colony_info& ai_col = (*_ai_colony_info)[c];
                MOX::Mox_Update_();
                for (int p_idx = 0; p_idx < col.n_pops; ++p_idx) {
                    if (col.pop[p_idx] & 0x400) {
                        ai_col.n_flagged_pops++;
                    }
                }
                ai_col.race = (int8_t)Colony_Dominant_Player_(&col.owner);
                ai_col.pop_limit = (int8_t)COLCALC::Colony_Race_Pop_Limit_(&col, (int16_t)ai_col.race);
                if (MOX::_player[ai_col.race].traits[TRAIT_EATS_MINERALS] == 0 || MOX::_player[col.owner].traits[TRAIT_EATS_MINERALS] == 0) {
                    ai_col.trait_eats_minerals = 1;
                }
                if (MOX::_player[(int)ai_col.race].traits[TRAIT_ENVIRONMENT_IMMUNE] == 0) {
                    ai_col.trait_environment_immune = 1;
                }
            }
        }

        for (int c = 0; c < MOX::_NUM_COLONIES; ++c) {
            s_colony& col = MOX::_colony[c];
            if (col.owner != -1) {
                int owner = col.owner;
                int star_idx = MOX::_planet[col.planet].star_index;
                MOX::Mox_Update_();
                for (int p = MOX::_NUM_PLAYERS - 1; p >= 0; --p) {
                    if (p != owner && (*_enroute_ships)[star_idx][p][9]) {
                        (*_ai_colony_info)[c].threatened = 1;
                        break;
                    }
                }
            }
        }

        MOX::Mox_Update_();
        memset(_ai_star_info, 0, sizeof(*_ai_star_info));
        for (int s = 0; s < MOX::_NUM_STARS; ++s) {
            s_ai_star_info& ai_star = (*_ai_star_info)[s];
            for (int pl_i = 0; pl_i < 5; ++pl_i) {
                if (MOX::_star[s].planet_index[pl_i] != -1) {
                    ai_star.n_planets_present++;
                }
            }
            for (int p = 0; p < MOX::_NUM_PLAYERS; ++p) {
                if (MOX::_player[p].objectives != 100 && MOX::_player[p].eliminated == 0 && (((*_star_in_extended_range_of_player)[s] >> p) & 1)) {
                    for (int other_p = MOX::_NUM_PLAYERS - 1; other_p >= 0; --other_p) {
                        if (other_p != p && (*_enroute_ships)[s][other_p][9]) {
                            ai_star.per_player[p].enemy_enroute_or_threat = 1;
                            break;
                        }
                    }
                }
            }
        }

        Compute_Star_Player_Range_Info_();

        MOX::Mox_Update_();
        for (int p1 = 0; p1 < MOX::_NUM_PLAYERS; ++p1) {
            MOX::Mox_Update_();
            for (int s = 0; s < MOX::_NUM_STARS; ++s) {
                if ((MOX::_star[s].has_colony >> p1) & 1) {
                    (*_ai_player_info)[p1].n_stars_with_colony++;
                    if ((MOX::_star[s].blockaded >> p1) & 1) {
                        (*_ai_player_info)[p1].n_blockaded_stars++;
                    }
                }
            }
            for (int p2 = 0; p2 < MOX::_NUM_PLAYERS; ++p2) {
                if (p1 == p2) continue;
                if (MOX::_player[p1].contact[p2]) {
                    (*_ai_player_info)[p1].n_contacts++;
                    (*_ai_player_info)[p1].reachable_players[p2] = 1;
                } else {
                    bool reachable = false;
                    uint8_t mask = (1 << p1) | (1 << p2);
                    for (int s = 0; s < MOX::_NUM_STARS; ++s) {
                        if ((*_ai_star_info)[s].n_planets_present != 0 && (((*_star_in_extended_range_of_player)[s] & mask) == mask)) {
                            reachable = true;
                            break;
                        }
                    }
                    if (reachable) {
                        (*_ai_player_info)[p1].n_missing_contacts++;
                        (*_ai_player_info)[p1].reachable_players[p2] = 1;
                    }
                }
            }
        }

        MOX::Mox_Update_();
        for (int c = 0; c < MOX::_NUM_COLONIES; ++c) {
            s_colony& col = MOX::_colony[c];
            if (col.owner != -1) {
                (*_ai_player_info)[(int)col.owner].colony_count++;
                if ((MOX::_star[MOX::_planet[col.planet].star_index].blockaded >> col.owner) & 1) {
                    (*_ai_player_info)[(int)col.owner].colony_blockaded_count++;
                }
            }
        }

        MOX::Mox_Update_();
        int16_t max_colonies = 0;
        for (int p = 0; p < MOX::_NUM_PLAYERS; ++p) {
            if ((*_ai_player_info)[p].colony_count > max_colonies) {
                max_colonies = (*_ai_player_info)[p].colony_count;
            }
        }
        for (int p = 0; p < MOX::_NUM_PLAYERS; ++p) {
            (*_ai_player_info)[p].colony_deficit = max_colonies - (*_ai_player_info)[p].colony_count;
        }

        MOX::Mox_Update_();
        Compute_Base_Planet_Values_();
        MOX::Mox_Update_();
        Compute_Contextual_Planet_Values_();
        MOX::Mox_Update_();
        aitech::Compute_AI_Tech_Info_(_aitech);
        MOX::Mox_Update_();

        _n_all_ships = 0;
        memset(_airefit, 0, sizeof(*_airefit));
        memset(_ship_refit_desirabilities, 0, sizeof(*_ship_refit_desirabilities));

        for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
            s_ship_data* ship = &MOX::_ship[i];
            if (ship->d.ship_type == 0 && ship->owner < MOX::_NUM_PLAYERS && ship->status < 3 && ship->mission != 4 /* SHIP_MISSION_SCRAP */) {
                (*_all_ships)[_n_all_ships++] = (int16_t)i;
                if (ship->status == 0) {
                    MOX::Mox_Update_();
                    int32_t d = Refit_Desirability_(ship);
                    (*_ship_refit_desirabilities)[i] = d;
                    s_ai_refit_player& refit_p = (*_airefit)[ship->owner];
                    refit_p.entry[refit_p.n_entries].ship_idx = (int16_t)i;
                    refit_p.entry[refit_p.n_entries].desirability = (int16_t)d;
                    refit_p.entry[refit_p.n_entries].field0_0x0 = (int16_t)i;
                    refit_p.n_entries++;
                }
            }
        }

        for (int p = 0; p < MOX::_NUM_PLAYERS; ++p) {
            MOX::Mox_Update_();
            _qsort((*_airefit)[p].entry, (*_airefit)[p].n_entries, 6, airefit_cmp_);
        }

        MOX::Mox_Update_();
        memset(_i_all_combat_ships, -1, sizeof(*_i_all_combat_ships));
        
        _qsort(_all_ships, _n_all_ships, 2, sort_ships_);

        for (int i = 0; i < _n_all_ships; ++i) {
            int ship_idx = (*_all_ships)[i];
            int loc = MOX::_ship[ship_idx].location;
            int owner = MOX::_ship[ship_idx].owner;
            if (loc < 72) {
                if ((*_i_all_combat_ships)[owner][loc] == -1) {
                    (*_i_all_combat_ships)[owner][loc] = (int16_t)i;
                }
            } else {
                if ((*_i_all_combat_ships)[owner][MOX::_NUM_STARS + loc - 500] == -1) {
                    (*_i_all_combat_ships)[owner][MOX::_NUM_STARS + loc - 500] = (int16_t)i;
                }
            }
        }

        MOX::Mox_Update_();

        memset(_possible_ship_strengths, 0, sizeof(*_possible_ship_strengths));

        for (int i_as = 0; i_as < _n_all_ships; ++i_as) {
            int i = (*_all_ships)[i_as];
            bool stealth = struct_::Test_Bit_Field_(MOX::_ship[i].d.special_device_flags, 11);
            int start_star = MOX::_ship[i].location;
            int owner = MOX::_ship[i].owner;
            int initial_delay = 0;
            
            MOX::Mox_Update_();

            if (MOX::_ship[i].status == 10) { // SHIP_STATUS_ANTARAN
                initial_delay = 0;
            } else if (start_star < MOX::_NUM_STARS) {
                initial_delay = 0;
            } else if (start_star < 1000) {
                start_star -= 500;
                initial_delay = (int)MOX::_ship[i].turns_left;
            } else {
                start_star = HAROLD::XY_To_Star_Id_(MOX::_ship[i].x, MOX::_ship[i].y);
            }

            bool in_nebula = MOX::_star[start_star].in_nebula != 0;

            for (int p = 0; p < MOX::_NUM_PLAYERS; ++p) {
                if (p != owner) {
                    for (int s = 0; s < MOX::_NUM_STARS; ++s) {
                        if ((((*_star_in_extended_range_of_player)[s] >> p) & 1) || s == start_star) {
                            int turns = Player_N_Turns_From_Star_To_Star_With_Intermediate_(owner, (int16_t)start_star, (int16_t)s, nullptr, stealth);
                            if (turns != -1) {
                                int eta = initial_delay + turns;
                                if (eta < 10) {
                                    int16_t str = in_nebula ? (*_ship_strengths_in_nebula)[i][p] : (*_ship_strengths)[i][p];
                                    for (int t = eta; t < 10; ++t) {
                                        (*_possible_ship_strengths)[s][owner][p][t] += str;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        MOX::Mox_Update_();
        memset(_planet_stage_weights, 0, sizeof(*_planet_stage_weights));
        for (int pl = 0; pl < MOX::_planet_count; ++pl) {
            if (MOX::_planet[pl].colony_index != -1) {
                int owner = MOX::_colony[MOX::_planet[pl].colony_index].owner;
                (*_planet_stage_weights)[pl][owner] = (*_base_planet_values)[owner][pl];
            }
        }

        MOX::Mox_Update_();
        memset(_star_stage_weights, 0, sizeof(*_star_stage_weights));
        memset(_base_star_stage_weights, 0, sizeof(*_base_star_stage_weights));
        int16_t best_planet_at_star[72][8];
        int16_t best_planet_val_at_star[72][8];
        memset(best_planet_at_star, -1, sizeof(best_planet_at_star));
        memset(best_planet_val_at_star, 0, sizeof(best_planet_val_at_star));

        uint8_t player_treaty_matrix[8][8];
        memset(player_treaty_matrix, 0, sizeof(player_treaty_matrix));

        for (int i = 0; i < MOX::_NUM_PLAYERS; i++) {
            int flag = 0;
            for (int j = 0; j < MOX::_NUM_PLAYERS; j++) {
                if (MOX::_player[j].objectives == 100) {
                    player_treaty_matrix[i][j] = 1;
                }
                if (MOX::_player[i].treaty[j] > 3 && MOX::_player[i].treaty[j] < 7) {
                    flag = 1;
                }
            }
            if (!flag) {
                memset(player_treaty_matrix[i], 1, sizeof(player_treaty_matrix[i]));
            }

            player_treaty_matrix[i][i] = 0;
        }

        for (int s = 0; s < MOX::_NUM_STARS; ++s) {
            if (MOX::_star[s].has_colony) {
                MOX::Mox_Update_();
                for (int pl_i = 0; pl_i < 5; ++pl_i) {
                    int pl_idx = MOX::_star[s].planet_index[pl_i];
                    if (pl_idx != -1 && MOX::_planet[pl_idx].colony_index != -1) {
                        int owner = MOX::_colony[MOX::_planet[pl_idx].colony_index].owner;
                        int16_t val = (*_planet_stage_weights)[pl_idx][owner];
                        if (val > best_planet_val_at_star[s][owner]) {
                            best_planet_at_star[s][owner] = (int16_t)pl_idx;
                            best_planet_val_at_star[s][owner] = val;
                        }
                    }
                }
                for (int p_o = 0; p_o < MOX::_NUM_PLAYERS; ++p_o) {
                    if (best_planet_at_star[s][p_o] != -1) {
                        int32_t threat_sum = 0;
                        int32_t value_sum = 0;

                        for (int p_v = 0; p_v < MOX::_NUM_PLAYERS; ++p_v) {
                            if (player_treaty_matrix[p_o][p_v]) {
                                int16_t max_eta = 8;
                                if ((*_ai_player_info)[p_v].n_stars_with_colony * 5 < (*_ai_player_info)[p_o].n_stars_with_colony) {
                                    max_eta = ((*_ai_player_info)[p_v].n_stars_with_colony <= 1) ? 1 : 4;
                                }
                                MOX::Mox_Update_();
                                for (int other_s = 0; other_s < MOX::_NUM_STARS; ++other_s) {
                                    int32_t str = (*_possible_ship_strengths)[other_s][p_v][p_o][0];
                                    if (str != 0) {
                                        int turns = Player_N_Turns_From_Star_To_Star_With_Intermediate_((int16_t)p_v, (int16_t)other_s, (int16_t)s, nullptr, true);
                                        if (turns != -1 && turns < max_eta) {
                                            threat_sum += str / (turns + 1);
                                        }
                                    }
                                }
                            }
                        }

                        for (int pl = 0; pl < MOX::_planet_count; ++pl) {
                            if (pl != best_planet_at_star[s][p_o] && Player_N_Turns_From_Star_To_Star_With_Intermediate_((int16_t)p_o, (int16_t)s, (int16_t)MOX::_planet[pl].star_index, nullptr, false) < 3) {
                                MOX::Mox_Update_();
                                value_sum += (*_planet_stage_weights)[pl][p_o];
                            }
                        }
                        int32_t base_weight = best_planet_val_at_star[s][p_o] + (value_sum >> 4);
                        (*_base_star_stage_weights)[s][p_o] = base_weight;
                        (*_star_stage_weights)[s][p_o] = ((base_weight + 7) >> 3) * ((threat_sum + 7) >> 3);
                    }
                }
            }
        }

        MOX::Mox_Update_();
        for (int p = 0; p < MOX::_NUM_PLAYERS; ++p) {
            AISTAGE::Compute_Defensive_Stage_Points_((int16_t)p);
        }

        for (int p = 0; p < MOX::_NUM_PLAYERS; ++p) {
            for (int s = 0; s < MOX::_NUM_STARS; ++s) {
                int8_t target = MOX::_star[s].surrender_to[p];
                if (target > 7) {
                    int target_p = target - 8;
                    MOX::_star[s].surrender_to[p] = -1;
                    if (MOX::_player[p].eliminated == 0 && MOX::_player[target_p].eliminated == 0) {
                        if (COLCALC::May_Surrender_Star_((int16_t)s, (int16_t)p, (int16_t)target_p)) {
                            COLCALC::Surrender_Star_((int16_t)s, (int16_t)p, (int16_t)target_p);
                        }
                    }
                }
            }
        }

        RUSS::Assert_Settings_();
        for (int s = 0; s < MOX::_NUM_STARS; ++s) {
            MOX::_star[s].just_visited = 0;
        }
    }
}