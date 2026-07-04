#include "pch.h"

namespace MSG_ {
    namespace {
        constexpr int16_t DIPLOMACY_MSG_WAR = 0;
        constexpr int16_t DIPLOMACY_MSG_PEACE = 1;
        constexpr int16_t DIPLOMACY_MSG_ALLIANCE = 2;
        constexpr int16_t DIPLOMACY_MSG_NONAGGRESSION = 3;
        constexpr int16_t DIPLOMACY_MSG_PEACE_ENDED = 4;
    }

    uint8_t _first_msg[MAX_PLAYERS];
    uint8_t _n_msgs[MAX_PLAYERS];
    s_MSG _msgs[100];
    uint8_t _max_msgs[MAX_PLAYERS];

    char* _msg_artifacts_fmt;
    char* _msg_colony_your_rebellion_lost_fmt;
    char* _msg_colony_starvation_food_fmt;
    char* _msg_colony_1_rebel_won_fmt;
    char* _msg_colony_assimilation_fmt;
    char* _msg_scrap_leader_fmt;
    char* _msg_colony_genocide_fmt;
    char* _msg_hyperspace_beast_hungry_fmt;
    char* _msg_lost_ship_fmt;
    char* _s_and_s_declare_war_fmt;
    char* _msg_colony_built_freighter_fmt;
    char* _msg_lost_attacking_spy_fmt;
    char* _msg_1_ship_killed_retreating_fmt;
    char* _msg_colony_built_next_queue_fmt;
    char* _lost_colony_reason_strings[4];
    char* _msg_lost_defending_spies_fmt;
    char* _s_and_s_declare_alliance_fmt;
    char* _msg_colony_d_rebels_won_fmt;
    char* _msg_hyperspace_flux_fmt;
    char* _msg_financial_fmt;
    char* _msg_lost_colony_leader_fmt;
    char* _msg_colony_built_android_farmer_fmt;
    char* _native_msg;
    char* _msg_scrap_ship_hyperspace_fmt;
    char* _msg_offensive_spy_fmt;
    char* _msg_colony_built_android_scientist_fmt;
    char* _msg_lost_defending_spy_fmt;
    char* _msg_april1_fmt;
    char* _msg_lost_colony_fmt;
    char* _msg_colony_boom_fmt;
    char* _msg_defensive_spy_fmt;
    char* _msg_eel_vanished_fmt;
    char* _msg_colony_built_ship_fmt;
    char* _leader_fate_strings[3];
    char* _msg_colony_built_auto_build_fmt;
    char* _msg_colony_android_killed_organic_fmt;
    char* _msg_colony_settlers_arrived_blockaded_fmt;
    char* _msg_colony_comet_fmt;
    char* _msg_colony_d_settlers_arrived_fmt;
    char* _msg_colony_starvation_ind_fmt;
    char* _msg_d_ships_killed_retreating_fmt;
    char* _msg_failed_retreat_leader_fmt;
    char* _msg_colony_or_outpost[2];
    char* _msg_colony_1_settler_arrived_over_limit_fmt;
    char* _msg_spy_assassinated_fmt;
    char* _msg_spy_assassinated_victim_fmt;
    char* _msg_colony_no_room_for_android_fmt;
    char* _failed_retreat_reason_strings[3];
    char* _msg_eel_spawned_fmt;
    char* _msg_scrap_ship_bc_fmt;
    char* _msg_colony_d_rebels_lost_fmt;
    char* _msg_colony_scrap_building_fmt;
    char* _msg_hyperspace_beast_ate_other_ship_fmt;
    char* _msg_colony_built_android_worker_fmt;
    char* _msg_colony_built_refit_fmt;
    char* _msg_artemis_leader_fmt;
    char* _msg_colony_your_rebellion_won_fmt;
    char* _msg_colony_plague_fmt;
    char* _msg_anomaly_fmt;
    char* _msg_diplomacy_strings[5];
    char* _msg_colony_starvation_both_fmt;
    char* _msg_colony_built_spy_fmt;
    char* _msg_colony_built_building_fmt;
    char* _msg_artemis_fmt;
    char* _msg_colony_1_rebel_lost_fmt;
    char* _msg_hyperspace_beast_ate_your_ship_fmt;
    char* _msg_colony_1_settler_arrived_fmt;
    char* _msg_colony_d_settlers_arrived_over_limit_fmt;

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

