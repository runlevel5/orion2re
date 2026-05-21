#include "pch.h"

namespace AIDATA {

    int16_t __cdecl Colony_Dominant_Player_(s_colony* colony) {
        uint8_t player_counts[8] = { 0 };
        int total_pops = 0;

        if (colony->n_pops != 0) {
            for (int i = 0; i < (int)colony->n_pops; ++i) {
                uint32_t player_id = colony->pop[i] & 0x0F;
                if (player_id < 8) {
                    total_pops++;
                    player_counts[player_id]++;
                }
            }
        }

        int max_player = (int)MOX::_NUM_PLAYERS - 1;
        int i = 0;
        if (max_player > 0) {
            do {
                if (player_counts[i] > player_counts[max_player]) {
                    max_player = i;
                }
                i++;
            } while (i < max_player);
        }

        uint8_t max_count = player_counts[max_player];

        if (max_count * 2 > total_pops) {
            return (int16_t)max_player;
        }

        int16_t current_owner = (int16_t)colony->owner;
        uint8_t owner_count = player_counts[current_owner];

        if (owner_count * 3 <= total_pops && max_count != 0 && owner_count != max_count) {
            return (int16_t)max_player;
        }

        return current_owner;
    }

    bool __cdecl Star_In_Range_Of_Given_Star_(int16_t target_star_idx, int16_t source_star_idx, int max_range) {
        bool in_range = false;

        // Check direct distance
        int distance = GEO::Parsecs_Between_Stars_(source_star_idx, target_star_idx);
        if (distance <= max_range) {
            return true;
        }

        // Check distance via wormhole if available
        int8_t wormhole_id = MOX::_star[source_star_idx].wormhole_star_id;
        if (wormhole_id != -1) {
            distance = GEO::Parsecs_Between_Stars_(static_cast<int16_t>(wormhole_id), target_star_idx);
            if (distance <= max_range) {
                in_range = true;
            }
        }

        return in_range;
    }

    int32_t __cdecl Refit_Desirability_(s_ship_data* ship) {
        int32_t desirability = 0;
        int owner = (int)ship->owner;

        if (ship->d.previous_owner != owner) {
            return 0;
        }

        s_ai_tech_info_player* ai_tech = &AIDATA::_aitech->players[owner];

        // Evaluate core defensive and utility systems
        if (ai_tech->best_shield > ship->d.shield_type) {
            desirability += (ai_tech->best_shield - ship->d.shield_type) * 10;
        }

        if (ai_tech->best_computer > ship->d.computer_type) {
            desirability += (ai_tech->best_computer - ship->d.computer_type) * 7;
        }

        if (ai_tech->best_armor > ship->d.armor_type) {
            desirability += (ai_tech->best_armor - ship->d.armor_type) * 5;
        }

        uint8_t best_beam_cost = 0;
        uint8_t best_missile_cost = 0;

        for (int i = 0; i < 8; ++i) {
            int weapon_type = (int)ship->d.ship_weapon[i].type;
            if (weapon_type != 0) {
                uint16_t app_id = TECHDATA::_weapons[weapon_type].tech_app_id;
                uint16_t field_id = TECHDATA::_technology_applications[app_id].tech_field_id;
                uint8_t normalized_cost = (uint8_t)(TECHDATA::_technology_fields[field_id].cost / 100);

                int8_t type= TECHDATA::_weapons[weapon_type].type;
                if (type == 0 /* RACK_BEAM */) {
                    if (normalized_cost > best_beam_cost) {
                        best_beam_cost = normalized_cost;
                    }
                } else if (type == 1 /* RACK_MISSILE */ || type == 2 /* RACK_TORPEDO */) {
                    if (normalized_cost > best_missile_cost) {
                        best_missile_cost = normalized_cost;
                    }
                }
            }
        }

        // Evaluate weapon upgrades
        if (best_beam_cost != 0 && ai_tech->best_weapon1 > best_beam_cost) {
            desirability += (ai_tech->best_weapon1 - best_beam_cost) * 8;
        }

        if (best_missile_cost != 0 && ai_tech->best_weapon2 > best_missile_cost) {
            desirability += (ai_tech->best_weapon2 - best_missile_cost) * 6;
        }

        // Small hulls check against standard designs
        if (desirability != 0 && ship->d.size < 5) {
            s_ship_design* standard_design = &MOX::_player[owner].ship_designs[(int)ship->d.size];
            // Compare the design contents excluding the owner field at offset 0x63
            if (memcmp(&ship->d, standard_design, 99 /* sizeof(s_ship_design) */) == 0) {
                return 0;
            }
        }

        return desirability;
    }

