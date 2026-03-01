#include "pch.h"

namespace MSG_ {
    int16_t __cdecl N_Msgs_For_Player_(int16_t player_idx) {
        return (int16_t)_n_msgs[player_idx];
    }

    void __cdecl Add_Msg_(s_MSG msg, int16_t plr_idx) {
        if (_max_msgs[plr_idx] == 0) {
            return;
        }

        uint8_t count = _n_msgs[plr_idx];

        if (count < _max_msgs[plr_idx]) {
            int index = _first_msg[plr_idx] + count;
            _msgs[index] = msg;
            _n_msgs[plr_idx] = count + 1;
        } else {
            s_MSG* lowest_priority_msg = &_msgs[_first_msg[plr_idx]];
            s_MSG* current = lowest_priority_msg + 1;

            // Note: The original assembly uses a shifting window comparison loop 'current < lowest_priority_msg + count + 1'
            // resulting in potential buffer overrun into adjacent player message slots or uninitialized data.
            while (current < lowest_priority_msg + count + 1) {
                if (current->priority < lowest_priority_msg->priority) {
                    lowest_priority_msg = current;
                }
                current++;
            }

            if (msg.priority > lowest_priority_msg->priority) {
                *lowest_priority_msg = msg;
            }
        }
    }

    int16_t __cdecl First_Msg_For_Player_(int16_t player_idx) {
        return (int16_t)_first_msg[player_idx];
    }

    /**
     * Calculates and allocates message pool slices (out of 100 slots) 
     * for all active (non-eliminated) players.
     */
    void __cdecl Allocate_Msg_Slots_() {
        int active_players = 0;

        // First pass: Count active players
        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            // s_player structure size is 0xea9 (3753 bytes)
            if (MOX::_player[i].eliminated == 0) {
                active_players++;
            }
        }

        // Reset message tracking arrays for all potential 8 players
        for (int i = 0; i < 8; ++i) {
            _first_msg[i] = 0;
            _n_msgs[i] = 0;
            _max_msgs[i] = 0;
        }