        // Reset message tracking arrays for all potential players
        for (int i = 0; i < MAX_PLAYERS; ++i) {
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

    void __cdecl sprintf_msg_(char* out_text, uint32_t out_text_size, s_MSG* msg) {
        char and_text[20];
        char append_text[200];
        char details_text[200];

        if (out_text == nullptr || out_text_size == 0) {
            return;
        }

        out_text[0] = '\0';

        switch (msg->msg_type) {
            case MSG_TYPE_COLONY: {
                const auto& colony_msg = msg->u.colony;
                const char* planet_name = HACCESS::Get_Planet_Name_((int16_t)colony_msg.planet_idx);

                switch (colony_msg.subtype) {
                    case 0: {
                        const auto& scrap_msg = colony_msg.c.scrap;
                        snprintf(out_text, out_text_size, _msg_colony_scrap_building_fmt,
                                     TECHDATA::_buildings[scrap_msg.building_id].name, planet_name,
                                     (int)scrap_msg.bc);
                        return;
                    }
                    case 1: {
                        const auto& build_msg = colony_msg.c.build;
                        int16_t build_id = build_msg.build_id;
                        if (build_id < BUILDING_ALIEN_CONTROL_CENTER || build_id >= BUILDING_COUNT) {
                            if (COLBLDG::Colony_Production_Is_Queued_Ship_(build_id)) {
                                const char* build_fmt = _msg_colony_built_ship_fmt;
                                if (build_msg.is_refit != 0) {
                                    build_fmt = _msg_colony_built_refit_fmt;
                                }
                                snprintf(out_text, out_text_size, build_fmt, planet_name, &MOX::_ship[COLBLDG::Colony_Production_Ship_Index_(build_id)]);
                            } else {
                                switch (build_id) {
                                    case COLONY_PRODUCTION_FREIGHTERS:
                                        snprintf(out_text, out_text_size, _msg_colony_built_freighter_fmt, planet_name);
                                        break;
                                    case COLONY_PRODUCTION_SPY:
                                        snprintf(out_text, out_text_size, _msg_colony_built_spy_fmt, planet_name);
                                        break;
                                    case COLONY_PRODUCTION_SCIENTIST:
                                        snprintf(out_text, out_text_size, _msg_colony_built_android_scientist_fmt, planet_name);
                                        break;
                                    case COLONY_PRODUCTION_WORKER:
                                        snprintf(out_text, out_text_size, _msg_colony_built_android_worker_fmt, planet_name);
                                        break;
                                    case COLONY_PRODUCTION_FARMER:
                                        snprintf(out_text, out_text_size, _msg_colony_built_android_farmer_fmt, planet_name);
                                        break;
                                    default:
                                        break;
                                }
                            }
                        } else {
                            snprintf(out_text, out_text_size, _msg_colony_built_building_fmt, planet_name, TECHDATA::_buildings[build_id].name);
                        }

                        int16_t product_id = build_msg.product_id;
                        if (product_id != COLONY_PRODUCTION_NONE) {
                            const char* queue_fmt = _msg_colony_built_next_queue_fmt;
                            if (build_msg.append_fmt != 0) {
                                queue_fmt = _msg_colony_built_auto_build_fmt;
                            }
                            const char* product_name = AIBUILD::Product_Name_(product_id);
                            snprintf(details_text, sizeof(details_text), queue_fmt, product_name);
                            strlcat(out_text, details_text, out_text_size);
                            return;
                        }
                        break;
                    }
                    case 2: {
                        uint32_t race_idx = (uint32_t)colony_msg.c.assimilation.val;
                        const char* victim_race_name = _native_msg;
                        if ((int)race_idx < (int)MOX::_NUM_PLAYERS) {
                            victim_race_name = MOX::_player[race_idx].race_name;
                        }
                        snprintf(out_text, out_text_size, _msg_colony_genocide_fmt, victim_race_name, planet_name);
                        return;
                    }
                    case 3:
                        snprintf(out_text, out_text_size, _msg_colony_assimilation_fmt,
                                     MOX::_player[colony_msg.c.assimilation.val].race_name, planet_name);
                        return;
                    case 4: {
                        uint8_t food_loss = colony_msg.c.starvation.a;
                        if (food_loss == 0) {
                            snprintf(out_text, out_text_size, _msg_colony_starvation_ind_fmt, planet_name,
                                         (uint32_t)(uint8_t)colony_msg.c.starvation.b);
                            return;
                        }
                        uint8_t industry_loss = colony_msg.c.starvation.b;
                        if (industry_loss != 0) {
                            snprintf(out_text, out_text_size, _msg_colony_starvation_both_fmt, planet_name, (uint32_t)food_loss, (uint32_t)industry_loss);
                            return;
                        }
                        snprintf(out_text, out_text_size, _msg_colony_starvation_food_fmt, planet_name, (uint32_t)food_loss);
                        return;
                    }
                    case 5: {
                        const auto& settlers_msg = colony_msg.c.settlers;
                        uint8_t settler_count = settlers_msg.settlers;
                        const char* settlers_fmt = _msg_colony_d_settlers_arrived_fmt;
                        if (settler_count == 1) {
                            settlers_fmt = _msg_colony_1_settler_arrived_fmt;
                        }
                        snprintf(out_text, out_text_size, settlers_fmt, (uint32_t)settler_count, planet_name);
                        if (settlers_msg.blockaded != 0) {
                            strlcat(out_text, _msg_colony_settlers_arrived_blockaded_fmt, out_text_size);
                            return;
                        }
                        uint8_t over_limit = settlers_msg.over_limit;
                        if (over_limit != 0) {
                            const char* over_limit_fmt = _msg_colony_d_settlers_arrived_over_limit_fmt;
                            if (over_limit == 1) {
                                over_limit_fmt = _msg_colony_1_settler_arrived_over_limit_fmt;
                            }
                            snprintf(append_text, sizeof(append_text), over_limit_fmt, (uint32_t)over_limit);
                            strlcat(out_text, append_text, out_text_size);
                        }
                        return;
                    }
                    case 6:
                        snprintf(out_text, out_text_size, _msg_colony_plague_fmt, planet_name);
                        return;
                    case 7:
                        snprintf(out_text, out_text_size, _msg_colony_boom_fmt, planet_name);
                        return;
                    case 8:
                        snprintf(out_text, out_text_size, _msg_colony_comet_fmt, planet_name);
                        return;
                    case 9: {
                        const auto& rebels_msg = colony_msg.c.rebels;
                        uint32_t owner_player_idx = (uint32_t)(uint8_t)rebels_msg.owner_player;
                        uint8_t rebel_count = rebels_msg.rebels;
                        if ((int)MOX::_PLAYER_NUM != (int)owner_player_idx) {
                            if ((uint8_t)rebels_msg.lost < rebel_count) {
                                const char* rebellion_fmt = _msg_colony_d_rebels_won_fmt;
                                if (rebel_count == 1) {
                                    rebellion_fmt = _msg_colony_1_rebel_won_fmt;
                                }
                                snprintf(out_text, out_text_size, rebellion_fmt, (uint32_t)rebel_count, MOX::_player[owner_player_idx].race_name, planet_name,
                                             (uint32_t)(uint8_t)rebels_msg.lost);
                                return;
                            }
                            const char* rebellion_fmt = _msg_colony_d_rebels_lost_fmt;
                            if (rebel_count == 1) {
                                rebellion_fmt = _msg_colony_1_rebel_lost_fmt;
                            }
                            snprintf(out_text, out_text_size, rebellion_fmt, (uint32_t)rebel_count, MOX::_player[owner_player_idx].race_name, planet_name);
                            return;
                        }
                        uint8_t lost_rebels = rebels_msg.lost;
                        if (lost_rebels < rebel_count) {
                            snprintf(out_text, out_text_size, _msg_colony_your_rebellion_won_fmt, (uint32_t)rebel_count, planet_name, (uint32_t)lost_rebels);
                            return;
                        }
                        snprintf(out_text, out_text_size, _msg_colony_your_rebellion_lost_fmt, (uint32_t)rebel_count, planet_name);
                        return;
                    }
                    case 10:
                        snprintf(out_text, out_text_size, _msg_colony_no_room_for_android_fmt, planet_name);
                        return;
                    case 11: {
                        uint8_t race_idx = colony_msg.c.android_killed.race_idx;
                        const char* victim_race_name = _native_msg;
                        if (race_idx < MAX_PLAYERS) {
                            victim_race_name = MOX::_player[race_idx].race_name;
                        }
                        snprintf(out_text, out_text_size, _msg_colony_android_killed_organic_fmt, planet_name, victim_race_name);
                        return;
                    }
                }
                break;
            }
            case MSG_TYPE_DECLARE_ALLIANCE:
                snprintf(out_text, out_text_size, _s_and_s_declare_alliance_fmt,
                             MOX::_player[msg->u.players.player_a_idx].race_name,
                             MOX::_player[msg->u.players.player_b_idx].race_name);
                return;
            case MSG_TYPE_DECLARE_WAR:
                snprintf(out_text, out_text_size, _s_and_s_declare_war_fmt,
                             MOX::_player[msg->u.players.player_a_idx].race_name,
                             MOX::_player[msg->u.players.player_b_idx].race_name);
                return;
            case MSG_TYPE_FINANCIAL:
                snprintf(out_text, out_text_size, _msg_financial_fmt, (int)msg->u.financial.a,
                             (int)msg->u.financial.b);
                return;
            case MSG_TYPE_LOST_SHIP: {
                const auto& lost_ship_msg = msg->u.lost_ship;
                s_ship_data* ship = &MOX::_ship[lost_ship_msg.ship_idx];
                snprintf(out_text, out_text_size, _msg_lost_ship_fmt, TECHDATA::_hull_data[ship->d.size].name, ship,
                                         (int)ship->d.cost, &MOX::_star[lost_ship_msg.star_idx]);
                uint8_t leader_idx = lost_ship_msg.leader_idx;
                if (leader_idx != 0xFF) {
                    snprintf(append_text, sizeof(append_text), _leader_fate_strings[lost_ship_msg.fate],
                                 &MOX::_leaders[leader_idx]);
                    strlcat(out_text, append_text, out_text_size);
                    return;
                }
                break;
            }
            case MSG_TYPE_SCRAP_SHIP: {
                const auto& scrap_ship_msg = msg->u.scrap_ship;
                int16_t location_idx = scrap_ship_msg.where;
                s_ship_data* ship = &MOX::_ship[scrap_ship_msg.ship_idx];
                if (location_idx < MOX::_NUM_STARS) {
                    snprintf(out_text, out_text_size, _msg_scrap_ship_bc_fmt, TECHDATA::_hull_data[ship->d.size].name, ship,
                                 (int)ship->d.cost, &MOX::_star[location_idx], (int)scrap_ship_msg.bc);
                    return;
                }
                snprintf(out_text, out_text_size, _msg_scrap_ship_hyperspace_fmt, TECHDATA::_hull_data[ship->d.size].name,
                             ship, (int)ship->d.cost);
                return;
            }
            case MSG_TYPE_LOST_COLONY: {
                const auto& lost_colony_msg = msg->u.lost_colony;
                const char* reason_text = _lost_colony_reason_strings[lost_colony_msg.reason];
                const char* planet_name = HACCESS::Get_Planet_Name_(lost_colony_msg.planet_idx);
                snprintf(out_text, out_text_size, _msg_lost_colony_fmt,
                                         _msg_colony_or_outpost[(uint8_t)lost_colony_msg.colony_or_outpost],
                                         planet_name, reason_text);
                int16_t leader_idx = lost_colony_msg.leader_idx;
                if (leader_idx != -1) {
                    snprintf(append_text, sizeof(append_text), _msg_lost_colony_leader_fmt, &MOX::_leaders[leader_idx]);
                    strlcat(out_text, append_text, out_text_size);
                    return;
                }
                break;
            }
            case MSG_TYPE_HYPERSPACE_FLUX:
                strlcpy(out_text, _msg_hyperspace_flux_fmt, out_text_size);
                return;
            case MSG_TYPE_HYPERSPACE_BEAST: {
                const auto& beast_msg = msg->u.hyperspace_beast;
                int16_t ship_idx = beast_msg.ship_idx;
                if (ship_idx == -1) {
                    strlcpy(out_text, _msg_hyperspace_beast_hungry_fmt, out_text_size);
                    return;
                }
                if (beast_msg.fate == 0) {
                    snprintf(out_text, out_text_size, _msg_hyperspace_beast_ate_other_ship_fmt,
                                 MOX::_player[MOX::_ship[ship_idx].owner].race_name);
                    return;
                }
                snprintf(out_text, out_text_size, _msg_hyperspace_beast_ate_your_ship_fmt, &MOX::_ship[ship_idx],
                                         &MOX::_star[beast_msg.star_idx]);
                uint8_t officer_idx = beast_msg.officer_idx;
                if (officer_idx != 0xFF) {
                    snprintf(append_text, sizeof(append_text), _leader_fate_strings[(uint8_t)beast_msg.leader_status],
                                 &MOX::_leaders[officer_idx]);
                    strlcat(out_text, append_text, out_text_size);
                    return;
                }
                break;
            }
            case MSG_TYPE_LOST_ATTACKING_SPY:
                snprintf(out_text, out_text_size, _msg_lost_attacking_spy_fmt,
                             MOX::_player[msg->u.lost_attacking_spy.player_idx].race_name);
                return;
            case MSG_TYPE_LOST_DEFENDING_SPY: {
                int16_t spy_count = msg->u.lost_defending_spy.count;
                if (spy_count != 1) {
                    snprintf(out_text, out_text_size, _msg_lost_defending_spies_fmt, (int)spy_count);
                    return;
                }
                strlcpy(out_text, _msg_lost_defending_spy_fmt, out_text_size);
                return;
            }
            case MSG_TYPE_ARTEMIS:
                snprintf(out_text, out_text_size, _msg_artemis_fmt, &MOX::_star[(uint8_t)msg->u.artemis.star_idx]);
                return;
            case MSG_TYPE_ARTEMIS_LEADER: {
                const auto& artemis_leader_msg = msg->u.artemis_leader;
                snprintf(out_text, out_text_size, _msg_artemis_leader_fmt,
                                         &MOX::_star[artemis_leader_msg.star_idx]);
                snprintf(append_text, sizeof(append_text), _leader_fate_strings[artemis_leader_msg.fate],
                             &MOX::_leaders[artemis_leader_msg.leader_idx]);
                strlcat(out_text, append_text, out_text_size);
                return;
            }
            case MSG_TYPE_FAILED_RETREAT: {
                const auto& retreat_msg = msg->u.retreat;
                uint16_t ships_lost = retreat_msg.ships_lost;
                const char* retreat_fmt = _msg_d_ships_killed_retreating_fmt;
                if (ships_lost == 1) {
                    retreat_fmt = _msg_1_ship_killed_retreating_fmt;
                }
                snprintf(out_text, out_text_size, retreat_fmt, (uint32_t)ships_lost, &MOX::_star[(uint8_t)retreat_msg.star_idx]);
                snprintf(details_text, sizeof(details_text), "  %s",
                         _failed_retreat_reason_strings[(uint8_t)retreat_msg.reason]);
                strlcat(out_text, details_text, out_text_size);
                uint8_t leader_count = retreat_msg.leader_count;
                if (leader_count != 0) {
                    snprintf(append_text, sizeof(append_text), _msg_failed_retreat_leader_fmt, (uint32_t)leader_count);
                    strlcat(out_text, append_text, out_text_size);
                    return;
                }
                break;
            }
            case MSG_TYPE_SPY_ASSASSINATION: {
                const auto& assassination_msg = msg->u.spy_assasination;
                uint8_t defending_player_idx = assassination_msg.plr_b;
                uint8_t attacking_player_idx = assassination_msg.plr_a;
                if (attacking_player_idx == defending_player_idx) {
                    strlcpy(details_text, _msg_defensive_spy_fmt, sizeof(details_text));
                } else {
                    snprintf(details_text, sizeof(details_text), _msg_offensive_spy_fmt, MOX::_player[defending_player_idx].name);
                }
                if (assassination_msg.victim_id != MOX::_PLAYER_NUM) {
                    snprintf(out_text, out_text_size, _msg_spy_assassinated_fmt, details_text);
                    return;
                }
                if (MOX::_settings.language == 4) {
                    snprintf(out_text, out_text_size, _msg_spy_assassinated_victim_fmt,
                                 &MOX::_leaders[(uint8_t)assassination_msg.leader_id], details_text,
                                 MOX::_player[attacking_player_idx].name);
                    return;
                }
                snprintf(out_text, out_text_size, _msg_spy_assassinated_victim_fmt,
                             &MOX::_leaders[(uint8_t)assassination_msg.leader_id],
                             MOX::_player[attacking_player_idx].name, details_text);
                return;
            }
            case MSG_TYPE_ANOMALY:
                snprintf(out_text, out_text_size, _msg_anomaly_fmt, &MOX::_star[msg->u.anomaly.star_idx]);
                return;
            case MSG_TYPE_DIPLOMACY:
                snprintf(out_text, out_text_size, _msg_diplomacy_strings[(uint8_t)msg->u.diplomacy.diplomacy_idx],
                             MOX::_player[(uint8_t)msg->u.diplomacy.plr_a].race_name,
                             MOX::_player[(uint8_t)msg->u.diplomacy.plr_b].race_name);
                return;
            case MSG_TYPE_EEL_SPAWNED:
                snprintf(out_text, out_text_size, _msg_eel_spawned_fmt, &MOX::_star[msg->u.eel.star_idx]);
                return;
            case MSG_TYPE_EEL_VANISHED:
                snprintf(out_text, out_text_size, _msg_eel_vanished_fmt, &MOX::_star[msg->u.eel.star_idx]);
                return;
            case MSG_TYPE_ARTIFACTS: {
                const auto& artifacts_msg = msg->u.artifacts;
                if (artifacts_msg.tech2 == '\0') {
                    snprintf(details_text, sizeof(details_text), MOX::s_fmt_s,
                                 TECHDATA::_technology_applications[(uint8_t)artifacts_msg.tech1].name);
                } else {
                    KEN::Ken_Get_Text_Message_(0x68, and_text, 0x13);
                    snprintf(details_text, sizeof(details_text), "%s %s %s",
                                 TECHDATA::_technology_applications[(uint8_t)artifacts_msg.tech1].name, and_text,
                                 TECHDATA::_technology_applications[(uint8_t)artifacts_msg.tech2].name);
                }
                snprintf(out_text, out_text_size, _msg_artifacts_fmt, details_text, &MOX::_star[artifacts_msg.star_idx]);
                return;
            }
            case MSG_TYPE_APRIL1:
                strlcpy(out_text, _msg_april1_fmt, out_text_size);
                return;
            case MSG_TYPE_SCRAP_LEADER:
                snprintf(out_text, out_text_size, _msg_scrap_leader_fmt,
                             &MOX::_leaders[(uint8_t)msg->u.scrap_leader.leader_idx]);
                return;
            default:
                break;
        }
        return;
    }

    void __cdecl Add_Ongoing_Event_Msgs_() {
        s_MSG msg;

        if (EVENTS::Event_Check_Hyperspace_Flux_() != 0) {
            memset(&msg, 0, sizeof(msg));
            msg.msg_type = MSG_TYPE_HYPERSPACE_FLUX;
            msg.priority = 0;

            for (int16_t player_idx = 0; player_idx < MOX::_NUM_PLAYERS; ++player_idx) {
                Add_Msg_(msg, player_idx);
            }
        }

        for (int16_t colony_idx = 0; colony_idx < MOX::_NUM_COLONIES; ++colony_idx) {
            int8_t owner = MOX::_colony[colony_idx].owner;
            if (owner >= 0) {
                if (EVENTS::Event_Check_Plague_(colony_idx) != 0) {
                    memset(&msg, 0, sizeof(msg));
                    msg.msg_type = MSG_TYPE_COLONY;
                    msg.u.colony.planet_idx = (uint8_t)MOX::_colony[colony_idx].planet;
                    msg.u.colony.subtype = 6;
                    Add_Msg_(msg, owner);
                }

                if (EVENTS::Event_Check_Population_Boom_(colony_idx) != 0) {
                    memset(&msg, 0, sizeof(msg));
                    msg.msg_type = MSG_TYPE_COLONY;
                    msg.u.colony.planet_idx = (uint8_t)MOX::_colony[colony_idx].planet;
                    msg.u.colony.subtype = 7;
                    Add_Msg_(msg, owner);
                }

                if (EVENTS::Event_Check_Comet_(colony_idx) != 0) {
                    memset(&msg, 0, sizeof(msg));
                    msg.msg_type = MSG_TYPE_COLONY;
                    msg.u.colony.planet_idx = (uint8_t)MOX::_colony[colony_idx].planet;
                    msg.u.colony.subtype = 8;
                    Add_Msg_(msg, owner);
                }
            }
        }

        for (int16_t star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
            if (EVENTS::Event_Check_Space_Anomaly_At_Star_(star_idx) != 0) {
                memset(&msg, 0, sizeof(msg));
                msg.msg_type = MSG_TYPE_ANOMALY;
                msg.u.anomaly.star_idx = (uint8_t)star_idx;

                for (int16_t player_idx = 0; player_idx < MOX::_NUM_PLAYERS; ++player_idx) {
                    Add_Msg_(msg, player_idx);
                }
            }
        }

        if (MOX::_stardate == 35023) {
            time_t current_time = time(NULL);
            tm* time_info = localtime(&current_time);

            if (time_info->tm_mon == 3 /* April */ && time_info->tm_mday == 1) {
                msg.msg_type = MSG_TYPE_APRIL1;
                Add_Msg_All_(msg);
            }
        }
    }

    void __cdecl Add_Peace_Ended_Msg_(int player_a_idx, int player_b_idx) {
        Add_Diplomacy_Msg_(player_a_idx, player_b_idx, DIPLOMACY_MSG_PEACE_ENDED);
    }

    /**
     * Adds a diplomacy message indicating peace has been declared between two players.
     */
    void __cdecl Add_Peace_Msg_(int32_t player_a_idx, int32_t player_b_idx) {
        Add_Diplomacy_Msg_(player_a_idx, player_b_idx, DIPLOMACY_MSG_PEACE);
    }

    void __cdecl Add_Diplomacy_Msg_(int16_t plr_a, int16_t plr_b, int16_t diplomacy_idx) {
        s_MSG msg = {};
        msg.msg_type = MSG_TYPE_DIPLOMACY;
        msg.u.diplomacy.diplomacy_idx = static_cast<int8_t>(diplomacy_idx);
        msg.u.diplomacy.plr_a = static_cast<int8_t>(plr_a);
        msg.u.diplomacy.plr_b = static_cast<int8_t>(plr_b);

        if (MOX::_NUM_PLAYERS > 0) {
            for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                if (diplomacy_idx == DIPLOMACY_MSG_PEACE_ENDED) {
                    if (i == plr_a || i == plr_b) {
                        Add_Msg_(msg, i);
                    }
                } else {
                    if (MOX::_player[i].contact[plr_a] != 0 || MOX::_player[i].contact[plr_b] != 0) {
                        Add_Msg_(msg, i);
                    }
                }
            }
        }
    }