    void Compute_AI_Data_() {
        RUSS::Assert_Settings_();
        allocate::Push_Block_(MOX::_screen_seg);
        _ship_strengths = (uint16_t(*)[500][8])allocate::Allocate_Next_Block_(MOX::_screen_seg, 8000);
        _ship_strengths_in_nebula = (uint16_t(*)[500][8])allocate::Allocate_Next_Block_(MOX::_screen_seg, 8000);
        _ship_bomb_strengths = (uint16_t(*)[500][4])allocate::Allocate_Next_Block_(MOX::_screen_seg, 4000);
        _planet_stage_weights = (int16_t(*)[360][8])allocate::Allocate_Next_Block_(MOX::_screen_seg, 5760);
        _base_star_stage_weights = (uint32_t(*)[72][8])allocate::Allocate_Next_Block_(MOX::_screen_seg, 2304);
        _star_stage_weights = (uint32_t(*)[72][8])allocate::Allocate_Next_Block_(MOX::_screen_seg, 2304);
        _n_turns_from_to = (uint8_t(*)[72][72][8])allocate::Allocate_Next_Block_(MOX::_screen_seg, 41472 /* 0xA200 */);
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
        RUSS::Mox_Update_();
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
                                uint16_t str = (uint16_t)AIPOWER::Ship_Strength_Vs_Ships_(i, p);
                                (*_ship_strengths)[i][p] = str;
                                (*_ai_player_info)[owner].total_ship_strength[p] += str;
                                (*_ship_strengths_in_nebula)[i][p] = (uint16_t)AIPOWER::Ship_Strength_Vs_Ships_In_Nebula_(i, p);
                                AIPOWER::Ship_Strength_Vs_Planetary_Shield_(i, (*_ship_bomb_strengths)[i]);
                                RUSS::Mox_Update_();
                                if (owner < 8 && turn_idx < 10) {
                                    uint16_t cur_str = in_nebula ? (*_ship_strengths_in_nebula)[i][p] : (*_ship_strengths)[i][p];
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

        RUSS::Mox_Update_();
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

        RUSS::Mox_Update_();
        int32_t range_sq[8], ext_range_sq[8];
        for (int p = 0; p < MOX::_NUM_PLAYERS; ++p) {
            int32_t r = MOX::_player[p].ship_range;
            range_sq[p] = (r * 30) * (r * 30);
            int32_t ext_r = r * 45;
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

        RUSS::Mox_Update_();
        memset(_n_turns_from_to, -1, sizeof(*_n_turns_from_to));
        memset(_n_turns_from_to_intermediate, -1, sizeof(*_n_turns_from_to_intermediate));
        for (int s1 = 0; s1 < MOX::_NUM_STARS; ++s1) {
            for (int p = 0; p < MOX::_NUM_PLAYERS; ++p) {
                if (((*_star_in_extended_range_of_player)[s1] >> p) & 1) {
                    for (int s2 = 0; s2 < MOX::_NUM_STARS; ++s2) {
                        if (((*_star_in_extended_range_of_player)[s2] >> p) & 1) {
                            (*_n_turns_from_to)[s1][s2][p] = (uint8_t)SHIPMOVE::Player_N_Turns_From_Star_To_Star_((int16_t)p, (int16_t)s1, (int16_t)s2);
                            RUSS::Mox_Update_();
                        }
                    }
                }
            }
        }

        RUSS::Mox_Update_();
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
                            RUSS::Mox_Update_();
                        }
                    }
                }
            }
        }

        RUSS::Mox_Update_();
        memset(_ai_colony_info, 0, sizeof(*_ai_colony_info));
        for (int c = 0; c < MOX::_NUM_COLONIES; ++c) {
            s_colony& col = MOX::_colony[c];
            if (col.owner != -1) {
                s_ai_colony_info& ai_col = (*_ai_colony_info)[c];
                RUSS::Mox_Update_();
                for (int p_idx = 0; p_idx < col.n_pops; ++p_idx) {
                    if (col.pop[p_idx] & 0x400) {
                        ai_col.n_flagged_pops++;
                    }
                }
                ai_col.race = (int8_t)Colony_Dominant_Player_(&col);
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
                RUSS::Mox_Update_();
                for (int p = MOX::_NUM_PLAYERS - 1; p >= 0; --p) {
                    if (p != owner && (*_enroute_ships)[star_idx][p][9]) {
                        (*_ai_colony_info)[c].threatened = 1;
                        break;
                    }
                }
            }
        }

        RUSS::Mox_Update_();
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

        RUSS::Mox_Update_();
        for (int p1 = 0; p1 < MOX::_NUM_PLAYERS; ++p1) {
            RUSS::Mox_Update_();
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
                    (*_ai_player_info)[p1].n_reachable_players++;
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
                        (*_ai_player_info)[p1].n_reachable_players++;
                        (*_ai_player_info)[p1].reachable_players[p2] = 1;
                    }
                }
            }
        }

        RUSS::Mox_Update_();
        for (int c = 0; c < MOX::_NUM_COLONIES; ++c) {
            s_colony& col = MOX::_colony[c];
            if (col.owner != -1) {
                (*_ai_player_info)[(int)col.owner].colony_count++;
                if ((MOX::_star[MOX::_planet[col.planet].star_index].blockaded >> col.owner) & 1) {
                    (*_ai_player_info)[(int)col.owner].colony_blockaded_count++;
                }
            }
        }

        RUSS::Mox_Update_();
        int16_t max_colonies = 0;
        for (int p = 0; p < MOX::_NUM_PLAYERS; ++p) {
            if ((*_ai_player_info)[p].colony_count > max_colonies) {
                max_colonies = (*_ai_player_info)[p].colony_count;
            }
        }
        for (int p = 0; p < MOX::_NUM_PLAYERS; ++p) {
            (*_ai_player_info)[p].colony_deficit = max_colonies - (*_ai_player_info)[p].colony_count;
        }

        RUSS::Mox_Update_();
        Compute_Base_Planet_Values_();
        RUSS::Mox_Update_();
        Compute_Contextual_Planet_Values_();
        RUSS::Mox_Update_();
        aitech::Compute_AI_Tech_Info_(_aitech);
        RUSS::Mox_Update_();

        _n_all_ships = 0;
        memset(_airefit, 0, sizeof(*_airefit));
        memset(_ship_refit_desirabilities, 0, sizeof(*_ship_refit_desirabilities));

        for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
            s_ship_data* ship = &MOX::_ship[i];
            if (ship->d.ship_type == 0 && ship->owner < MOX::_NUM_PLAYERS && ship->status < 3 && ship->mission != 4 /* SHIP_MISSION_SCRAP */) {
                (*_all_ships)[_n_all_ships++] = (int16_t)i;
                if (ship->status == 0) {
                    RUSS::Mox_Update_();
                    int32_t d = Refit_Desirability_(ship);
                    (*_ship_refit_desirabilities)[i] = d;
                    s_ai_refit_player& refit_p = (*_airefit)[ship->owner];
                    refit_p.entry[refit_p.n_entries].desirability = d;
                    if (d != 0) {
                        refit_p.entry[refit_p.n_entries].ship_idx = (int16_t)i;
                        refit_p.n_entries++;
                    }
                }
            }
        }

        for (int p = 0; p < MOX::_NUM_PLAYERS; ++p) {
            RUSS::Mox_Update_();
            qsort((*_airefit)[p].entry, (*_airefit)[p].n_entries, 6, airefit_cmp_);
        }

        RUSS::Mox_Update_();
        memset(_i_all_combat_ships, -1, sizeof(*_i_all_combat_ships));
        
        qsort(_all_ships, _n_all_ships, 2, sort_ships_);

        for (int i = 0; i < _n_all_ships; ++i) {
            int ship_idx = (*_all_ships)[i];
            int loc = MOX::_ship[ship_idx].location;
            int owner = MOX::_ship[ship_idx].owner;
            if (loc < 72) {
                if ((*_i_all_combat_ships)[owner][loc] == -1) {
                    (*_i_all_combat_ships)[owner][loc] = (int16_t)i;
                }
            } else {
                if ((*_i_all_combat_ships)[owner][MOX::_NUM_STARS] == -1) {
                    (*_i_all_combat_ships)[owner][MOX::_NUM_STARS] = (int16_t)i;
                }
            }
        }

        RUSS::Mox_Update_();

        memset(_possible_ship_strengths, 0, sizeof(*_possible_ship_strengths));

        for (int i_as = 0; i_as < _n_all_ships; ++i_as) {
            int i = (*_all_ships)[i_as];
            bool stealth = struct_::Test_Bit_Field_(MOX::_ship[i].d.special_device_flags, 11);
            int start_star = MOX::_ship[i].location;
            int owner = MOX::_ship[i].owner;
            int initial_delay = 0;
            
            RUSS::Mox_Update_();

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
                                    uint16_t str = in_nebula ? (*_ship_strengths_in_nebula)[i][p] : (*_ship_strengths)[i][p];
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

        RUSS::Mox_Update_();
        memset(_planet_stage_weights, 0, sizeof(*_planet_stage_weights));
        for (int pl = 0; pl < MOX::_planet_count; ++pl) {
            if (MOX::_planet[pl].colony_index != -1) {
                int owner = MOX::_colony[MOX::_planet[pl].colony_index].owner;
                (*_planet_stage_weights)[pl][owner] = (*_base_planet_values)[owner][pl];
            }
        }

        RUSS::Mox_Update_();
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
                RUSS::Mox_Update_();
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
                                RUSS::Mox_Update_();
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
                                RUSS::Mox_Update_();
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

        RUSS::Mox_Update_();
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

    void __cdecl Compute_Star_Player_Range_Info_() {
        if (MOX::_NUM_STARS <= 0) {
            return;
        }

        for (int star_idx = 0; star_idx < (int)MOX::_NUM_STARS; ++star_idx) {
            s_ai_star_info& ai_info = (*_ai_star_info)[star_idx];

            // Evaluate colony owner ship range threats to this star
            for (int colony_idx = 0; colony_idx < (int)MOX::_NUM_COLONIES; ++colony_idx) {
                s_colony& col = MOX::_colony[colony_idx];
                int owner = (int)col.owner;

                if (owner != -1) {
                    int16_t col_star_idx = MOX::_planet[(int16_t)col.planet].star_index;
                    int ship_range = (int)MOX::_player[owner].ship_range;

                    if (Star_In_Range_Of_Given_Star_((short)star_idx, col_star_idx, ship_range)) {
                        ai_info.per_player[owner].nearest_eta++;
                    }
                }
            }

            int8_t in_range[8];
            int8_t out_of_range[8];
            Find_Players_In_Range_((short)star_idx, in_range, out_of_range);

            for (int player_idx = 0; player_idx < (int)MOX::_NUM_PLAYERS; ++player_idx) {
                // If this player has a colony in the current star system
                if ((MOX::_star[star_idx].has_colony >> player_idx) & 1) {
                    s_player& plr = MOX::_player[player_idx];
                    s_ai_star_player_block& block = ai_info.per_player[player_idx];

                    for (int other_idx = 0; other_idx < (int)MOX::_NUM_PLAYERS; ++other_idx) {
                        if (player_idx == other_idx) {
                            continue;
                        }
                        if (MOX::_player[other_idx].eliminated) {
                            continue;
                        }

                        if (in_range[other_idx]) {
                            int treaty = (int)plr.treaty[other_idx];
                            switch (treaty) {
                                case 0:
                                    block.neutral_count++;
                                    break;
                                case 1:
                                case 2:
                                case 3:
                                    block.enemy_count++;
                                    break;
                                case 4:
                                case 5:
                                case 6:
                                    block.alliance_count++;
                                    break;
                                default:
                                    break;
                            }
                        } else if (out_of_range[other_idx]) {
                            block.out_of_range_count++;
                        }
                    }
                }
            }
        }
    }

    void __cdecl Find_Players_In_Range_(int16_t star_idx, int8_t* in_range_900, int8_t* in_range_2025) {
        auto& target_star = MOX::_star[star_idx];
        int32_t target_x = target_star.x;
        int32_t target_y = target_star.y;

        // Initialize flag arrays (8 players)
        memset(in_range_900, 0, 8);
        memset(in_range_2025, 0, 8);

        int32_t max_range_sq_900[8];
        int32_t max_range_sq_2025[8];

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            int32_t range = MOX::_player[i].ship_range;
            int32_t range_sq = range * range;

            // Primary range: 30 cells (30^2 = 900)
            max_range_sq_900[i] = range_sq * 900;
            // Extended range: 45 cells (45^2 = 2025)
            max_range_sq_2025[i] = range_sq * 2025;
        }

        for (int i = 0; i < MOX::_NUM_COLONIES; ++i) {
            auto& colony = MOX::_colony[i];
            int8_t owner = colony.owner;

            // If player has no colony here or is already marked as 'in range', skip
            if (owner != -1 && in_range_900[owner] == 0) {
                auto& planet = MOX::_planet[colony.planet];
                auto& colony_star = MOX::_star[planet.star_index];

                int32_t dx = (int32_t)colony_star.x - target_x;
                int32_t dy = (int32_t)colony_star.y - target_y;
                int32_t dist_sq = dx * dx + dy * dy;

                if (dist_sq <= max_range_sq_2025[owner]) {
                    in_range_2025[owner] = 1;
                    if (dist_sq <= max_range_sq_900[owner]) {
                        in_range_900[owner] = 1;
                    }
                }
            }
        }
    }

    int __cdecl airefit_cmp_(const void* a, const void* b) {
        int32_t val_1 = *reinterpret_cast<const int32_t*>(reinterpret_cast<const uint8_t*>(a) + 2);
        int32_t val_2 = *reinterpret_cast<const int32_t*>(reinterpret_cast<const uint8_t*>(b) + 2);

        return val_1 - val_2;
    }

    int __cdecl sort_ships_(const void* a, const void* b) {
        int16_t idx1 = *static_cast<const int16_t*>(a);
        int16_t idx2 = *static_cast<const int16_t*>(b);

        s_ship_data& ship1 = MOX::_ship[idx1];
        s_ship_data& ship2 = MOX::_ship[idx2];

        // Sort by owner
        if (ship1.owner < ship2.owner) {
            return -1;
        }
        if (ship1.owner > ship2.owner) {
            return 1;
        }

        // Sort by location
        int16_t loc1 = ship1.location;
        int16_t loc2 = ship2.location;

        if (loc2 > loc1) {
            if (loc1 < 0x48 /* 72 - Threshold for star systems vs deep space */) {
                return -1;
            }
        } else if (loc2 < loc1) {
            if (loc2 < 0x48) {
                return 1;
            }
        }

        // If both ships are in the same system index (below 72), they are considered equal in location
        if (loc1 < 0x48 && loc2 < 0x48) {
            return 0;
        }

        // Sort by coordinates (ascending)
        if (ship1.x < ship2.x) {
            return -1;
        }
        if (ship1.x > ship2.x) {
            return 1;
        }

        if (ship1.y < ship2.y) {
            return -1;
        }
        if (ship1.y > ship2.y) {
            return 1;
        }

        return 0;
    }

    void __cdecl Compute_Base_Planet_Values_() {
        memset(*_base_planet_values, 0, sizeof(int16_t) * 8 * 360);

        if (MOX::_NUM_PLAYERS <= 0) {
            return;
        }

        for (int16_t player_idx = 0; player_idx < (int16_t)MOX::_NUM_PLAYERS; ++player_idx) {
            for (int16_t planet_idx = 0; planet_idx < (int16_t)MOX::_planet_count; ++planet_idx) {
                s_planet_data* planet = &MOX::_planet[planet_idx];

                uint8_t system_visibility_bits = (*_star_in_extended_range_of_player)[planet->star_index];

                if ((system_visibility_bits >> (player_idx & 0x1F)) & 1) {
                    int16_t col_idx = planet->colony_index;

                    RUSS::Mox_Update_();

                    uint16_t value;
                    if (col_idx == -1) {
                        value = AIDATA::Uncolonized_Planet_Worth_To_Player_(planet, &MOX::_player[player_idx]);
                    } else {
                        value = Colony_Worth_To_Player_(&MOX::_colony[col_idx], &MOX::_player[player_idx]);
                    }

                    (*_base_planet_values)[player_idx][planet_idx] = value;
                }
            }
        }
    }

    void __cdecl Deallocate_AI_Data_() {
        RUSS::Assert_Settings_();
        allocate::Pop_Block_(MOX::_screen_seg);
    }

    /**
     * Computes the "contextual" value of planets for each player, adjusting the base
     * values based on ownership of other planets in the same star system.
     */
    void __cdecl Compute_Contextual_Planet_Values_() {
        // Clear all contextual values for all players (8 players * 360 planets)
        memset(*AIDATA::_contextual_planet_values, 0, 8 * 360 * sizeof(uint16_t));

        if (MOX::_NUM_PLAYERS <= 0) {
            return;
        }

        for (int p = 0; p < (int)MOX::_NUM_PLAYERS; ++p) {
            for (int i = 0; i < (int)MOX::_planet_count; ++i) {
                uint16_t base_val = (*AIDATA::_base_planet_values)[p][i];
                if (base_val <= 0) {
                    continue;
                }

                s_planet_data* planet = &MOX::_planet[i];
                s_star_data* star = &MOX::_star[planet->star_index];

                // Refresh UI/input during processing
                RUSS::Mox_Update_();

                uint16_t& contextual_val = (*AIDATA::_contextual_planet_values)[p * 360 + i];
                contextual_val = base_val;

                int uncolonized_system_base_sum = 0;
                int owned_system_base_sum = 0;
                int owned_colonies_in_system = 0;
                int enemy_colonies_in_system = 0;

                // Examine all potential planets in the system (up to 5)
                for (int k = 0; k < 5; ++k) {
                    int16_t sys_planet_idx = star->planet_index[k];

                    if (sys_planet_idx == -1 || sys_planet_idx == i) {
                        continue;
                    }

                    int16_t colony_idx = MOX::_planet[sys_planet_idx].colony_index;

                    if (colony_idx == -1) {
                        // Uncolonized planet in system
                        uncolonized_system_base_sum += (*AIDATA::_base_planet_values)[p][sys_planet_idx];
                    } else {
                        if (MOX::_colony[colony_idx].owner == p) {
                            // Colony owned by the current player
                            owned_colonies_in_system++;
                            owned_system_base_sum += (*AIDATA::_base_planet_values)[p][sys_planet_idx];
                        } else {
                            // Colony owned by an enemy
                            enemy_colonies_in_system++;
                        }
                    }
                }

                if (planet->colony_index == -1) {
                    // Current planet is not yet colonized
                    if (owned_colonies_in_system > 0) {
                        // Bonus if we already have a presence in the system
                        contextual_val = (int16_t)((contextual_val * (planet->size + 1)) / 10);
                    } else {
                        // Small bonus for general system potential
                        contextual_val += (int16_t)(uncolonized_system_base_sum / 8);
                    }

                    // Penalize value if enemies are present
                    if (enemy_colonies_in_system > 0) {
                        contextual_val = (int16_t)(contextual_val / (enemy_colonies_in_system + 2));
                    }
                } else {
                    // Current planet is already colonized; add value of neighboring potential
                    contextual_val += (int16_t)(uncolonized_system_base_sum / 8);
                    contextual_val += (int16_t)(owned_system_base_sum / 4);
                }
            }
        }
    }

    /**
     * Calculates the "proximity worth" value of a colony for a specific player.
     * This logic accounts for difficulty settings, alien management centers,
     * and strategic range to existing colonies.
     */
    int16_t __cdecl Proximity_Worth_To_Player_(s_colony* colony, s_player* target_player) {
        if (MOX::_settings.game_difficulty == 0) {
            return 0;
        }

        int8_t owner_idx = colony->owner;
        s_player* owner_ptr = &MOX::_player[owner_idx];

        // Check for Alien Management Center tech
        if (owner_ptr->tech_applications[TECH_APP_ALIEN_MANAGEMENT_CENTER] == 3) {
            return 0;
        }

        int16_t ship_range = owner_ptr->ship_range;
        int16_t colony_star_idx = MOX::_planet[colony->planet].star_index;
        int target_player_idx = (target_player - MOX::_player);

        int16_t total_worth = 0;

        if (target_player_idx == owner_idx) {
            for (int i = 0; i < MOX::_NUM_STARS; ++i) {
                if (MOX::_star[i].spectral_class != STAR_CLASS_BLACK_HOLE) {
                    if (Star_In_Range_Of_Given_Star_(i, colony_star_idx, ship_range)) {
                        uint8_t has_colony_mask = MOX::_star[i].has_colony;
                        bool other_players_present = (has_colony_mask & ~(1 << owner_idx)) != 0;

                        // Score is 120 if others are present, 80 otherwise
                        int score = other_players_present ? 120 : 80;
                        uint8_t eta = (*_ai_star_info)[i].per_player[owner_idx].nearest_eta;
                        total_worth += (short)(score / eta);
                    }
                }
            }
        } else {
            uint32_t min_eta = 10000;
            for (int i = 0; i < MOX::_NUM_STARS; ++i) {
                if (Star_In_Range_Of_Given_Star_(i, colony_star_idx, ship_range)) {
                    // Check if target player has a colony at star i
                    if (MOX::_star[i].has_colony & (1 << target_player_idx)) {
                        uint8_t eta = (*_ai_star_info)[i].per_player[owner_idx].nearest_eta;
                        total_worth += (short)(100 / eta);
                        if ((uint32_t)eta < min_eta) {
                            min_eta = eta;
                        }
                    }
                }
            }

            if (total_worth != 0) {
                total_worth += (short)(400 / min_eta);
            }
        }

        return total_worth;
    }

    short __cdecl Player_N_Turns_From_Star_To_Star_With_Intermediate_(short player, short star_a, short star_b, int16_t* inter_star_out, bool use_extended) {
        uint8_t* info = (uint8_t*)&(*_n_turns_from_to_intermediate)[star_a][star_b][player];

        if (info[1] == 0xFF) {
            s_player* plr = &MOX::_player[player];

            // Check if player can reach the star directly
            if (plr->ship_speed != 0 &&
                plr->tech_applications[TECH_APP_JUMP_GATE] != 3 &&
                plr->tech_applications[TECH_APP_STAR_GATE] != 3 &&
                MOX::_star[star_a].wormhole_star_id == -1 &&
                MOX::_star[star_b].wormhole_star_id == -1 &&
                (*_n_turns_from_to)[star_a][star_b][player] != 0xFF) {

                int dist = GEO::Parsecs_Between_Stars_(star_a, star_b);
                int turns = (dist + (uint8_t)plr->ship_speed - 1) / (uint8_t)plr->ship_speed;

                if ((uint8_t)(*_n_turns_from_to)[star_a][star_b][player] <= turns) {
                    if (((*_star_in_range_of_player)[star_b] >> player) & 1) {
                        info[0] = (uint8_t)turns;
                        info[1] = (uint8_t)star_b;
                    } else {
                        info[1] = MAX_STARS;
                    }
                    info[2] = (uint8_t)turns;
                    info[3] = (uint8_t)star_b;
                }
            }

            if (info[1] == 0xFF) {
                for (int i = MOX::_NUM_STARS - 1; i >= 0; --i) {
                    // Check if Star i is in range and not blocked
                    if (((*_star_in_extended_range_of_player)[i] >> player) & 1) {
                        s_star_data* s = &MOX::_star[i];

                        if (i == star_b || (s->spectral_class != STAR_CLASS_BLACK_HOLE &&
                            s->system_special != 9 /* Orion */ &&
                            (((s->has_colony >> player) & 1) || s->has_colony == 0))) {

                            uint8_t dist_a_i = (uint8_t)(*_n_turns_from_to)[star_a][i][player];
                            uint8_t dist_i_b = (uint8_t)(*_n_turns_from_to)[i][star_b][player];
                            int total_turns = dist_a_i + dist_i_b;

                            if ((((*_star_in_range_of_player)[i] >> player) & 1) && total_turns < info[0]) {
                                info[0] = total_turns;
                                info[1] = (i == star_a) ? (uint8_t)star_b : (uint8_t)i;
                            }

                            if (total_turns < info[2]) {
                                info[2] = total_turns;
                                info[3] = (i == star_a) ? (uint8_t)star_b : (uint8_t)i;
                            }
                        }
                    }
                    if (info[0] <= 1) break;
                }

                if (info[1] == 0xFF) {
                    info[1] = 0x48 /* NUM_STARS (72) */;
                }
                if (info[3] == 0xFF) {
                    info[3] = 0x48 /* NUM_STARS (72) */;
                }
            }
        }

        if (inter_star_out) {
            uint8_t inter_star = use_extended ? info[3] : info[1];
            if (inter_star < 0x48 /* NUM_STARS (72) */) {
                *inter_star_out = (uint16_t)inter_star;
            } else {
                *inter_star_out = 0xffff;
            }
        }

        if (use_extended) {
            return (short)info[2];
        }
        return (short)info[0];
    }

    short __cdecl Ship_N_Turns_To_Star_With_Intermediate_(short ship_idx, short dest_star_idx, int16_t* intermediate_star_idx) {
        uint8_t* cached_info = (uint8_t*)&(*_ship_n_turns_to_intermediate)[ship_idx][dest_star_idx];

        bool has_stargate = struct_::Test_Bit_Field_(MOX::_ship[ship_idx].d.special_device_flags, 11);

        if (cached_info[1] == 0xff) {
            int16_t loc = MOX::_ship[ship_idx].location;
            if (loc >= 72 && loc < 1000) {
                int owner = MOX::_ship[ship_idx].owner;
                int ship_dest_star = loc - 500;

                if (cached_info[0] > 1) {
                    for (int i = MOX::_NUM_STARS - 1; i >= 0; i--) {
                        bool in_range = ((*_star_in_range_of_player)[i] >> owner) & 1;
                        bool in_ext_range = ((*_star_in_extended_range_of_player)[i] >> owner) & 1;

                        if (in_range || (in_ext_range && has_stargate)) {
                            if (i == dest_star_idx || i == ship_dest_star ||
                                (MOX::_star[i].spectral_class != 6 &&
                                    MOX::_star[i].system_special != 9 &&
                                    (((MOX::_star[i].has_colony >> owner) & 1) != 0 || MOX::_star[i].has_colony == 0))) {

                                bool blocked = GEO::Black_Hole_Blocks_Points_(
                                    MOX::_ship[ship_idx].x, MOX::_ship[ship_idx].y,
                                    MOX::_star[i].x, MOX::_star[i].y
                                );

                                if (!blocked) {
                                    uint32_t total_turns = (uint8_t)(*_n_turns_from_to)[i][dest_star_idx][owner] + (uint8_t)(*_ship_n_turns_to)[ship_idx][i];
                                    if (total_turns < cached_info[0]) {
                                        cached_info[0] = static_cast<uint8_t>(total_turns);
                                        cached_info[1] = static_cast<uint8_t>(i);
                                    }
                                }
                            }
                        }
                        if (cached_info[0] <= 1) {
                            break;
                        }
                    }
                }

                if (cached_info[1] == 0xff) {
                    cached_info[1] = 72;
                }
            } else {
                if (loc >= 1000) {
                    loc = HAROLD::XY_To_Star_Id_(MOX::_ship[ship_idx].x, MOX::_ship[ship_idx].y);
                }

                cached_info[0] = static_cast<uint8_t>(AIDATA::Player_N_Turns_From_Star_To_Star_With_Intermediate_(
                    MOX::_ship[ship_idx].owner,
                    loc,
                    dest_star_idx,
                    intermediate_star_idx,
                    has_stargate
                ));

                // BUG: intermediate_star_idx is dereferenced here without null check
                cached_info[1] = static_cast<uint8_t>(*intermediate_star_idx);
            }
        }

        if (intermediate_star_idx != nullptr) {
            if (cached_info[1] < 72) {
                *intermediate_star_idx = cached_info[1];
            } else {
                *intermediate_star_idx = 0xffff;
            }
        }

        return cached_info[0];
    }

    int16_t __cdecl Colony_Worth_To_Player_(s_colony* colony, s_player* target_player) {
        int target_player_idx = (target_player - MOX::_player);
        s_planet_data* planet = &MOX::_planet[colony->planet];
        int colony_idx = (colony - MOX::_colony);
        int colony_race = (*AIDATA::_ai_colony_info)[colony_idx].race;
        s_player* race_player = &MOX::_player[colony_race];

        int max_pop_race = COLCALC::Planet_Max_Population_For_Player_(planet, (int16_t)colony_race);
        int max_pop_target = COLCALC::Planet_Max_Population_For_Player_(planet, (int16_t)target_player_idx);
        int avg_max_pop = (max_pop_target + max_pop_race + 1) / 2;

        bool is_hostile = (planet->climate == PLANET_CLIMATE_TOXIC || planet->climate == PLANET_CLIMATE_RADIATED);

        if (colony->outpost_flag != 0) {
            return AIDATA::Proximity_Worth_To_Player_(colony, target_player);
        }

        int food_base = COLCALC::Colony_Empire_Base_Food2_Produced_(colony, (int16_t)target_player_idx, (uint32_t)colony_race, nullptr);
        int food = food_base / 2;
        int industry = COLCALC::Colony_Empire_Base_Industry_Produced_(colony, (uint16_t)target_player_idx, (uint32_t)colony_race, nullptr);
        int research = COLCALC::Colony_Empire_Base_Research_Produced_(colony, (int16_t)target_player_idx, (uint32_t)colony_race, nullptr);

        int prod_mult;
        if (race_player->traits[TRAIT_EATS_MINERALS] != 0) {
            prod_mult = (industry + research) * 6;
        } else if (race_player->traits[TRAIT_CYBERNETIC] != 0) {
            prod_mult = (industry + research + food_base / 2) * 4;
        } else {
            prod_mult = (research + food * 2 + industry) * 3;
        }

        int growth_trait = target_player->traits[TRAIT_POP_GROWTH_BONUS];
        int growth_mult;
        switch (growth_trait) {
            case 50:
                growth_mult = (colony->n_pops * 19 + avg_max_pop) * 5;
                break;
            case 100:
                growth_mult = (colony->n_pops * 9 + avg_max_pop) * 10;
                break;
            case 0:
                growth_mult = colony->n_pops * 85 + avg_max_pop * 15;
                break;
            case -50:
                growth_mult = (avg_max_pop + colony->n_pops * 4) * 20;
                break;
            default:
                growth_mult = food;
                break;
        }

        int worth = (avg_max_pop - colony->n_pops + 100) * growth_mult * prod_mult / 100;

        if (!is_hostile || race_player->traits[12] == 0) {
            int climate_modifier = COLCALC::_climate_maintenance_modifiers[(int)planet->climate] >> 2;
            worth = worth * (100 - climate_modifier) / 100;

            if (is_hostile) {
                worth = (worth * 3) / 4;
            }
        }

        if (target_player_idx != colony->owner || colony->buildings[BUILDING_GRAVITY_GENERATOR] == 0) {
            int grav_gen_bonus = (target_player->tech_applications[TECH_APP_PLANETARY_GRAVITY_GENERATOR] == 3) ? 1 : 0;
            int gravity = planet->gravity_class;

            if (gravity == PLANET_GRAVITY_LOW_G) {
                if (race_player->traits[TRAIT_LOW_G_WORLD] == 0) {
                    worth = (grav_gen_bonus + 12) * worth / 16;
                }
            } else if (gravity == PLANET_GRAVITY_NORMAL_G) {
                if (race_player->traits[TRAIT_LOW_G_WORLD] != 0) {
                    worth = (grav_gen_bonus + 12) * worth / 16;
                }
            } else if (gravity == PLANET_GRAVITY_HEAVY_G) {
                if (race_player->traits[TRAIT_HEAVY_G_WORLD] == 0) {
                    worth = (grav_gen_bonus + 6) * worth / 12;
                }
            }
        }

        if (colony->buildings[BUILDING_CAPITOL] != 0) {
            int government_tier = MOX::_player[(int)colony->owner].traits[TRAIT_CURRENT_GOVERNMENT] / 2;
            if (government_tier == 0) {
                worth = worth * 3 / 2;
            } else if (government_tier == 1) {
                worth = worth * 4 / 3;
            } else if (government_tier == 2) {
                worth = worth * 6 / 5;
            }
        }

        if (planet->planet_special == SYSTEM_SPECIAL_GOLD_DEPOSITS) {
            worth += 1000;
        } else if (planet->planet_special == SYSTEM_SPECIAL_GEM_DEPOSITS) {
            worth += 2000;
        }

        int final_worth = worth / 64;

        if (MOX::_settings.game_difficulty >= 2 /* DIFFICULTY_NORMAL */) {
            short proximity_worth = AIDATA::Proximity_Worth_To_Player_(colony, target_player);
            final_worth += (proximity_worth / 8);
            if (final_worth < proximity_worth) {
                final_worth = proximity_worth;
            }
        }

        if (final_worth > 65535) {
            final_worth = 65535;
        }

        return (int16_t)final_worth;
    }

    int32_t __cdecl Uncolonized_Planet_Worth_To_Player_(s_planet_data* planet, s_player* plr) {
        int32_t worth = 0;

        if (planet->planet_type == PLANET_TYPE_PLANET) {
            int16_t subterranean_bonus = 0;
            if (plr->traits[TRAIT_SUBTERRANEAN] != 0) {
                subterranean_bonus = COLCALC::Double_Add_Two_(planet->size);
            }

            int16_t player_idx = static_cast<int16_t>(plr - MOX::_player);
            int16_t max_pop = COLCALC::Planet_Max_Population_For_Player_(planet, player_idx);

            int32_t total_pop_sum = max_pop + subterranean_bonus;
            if (plr->tech_applications[TECH_APP_ADVANCED_CITY_PLANNING] == 3) {
                total_pop_sum += 5;
            }

            int8_t mineral_bonus = COLCALC::_minerals_per_mine[planet->mineral_class];
            int32_t scaled_pop = (mineral_bonus + 1) * total_pop_sum / 3;

            int8_t pop_growth_bonus = plr->traits[TRAIT_POP_GROWTH_BONUS];
            if (pop_growth_bonus == -50) {
                worth = 80 * mineral_bonus + scaled_pop;
            }
            else if (pop_growth_bonus == 0) {
                worth = 75 * mineral_bonus + 2 * scaled_pop;
            }
            else if (pop_growth_bonus == 50) {
                worth = 70 * mineral_bonus + 3 * scaled_pop;
            }
            else if (pop_growth_bonus == 100) {
                worth = 65 * mineral_bonus + 4 * scaled_pop;
            }

            int32_t food_per_farmer = planet->food_per_farmer;
            bool has_grav_gen_tech = (plr->tech_applications[TECH_APP_PLANETARY_GRAVITY_GENERATOR] == 3);
            if (food_per_farmer == 0 && plr->tech_applications[TECH_APP_BIOMORPHIC_FUNGI] == 3) {
                food_per_farmer = 1;
            }

            if (plr->traits[TRAIT_EATS_MINERALS] == 0) {
                if (plr->traits[TRAIT_CYBERNETIC] == 0) {
                    worth += food_per_farmer * 150;
                }
                else {
                    worth += food_per_farmer * 75;
                }
            }

            switch (planet->planet_special) {
            case SYSTEM_SPECIAL_GOLD_DEPOSITS: worth += 1280; break;
            case SYSTEM_SPECIAL_GEM_DEPOSITS: worth += 2560; break;
            case SYSTEM_SPECIAL_ANCIENT_ARTIFACTS: worth += 640; break;
            case SYSTEM_SPECIAL_ORION_STAR: worth += 1600; break;
            default: break;
            }

            worth = (mineral_bonus + 7) * total_pop_sum * worth / 10;

            bool is_swamp_ocean = (planet->climate == PLANET_CLIMATE_OCEAN || planet->climate == PLANET_CLIMATE_SWAMP);
            bool is_aquatic = (plr->traits[TRAIT_AQUATIC] != 0);

            if (!is_swamp_ocean || !is_aquatic) {
                int32_t maintenance_reduction = COLCALC::_climate_maintenance_modifiers[planet->climate] >> 2;
                worth = worth * (100 - maintenance_reduction) / 100;
                if (is_swamp_ocean) {
                    worth = (worth * 3) / 4;
                }
            }

            int32_t divisor = 1;
            int32_t penalty_factor = has_grav_gen_tech ? 1 : 0;
            bool apply_penalty = false;

            if (planet->gravity_class == PLANET_GRAVITY_LOW_G) {
                if (plr->traits[TRAIT_LOW_G_WORLD] == 0) {
                    divisor = 20;
                    penalty_factor += 18;
                    apply_penalty = true;
                }
            }
            else if (planet->gravity_class == PLANET_GRAVITY_NORMAL_G) {
                if (plr->traits[TRAIT_LOW_G_WORLD] != 0) {
                    divisor = 20;
                    penalty_factor += 18;
                    apply_penalty = true;
                }
            }
            else if (planet->gravity_class == PLANET_GRAVITY_HEAVY_G) {
                if (plr->traits[TRAIT_HEAVY_G_WORLD] == 0) {
                    divisor = 6;
                    penalty_factor += 4;
                    apply_penalty = true;
                }
            }

            if (apply_penalty) {
                worth = (worth * penalty_factor) / divisor;
            }
        }

        int32_t final_worth = (worth + ((worth >> 31) & 0x3F)) >> 6;
        if (final_worth > 0xFFFE) {
            final_worth = 0xFFFF;
        }
        return final_worth;
    }

}