        // Second pass: Distribute 100 message slots among active players
        if (MOX::_NUM_PLAYERS > 0 && active_players > 0) {
            int cumulative_pool = 0;
            for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
                if (MOX::_player[i].eliminated == 0) {
                    uint8_t start_slot = (uint8_t)(cumulative_pool / active_players);
                    cumulative_pool += 100;
                    uint8_t end_slot = (uint8_t)(cumulative_pool / active_players);

                    _first_msg[i] = start_slot;
                    _max_msgs[i] = end_slot - start_slot;
                }
            }
        }
    }

    void __cdecl sprintf_msg_(char* param_1, s_MSG* param_2) {
        char local_dc[20];
        char local_c8[200];

        switch (param_2->msg_type) {
            case 0 /* MSG_TYPE_COLONY */: {
                char* pcVar6 = HACCESS::Get_Planet_Name_((int16_t)(param_2->u).colony.planet_idx);
                switch ((param_2->u).colony.subtype) {
                    case 0:
                        _sprintf(param_1, _msg_colony_scrap_building_fmt,
                                     TECHDATA::_buildings[(param_2->u).colony.c.scrap.building_id].name, pcVar6,
                                     (int)(param_2->u).colony.c.scrap.bc);
                        return;
                    case 1: {
                        int16_t sVar4 = (param_2->u).colony.c.build.build_id;
                        if (sVar4 < 1 || sVar4 > 0x30) {
                            if (sVar4 < -99) {
                                char* pcVar8 = _msg_colony_built_ship_fmt;
                                if ((param_2->u).colony.c.build.is_refit != 0) {
                                    pcVar8 = _msg_colony_built_refit_fmt;
                                }
                                _sprintf(param_1, pcVar8, pcVar6, &MOX::_ship[-100 - sVar4]);
                            } else {
                                switch (sVar4) {
                                    case -15:
                                        _sprintf(param_1, _msg_colony_built_freighter_fmt, pcVar6);
                                        break;
                                    case -7:
                                        _sprintf(param_1, _msg_colony_built_spy_fmt, pcVar6);
                                        break;
                                    case -6:
                                        _sprintf(param_1, _msg_colony_built_android_scientist_fmt, pcVar6);
                                        break;
                                    case -5:
                                        _sprintf(param_1, _msg_colony_built_android_worker_fmt, pcVar6);
                                        break;
                                    case -4:
                                        _sprintf(param_1, _msg_colony_built_android_farmer_fmt, pcVar6);
                                        break;
                                    default:
                                        break;
                                }
                            }
                        } else {
                            _sprintf(param_1, _msg_colony_built_building_fmt, pcVar6, TECHDATA::_buildings[sVar4].name);
                        }

                        int16_t ship_idx = (param_2->u).colony.c.build.build_id;
                        if (ship_idx != -1) {
                            char* pcVar6_fmt = _msg_colony_built_next_queue_fmt;
                            if ((param_2->u).colony.c.build.is_refit != 0) {
                                pcVar6_fmt = _msg_colony_built_auto_build_fmt;
                            }
                            char* pcVar7 = AIBUILD::Product_Name_(ship_idx);
                            _sprintf(param_1 + strlen(param_1), pcVar6_fmt, pcVar7);
                            return;
                        }
                        break;
                    }
                    case 2: {
                        uint32_t uVar11 = (uint32_t)(param_2->u).colony.c.assimilation.val;
                        char* pcVar8 = _native_msg;
                        if ((int)uVar11 < (int)MOX::_NUM_PLAYERS) {
                            pcVar8 = MOX::_player[uVar11].race_name;
                        }
                        _sprintf(param_1, _msg_colony_genocide_fmt, pcVar8, pcVar6);
                        return;
                    }
                    case 3:
                        _sprintf(param_1, _msg_colony_assimilation_fmt,
                                     MOX::_player[(param_2->u).colony.c.assimilation.val].race_name, pcVar6);
                        return;
                    case 4: {
                        uint8_t bVar3 = (param_2->u).colony.c.starvation.a;
                        if (bVar3 == 0) {
                            _sprintf(param_1, _msg_colony_starvation_ind_fmt, pcVar6,
                                         (uint32_t)(uint8_t)(param_2->u).colony.c.starvation.b);
                            return;
                        }
                        uint8_t bVar2 = (param_2->u).colony.c.starvation.b;
                        if (bVar2 != 0) {
                            _sprintf(param_1, _msg_colony_starvation_both_fmt, pcVar6, (uint32_t)bVar3, (uint32_t)bVar2);
                            return;
                        }
                        _sprintf(param_1, _msg_colony_starvation_food_fmt, pcVar6, (uint32_t)bVar3);
                        return;
                    }
                    case 5: {
                        uint8_t bVar3 = (param_2->u).colony.c.settlers.settlers;
                        char* pcVar8 = _msg_colony_d_settlers_arrived_fmt;
                        if (bVar3 == 1) {
                            pcVar8 = _msg_colony_1_settler_arrived_fmt;
                        }
                        int iVar9 = _sprintf(param_1, pcVar8, (uint32_t)bVar3, pcVar6);
                        if ((param_2->u).colony.c.settlers.blockaded != 0) {
                            strcpy(param_1 + iVar9, _msg_colony_settlers_arrived_blockaded_fmt);
                            return;
                        }
                        uint8_t over_limit = (param_2->u).colony.c.settlers.over_limit;
                        if (over_limit != 0) {
                            char* pcVar6_fmt = _msg_colony_d_settlers_arrived_over_limit_fmt;
                            if (over_limit == 1) {
                                pcVar6_fmt = _msg_colony_1_settler_arrived_over_limit_fmt;
                            }
                            _sprintf(param_1 + iVar9, pcVar6_fmt, (uint32_t)over_limit);
                        }
                        return;
                    }
                    case 6:
                        _sprintf(param_1, _msg_colony_plague_fmt, pcVar6);
                        return;
                    case 7:
                        _sprintf(param_1, _msg_colony_boom_fmt, pcVar6);
                        return;
                    case 8:
                        _sprintf(param_1, _msg_colony_comet_fmt, pcVar6);
                        return;
                    case 9: {
                        uint32_t uVar11 = (uint32_t)(uint8_t)(param_2->u).colony.c.rebels.owner_player;
                        uint8_t bVar3 = (param_2->u).colony.c.rebels.rebels;
                        if ((int)MOX::_PLAYER_NUM != (int)uVar11) {
                            if ((uint8_t)(param_2->u).colony.c.rebels.lost < bVar3) {
                                char* pcVar8 = _msg_colony_d_rebels_won_fmt;
                                if (bVar3 == 1) {
                                    pcVar8 = _msg_colony_1_rebel_won_fmt;
                                }
                                _sprintf(param_1, pcVar8, (uint32_t)bVar3, MOX::_player[uVar11].race_name, pcVar6,
                                             (uint32_t)(uint8_t)(param_2->u).colony.c.rebels.lost);
                                return;
                            }
                            char* pcVar8 = _msg_colony_d_rebels_lost_fmt;
                            if (bVar3 == 1) {
                                pcVar8 = _msg_colony_1_rebel_lost_fmt;
                            }
                            _sprintf(param_1, pcVar8, (uint32_t)bVar3, MOX::_player[uVar11].race_name, pcVar6);
                            return;
                        }
                        uint8_t bVar2 = (param_2->u).colony.c.rebels.lost;
                        if (bVar2 < bVar3) {
                            _sprintf(param_1, _msg_colony_your_rebellion_won_fmt, (uint32_t)bVar3, pcVar6, (uint32_t)bVar2);
                            return;
                        }
                        _sprintf(param_1, _msg_colony_your_rebellion_lost_fmt, (uint32_t)bVar3, pcVar6);
                        return;
                    }
                    case 10:
                        _sprintf(param_1, _msg_colony_no_room_for_android_fmt, pcVar6);
                        return;
                    case 11: {
                        uint8_t bVar3 = (param_2->u).colony.c.android_killed.race_idx;
                        char* pcVar8 = _native_msg;
                        if (bVar3 < 8) {
                            pcVar8 = MOX::_player[bVar3].race_name;
                        }
                        _sprintf(param_1, _msg_colony_android_killed_organic_fmt, pcVar6, pcVar8);
                        return;
                    }
                }
                break;
            }
            case 1 /* MSG_TYPE_DECLARE_ALLIANCE */:
                _sprintf(param_1, _s_and_s_declare_alliance_fmt,
                             MOX::_player[(param_2->u).players.player_a_idx].race_name,
                             MOX::_player[(param_2->u).players.player_b_idx].race_name);
                return;
            case 2 /* MSG_TYPE_DECLARE_WAR */:
                _sprintf(param_1, _s_and_s_declare_war_fmt,
                             MOX::_player[(param_2->u).players.player_a_idx].race_name,
                             MOX::_player[(param_2->u).players.player_b_idx].race_name);
                return;
            case 4 /* MSG_TYPE_FINANCIAL */:
                _sprintf(param_1, _msg_financial_fmt, (int)(param_2->u).financial.a,
                             (int)(param_2->u).financial.b);
                return;
            case 5 /* MSG_TYPE_LOST_SHIP */: {
                s_ship_data* psVar13 = &MOX::_ship[(param_2->u).lost_ship.ship_idx];
                int iVar9 = _sprintf(param_1, _msg_lost_ship_fmt, TECHDATA::_hull_data[psVar13->d.size].name, psVar13,
                                         (int)psVar13->d.cost, &MOX::_star[(param_2->u).lost_ship.star_idx]);
                uint8_t cVar1 = (param_2->u).lost_ship.leader_idx;
                if (cVar1 != 0xFF) {
                    _sprintf(param_1 + iVar9, _leader_fate_strings[(param_2->u).lost_ship.fate],
                                 &MOX::_leaders[cVar1]);
                    return;
                }
                break;
            }
            case 6 /* MSG_TYPE_SCRAP_SHIP */: {
                int16_t sVar4 = (param_2->u).scrap_ship.where;
                if (sVar4 < MOX::_NUM_STARS) {
                    s_ship_data* psVar13 = &MOX::_ship[(param_2->u).scrap_ship.ship_idx];
                    _sprintf(param_1, _msg_scrap_ship_bc_fmt, TECHDATA::_hull_data[psVar13->d.size].name, psVar13,
                                 (int)psVar13->d.cost, &MOX::_star[sVar4], (int)(param_2->u).scrap_ship.bc);
                    return;
                }
                s_ship_data* psVar13 = &MOX::_ship[(param_2->u).scrap_ship.ship_idx];
                _sprintf(param_1, _msg_scrap_ship_hyperspace_fmt, TECHDATA::_hull_data[psVar13->d.size].name,
                             psVar13, (int)psVar13->d.cost);
                return;
            }
            case 7 /* MSG_TYPE_LOST_COLONY */: {
                char* pcVar6 = _lost_colony_reason_strings[(param_2->u).lost_colony.reason];
                char* pcVar8 = HACCESS::Get_Planet_Name_((param_2->u).lost_colony.planet_idx);
                int iVar9 = _sprintf(param_1, _msg_lost_colony_fmt,
                                         _msg_colony_or_outpost[(uint8_t)(param_2->u).lost_colony.colony_or_outpost],
                                         pcVar8, pcVar6);
                int16_t sVar4 = (param_2->u).lost_colony.leader_idx;
                if (sVar4 != -1) {
                    _sprintf(param_1 + iVar9, _msg_lost_colony_leader_fmt, &MOX::_leaders[sVar4]);
                    return;
                }
                break;
            }
            case 8 /* MSG_TYPE_HYPERSPACE_FLUX */:
                strcpy(param_1, _msg_hyperspace_flux_fmt);
                return;
            case 9 /* MSG_TYPE_HYPERSPACE_BEAST */: {
                int16_t sVar4 = (param_2->u).hyperspace_beast.ship_idx;
                if (sVar4 == -1) {
                    strcpy(param_1, _msg_hyperspace_beast_hungry_fmt);
                    return;
                }
                if ((param_2->u).hyperspace_beast.fate == 0) {
                    _sprintf(param_1, _msg_hyperspace_beast_ate_other_ship_fmt,
                                 MOX::_player[MOX::_ship[sVar4].owner].race_name);
                    return;
                }
                int iVar9 = _sprintf(param_1, _msg_hyperspace_beast_ate_your_ship_fmt, &MOX::_ship[sVar4],
                                         &MOX::_star[(param_2->u).hyperspace_beast.star_idx]);
                uint8_t cVar1 = (param_2->u).hyperspace_beast.leader_idx;
                if (cVar1 != 0xFF) {
                    _sprintf(param_1 + iVar9, _leader_fate_strings[(param_2->u).hyperspace_beast.fate],
                                 &MOX::_leaders[cVar1]);
                    return;
                }
                break;
            }
            case 10 /* MSG_TYPE_LOST_ATTACKING_SPY */:
                _sprintf(param_1, _msg_lost_attacking_spy_fmt,
                             MOX::_player[(param_2->u).lost_attacking_spy.player_idx].race_name);
                return;
            case 11 /* MSG_TYPE_LOST_DEFENDING_SPY */: {
                int16_t sVar4 = (param_2->u).lost_defending_spy.count;
                if (sVar4 != 1) {
                    _sprintf(param_1, _msg_lost_defending_spies_fmt, (int)sVar4);
                    return;
                }
                strcpy(param_1, _msg_lost_defending_spy_fmt);
                return;
            }
            case 12 /* MSG_TYPE_ARTEMIS */:
                _sprintf(param_1, _msg_artemis_fmt, &MOX::_star[(uint8_t)(param_2->u).artemis.star_idx]);
                return;
            case 13 /* MSG_TYPE_ARTEMIS_LEADER */: {
                int iVar9 = _sprintf(param_1, _msg_artemis_leader_fmt,
                                         &MOX::_star[(param_2->u).artemis_leader.star_idx]);
                _sprintf(param_1 + iVar9, _leader_fate_strings[(param_2->u).artemis_leader.fate],
                             &MOX::_leaders[(param_2->u).artemis_leader.leader_idx]);
                return;
            }
            case 14 /* MSG_TYPE_FAILED_RETREAT */: {
                uint16_t uVar5 = (param_2->u).retreat.ships_lost;
                char* pcVar6 = _msg_d_ships_killed_retreating_fmt;
                if (uVar5 == 1) {
                    pcVar6 = _msg_1_ship_killed_retreating_fmt;
                }
                int iVar9 = _sprintf(param_1, pcVar6, (uint32_t)uVar5, &MOX::_star[(uint8_t)(param_2->u).retreat.star_idx]);
                int iVar10 = _sprintf(param_1 + iVar9, GLOBALS::s_spaces_percent_s_0055236c,
                                          _failed_retreat_reason_strings[(uint8_t)(param_2->u).retreat.reason]);
                uint8_t bVar3 = (param_2->u).retreat.leader_count;
                if (bVar3 != 0) {
                    _sprintf(param_1 + iVar9 + iVar10, _msg_failed_retreat_leader_fmt, (uint32_t)bVar3);
                    return;
                }
                break;
            }
            case 15 /* MSG_TYPE_SPY_ASSASSINATION */: {
                uint8_t bVar3 = (param_2->u).spy_assasination.plr_b;
                uint8_t plr_a = (param_2->u).spy_assasination.plr_a;
                if (plr_a == bVar3) {
                    strcpy(local_c8, _msg_defensive_spy_fmt);
                } else {
                    _sprintf(local_c8, _msg_offensive_spy_fmt, MOX::_player[bVar3].name);
                }
                if ((param_2->u).spy_assasination.victim_id != MOX::_PLAYER_NUM) {
                    _sprintf(param_1, _msg_spy_assassinated_fmt, local_c8);
                    return;
                }
                if (MOX::_settings_language == 4) {
                    _sprintf(param_1, _msg_spy_assassinated_victim_fmt,
                                 &MOX::_leaders[(uint8_t)(param_2->u).spy_assasination.leader_id], local_c8,
                                 MOX::_player[plr_a].name);
                    return;
                }
                _sprintf(param_1, _msg_spy_assassinated_victim_fmt,
                             &MOX::_leaders[(uint8_t)(param_2->u).spy_assasination.leader_id],
                             MOX::_player[plr_a].name, local_c8);
                return;
            }
            case 16 /* MSG_TYPE_ANOMALY */:
                _sprintf(param_1, _msg_anomaly_fmt, &MOX::_star[(param_2->u).anomaly.star_idx]);
                return;
            case 17 /* MSG_TYPE_DIPLOMACY */:
                _sprintf(param_1, _msg_diplomacy_strings[(uint8_t)(param_2->u).diplomacy.diplomacy_idx],
                             MOX::_player[(uint8_t)(param_2->u).diplomacy.plr_a].race_name,
                             MOX::_player[(uint8_t)(param_2->u).diplomacy.plr_b].race_name);
                return;
            case 18 /* MSG_TYPE_EEL_SPAWNED */:
                _sprintf(param_1, _msg_eel_spawned_fmt, &MOX::_star[(param_2->u).eel.star_idx]);
                return;
            case 19 /* MSG_TYPE_EEL_VANISHED */:
                _sprintf(param_1, _msg_eel_vanished_fmt, &MOX::_star[(param_2->u).eel.star_idx]);
                return;
            case 20 /* MSG_TYPE_ARTIFACTS */:
                if ((param_2->u).artifacts.tech2 == '\0') {
                    _sprintf(local_c8, MOX::s_fmt_s,
                                 TECHDATA::_technology_applications[(uint8_t)(param_2->u).artifacts.tech1].name);
                } else {
                    KEN::Ken_Get_Text_Message_(0x68, local_dc, 0x13);
                    _sprintf(local_c8, GLOBALS::s_percent_s_percent_s_percent_s_005574f4,
                                 TECHDATA::_technology_applications[(uint8_t)(param_2->u).artifacts.tech1].name, local_dc,
                                 TECHDATA::_technology_applications[(uint8_t)(param_2->u).artifacts.tech2].name);
                }
                _sprintf(param_1, _msg_artifacts_fmt, local_c8, &MOX::_star[(param_2->u).artifacts.star_idx]);
                return;
            case 21 /* MSG_TYPE_APRIL1 */:
                strcpy(param_1, _msg_april1_fmt);
                return;
            case 22 /* MSG_TYPE_SCRAP_LEADER */:
                _sprintf(param_1, _msg_scrap_leader_fmt,
                             &MOX::_leaders[(uint8_t)(param_2->u).scrap_leader.leader_idx]);
                return;
            default:
                break;
        }
        return;
    }
}