    void __cdecl Add_Nonaggression_Msg_(int16_t plr_a, int16_t plr_b) {
        Add_Diplomacy_Msg_(plr_a, plr_b, DIPLOMACY_MSG_NONAGGRESSION);
    }

    void __cdecl Add_Msg_All_(s_MSG msg) {
        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            Add_Msg_(msg, i);
        }
    }

    void __cdecl Add_Alliance_Msg_(uint32_t player_a_idx, uint32_t player_b_idx) {
        Add_Diplomacy_Msg_(player_a_idx, player_b_idx, DIPLOMACY_MSG_ALLIANCE);
    }

    void __cdecl Add_War_Msg_(int32_t player_a, int32_t player_b) {
        Add_Diplomacy_Msg_(player_a, player_b, DIPLOMACY_MSG_WAR);
    }

    /**
     * @brief Transitions to the colony screen for a colony mentioned in a message.
     *
     * If the message is of type MSG_TYPE_COLONY and the player owns the colony,
     * this function updates the screen state to navigate to that colony's management view.
     */
    int8_t __cdecl Goto_Msg_Colony_(s_MSG* msg) {
        if (msg->msg_type == MSG_TYPE_COLONY) {
            uint8_t planet_idx = msg->u.colony.planet_idx;
            int16_t colony_idx = MOX::_planet[planet_idx].colony_index;

            if (colony_idx != -1) {
                s_colony* colony = &MOX::_colony[colony_idx];

                if (colony->owner == MOX::_PLAYER_NUM) {
                    MOX::_return_screen = SCREEN_INFO;
                    MOX::_current_screen = SCREEN_COLONY;

                    s_planet_data* planet = &MOX::_planet[colony->planet];
                    MOX::_screen_data = (int16_t)planet->star_index;
                    COLONY::_orbit_temp = (int16_t)planet->orbit;

                    return 1;
                }
            }
        }

        return 0;
    }

    char* __cdecl Load_Msg_Strings_(char* native_msg) {
        auto next_string = [](char* s) -> char* {
            return s + strlen(s) + 1;
        };

        char* cursor = native_msg;

        _native_msg = cursor;
        cursor = next_string(cursor);

        _s_and_s_declare_alliance_fmt = cursor;
        cursor = next_string(cursor);

        _s_and_s_declare_war_fmt = cursor;
        cursor = next_string(cursor);

        _msg_financial_fmt = cursor;
        cursor = next_string(cursor);

        _msg_lost_ship_fmt = cursor;
        cursor = next_string(cursor);

        _msg_scrap_ship_bc_fmt = cursor;
        cursor = next_string(cursor);

        _msg_scrap_ship_hyperspace_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_or_outpost[0] = cursor;
        cursor = next_string(cursor);

        _msg_colony_or_outpost[1] = cursor;
        cursor = next_string(cursor);

        _msg_lost_colony_fmt = cursor;
        cursor = next_string(cursor);

        _msg_lost_colony_leader_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_scrap_building_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_built_building_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_built_ship_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_built_refit_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_built_spy_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_built_android_farmer_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_built_android_worker_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_built_android_scientist_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_built_freighter_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_built_next_queue_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_built_auto_build_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_genocide_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_assimilation_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_starvation_food_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_starvation_ind_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_starvation_both_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_1_settler_arrived_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_d_settlers_arrived_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_settlers_arrived_blockaded_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_1_settler_arrived_over_limit_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_d_settlers_arrived_over_limit_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_plague_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_boom_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_comet_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_your_rebellion_won_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_your_rebellion_lost_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_1_rebel_won_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_d_rebels_won_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_1_rebel_lost_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_d_rebels_lost_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_no_room_for_android_fmt = cursor;
        cursor = next_string(cursor);

        _msg_colony_android_killed_organic_fmt = cursor;
        cursor = next_string(cursor);

        _msg_hyperspace_flux_fmt = cursor;
        cursor = next_string(cursor);

        _msg_hyperspace_beast_hungry_fmt = cursor;
        cursor = next_string(cursor);

        _msg_hyperspace_beast_ate_your_ship_fmt = cursor;
        cursor = next_string(cursor);

        _msg_hyperspace_beast_ate_other_ship_fmt = cursor;
        cursor = next_string(cursor);

        _msg_lost_attacking_spy_fmt = cursor;
        cursor = next_string(cursor);

        _msg_lost_defending_spy_fmt = cursor;
        cursor = next_string(cursor);

        _msg_lost_defending_spies_fmt = cursor;
        cursor = next_string(cursor);

        _msg_artemis_fmt = cursor;
        cursor = next_string(cursor);

        _msg_artemis_leader_fmt = cursor;
        cursor = next_string(cursor);

        _msg_1_ship_killed_retreating_fmt = cursor;
        cursor = next_string(cursor);

        _msg_d_ships_killed_retreating_fmt = cursor;
        cursor = next_string(cursor);

        _msg_defensive_spy_fmt = cursor;
        cursor = next_string(cursor);

        _msg_offensive_spy_fmt = cursor;
        cursor = next_string(cursor);

        _msg_spy_assassinated_victim_fmt = cursor;
        cursor = next_string(cursor);

        _msg_spy_assassinated_fmt = cursor;
        cursor = next_string(cursor);

        _msg_anomaly_fmt = cursor;
        cursor = next_string(cursor);

        _msg_diplomacy_strings[0] = cursor;
        cursor = next_string(cursor);

        _msg_diplomacy_strings[1] = cursor;
        cursor = next_string(cursor);

        _msg_diplomacy_strings[2] = cursor;
        cursor = next_string(cursor);

        _msg_diplomacy_strings[3] = cursor;
        cursor = next_string(cursor);

        _msg_diplomacy_strings[4] = cursor;
        cursor = next_string(cursor);

        _msg_eel_spawned_fmt = cursor;
        cursor = next_string(cursor);

        _msg_eel_vanished_fmt = cursor;
        cursor = next_string(cursor);

        _msg_artifacts_fmt = cursor;
        cursor = next_string(cursor);

        _msg_april1_fmt = cursor;
        cursor = next_string(cursor);

        _msg_scrap_leader_fmt = cursor;
        cursor = next_string(cursor);

        _failed_retreat_reason_strings[0] = cursor;
        cursor = next_string(cursor);

        _failed_retreat_reason_strings[1] = cursor;
        cursor = next_string(cursor);

        _failed_retreat_reason_strings[2] = cursor;
        cursor = next_string(cursor);

        _msg_failed_retreat_leader_fmt = cursor;
        cursor = next_string(cursor);

        _leader_fate_strings[0] = cursor;
        cursor = next_string(cursor);

        _leader_fate_strings[1] = cursor;
        cursor = next_string(cursor);

        _leader_fate_strings[2] = cursor;
        cursor = next_string(cursor);

        _lost_colony_reason_strings[0] = cursor;
        cursor = next_string(cursor);

        _lost_colony_reason_strings[1] = cursor;
        cursor = next_string(cursor);

        _lost_colony_reason_strings[2] = cursor;
        cursor = next_string(cursor);

        _lost_colony_reason_strings[3] = cursor;
        cursor = next_string(cursor);

        return cursor;
    }
}