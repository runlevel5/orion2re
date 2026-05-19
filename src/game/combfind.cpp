#include "pch.h"

namespace COMBFIND {

void __cdecl Search_For_Battles_() {
    MOX2::action_func_t saved_auto_fn;
    MOX2::Get_Auto_Function_(&saved_auto_fn);

    if (MOX::_game_type < 2) {
        fields::Assign_Auto_Function_(MAINSCR::Draw_Mini_Main_Screen_, 1);
    }

    MAINSCR::Update_Ship_Icons_In_Between_Turns_();

    s_combfind_star_has_player star_has_player_stack[73][8];
    uint8_t star_has_nonplayer_stack[73];
    uint16_t player_needs_to_attack_stack[73];
    int8_t battle_stars_stack[76];
    int8_t attack_confirmed_buf[8][8];

    _star_has_player = &star_has_player_stack;
    memset(&star_has_player_stack, 0, sizeof(star_has_player_stack));

    _star_has_nonplayer = &star_has_nonplayer_stack;
    memset(&star_has_nonplayer_stack, 0, sizeof(star_has_nonplayer_stack));

    _player_needs_to_attack = &player_needs_to_attack_stack;
    memset(&player_needs_to_attack_stack, 0, sizeof(player_needs_to_attack_stack));

    _battle_stars = battle_stars_stack;

    for (int16_t i = 0; i < MOX::_NUM_COLONIES; ++i) {
        int8_t owner = MOX::_colony[i].owner;
        if (owner != -1) {
            int16_t planet_idx = MOX::_colony[i].planet;
            int8_t star_idx = MOX::_planet[planet_idx].star_index;
            (*_star_has_player)[star_idx][owner].colony_count++;
        }
    }

    (*_star_has_nonplayer)[MOX::_NUM_STARS] |= 1;

    for (int16_t i = 0; i < MOX::_NUM_SHIPS; ++i) {
        s_ship_data* ship = &MOX::_ship[i];
        int16_t loc = ship->location;
        if (loc < 72) {
            if (ship->status == 0) {
                int8_t owner = ship->owner;
                if (owner < 8) {
                    uint8_t ship_type = ship->d.ship_type;
                    (*_star_has_player)[loc][owner].has_player = 1;
                    (*_star_has_player)[loc][owner].ship_sizes[ship_type] = 1;
                    if (ship_type == SHIP_TYPE_TRANSPORT) {
                        (*_star_has_player)[loc][owner].has_transport = 1;
                    } else if (ship_type == SHIP_TYPE_COMBAT) {
                        (*_player_needs_to_attack)[loc] |= (1 << (owner & 0x1F));
                    }
                } else {
                    (*_star_has_nonplayer)[loc] |= (1 << ((owner - 8) & 0x1F));
                    (*_player_needs_to_attack)[loc] |= (1 << (owner & 0x1F));
                }
            }
        } else if (ship->d.ship_type == SHIP_TYPE_TRANSPORT && ship->status == 1) {
            int16_t star_idx = loc - 500;
            if (ship->turns_left == 1) {
                (*_star_has_player)[star_idx][ship->owner].transport_count++;
            } else {
                (*_star_has_player)[star_idx][ship->owner].transport_unk = 1;
            }
        }
    }

    _n_battle_stars = 0;
    for (int16_t i = 0; i <= MOX::_NUM_STARS + 1; ++i) {
        if ((*_player_needs_to_attack)[i] != 0) {
            if (EVENTS::Event_Check_Space_Anomaly_At_Star_(i) == 0) {
                _battle_stars[_n_battle_stars++] = (int8_t)i;
            }
        }
    }

    RUSS::Shuffle_Char_(_battle_stars, _n_battle_stars);

    if (MOX::_game_type < 2) { /* Case 0 (Single), Case 1 (Host) */
        memset(attack_confirmed_buf, 0, sizeof(attack_confirmed_buf));
        _attack_confirmed = &attack_confirmed_buf;

        while (_n_battle_stars > 0) {
            int16_t star_idx, attacker_idx;
            Pick_Star_And_Attacker_(&star_idx, &attacker_idx);
            if (star_idx != -1) {
                int16_t defender_idx = -1;
                int16_t colony_idx = -1;
                int8_t unk_flag = 0;

                if (attacker_idx < 8) {
                    if (MOX::_player[attacker_idx].objectives != 100) {
                        Get_AI_Target_(star_idx, attacker_idx, &defender_idx, &colony_idx, &unk_flag);
                    } else {
                        if ((MOX::_star[star_idx].ignore_combat_ships & (1 << (attacker_idx & 0x1F))) == 0) {
                            Get_Local_Human_Target_(star_idx, attacker_idx, &defender_idx, &colony_idx);
                            if (colony_idx == -1) {
                                if (defender_idx == -1) {
                                    MOX::_star[star_idx].ignore_combat_ships |= (1 << (attacker_idx & 0x1F));
                                }
                            }
                        }
                    }
                } else {
                    Get_NPC_Target_(star_idx, attacker_idx, &defender_idx, &colony_idx);
                }

                int16_t final_defender_idx = defender_idx;
                if (colony_idx != -1) {
                    final_defender_idx = MOX::_colony[colony_idx].owner;
                    MOX::_player[final_defender_idx].last_attacker = (uint8_t)attacker_idx;
                }

                if (attacker_idx == 12 /* Antarans */ && colony_idx != -1) {
                    int16_t bribe_planet_idx = -1;
                    if (MOX::_player[final_defender_idx].objectives == 100) {
                        bribe_planet_idx = MAINPUPS::Dragon_New_Star_Popup_(final_defender_idx, star_idx);
                    }
                    if (bribe_planet_idx != -1) {
                        // BUG: final_defender_idx was set to -1 before being passed to Do_Dragon_Bribe_
                        Do_Dragon_Bribe_(final_defender_idx, star_idx, bribe_planet_idx);
                        final_defender_idx = -1;
                    }
                }

                if (final_defender_idx != -1) {
                    int16_t planet_idx = COMBFIND::Target_Colony_To_Planet_(colony_idx);
                    MOX::_star[star_idx].ignore_combat_ships = 0;
                    Do_1_Combat_(star_idx, attacker_idx, defender_idx, planet_idx, nullptr, unk_flag);
                    _current_star_has_player = 0;
                    _current_star_has_nonplayer = 0;
                    Update_Player_Needs_To_Attack_(star_idx, attacker_idx, final_defender_idx);
                }
            } else {
                break;
            }
        }
        _attack_confirmed = nullptr;
    } else if (MOX::_game_type < 4) { /* Case 2 (Client IP), Case 3 (Ten) */
        net::state = 0xE;
        Client_Combat_Loop_();
        NETMOX::Mox_Send_Message_(0x7FFE, 0x1A, nullptr, 0);
    }

    Process_Retreating_Ships_();

    if (_messed_up_fleet_palette) {
        MAINSCR::Set_Reload_Fleet_Box_Palette_Flag_();
        _messed_up_fleet_palette = 0;
    }

    for (int i = 0; i < MOX::_NUM_STARS; ++i) {
        Do_System_Discoveries_At_Star_(&MOX::_star[i]);
    }

    MOX2::Put_Auto_Function_(&saved_auto_fn);
    _star_has_player = nullptr;
    _star_has_nonplayer = nullptr;
    _player_needs_to_attack = nullptr;
    _battle_stars = nullptr;

    COLCALC::Check_For_Eliminated_Players_();
}

void __cdecl Process_Retreating_Ships_(void) {
    struct s_msg {
        char type;
        char pad1;
        char star_idx;
        char reason;
        short count;
        char killed_officer_count;
        char pad2[13];
    };

    s_retreat_report report[8][73];
    int8_t player_retreat_target[8];
    int16_t ship_indices[500];
    s_ship_move_info move_info;
    
    memset(report, 0, sizeof(report));
    memset(player_retreat_target, 0xFF, sizeof(player_retreat_target));

    for (int i = MOX::_NUM_SHIPS - 1; i >= 0; --i) {
        s_ship_data* ship = &MOX::_ship[i];

        if (ship->location == MOX::_NUM_STARS) {
            int8_t owner = ship->owner;
            if (player_retreat_target[owner] == -1) {
                player_retreat_target[owner] = 0x48; /* 'H' */
                uint32_t valid_star_count = 0;
                for (int j = 0; j < MOX::_NUM_STARS; ++j) {
                    if ((MOX::_star[j].has_dimensional_portal >> owner) & 1) {
                        valid_star_count++;
                        if (random::Random_(valid_star_count) == 1) {
                            player_retreat_target[owner] = (int8_t)j;
                        }
                    }
                }
            }

            if (player_retreat_target[owner] == 0x48) {
                int16_t leader_idx = ship->officer_index;
                if (leader_idx != -1) {
                    if (MOX::_leaders[leader_idx].eta < 1) {
                        MOX::_leaders[leader_idx].location = -1;
                        ship->officer_index = -1;
                        OFFICER::Kill_Officer_(leader_idx);
                        report[owner][MOX::_NUM_STARS].killed_officers++;
                    } else {
                        OFFICER::Move_Officer_To_Pool_(leader_idx);
                    }
                }
                report[owner][MOX::_NUM_STARS].count++;
                report[owner][MOX::_NUM_STARS].reason = 0;
                SHIPS::Kill_Ship_((int16_t)i);
            } else {
                int target_loc = (int)player_retreat_target[owner];
                ship->location = (int16_t)target_loc;
                ship->x = MOX::_star[target_loc].x;
                ship->y = MOX::_star[target_loc].y;
                ship->status = 0;
            }
        } else if (ship->status == 9) {
            int group_count = 1;
            int16_t loc = ship->location;
            int8_t owner = ship->owner;
            ship_indices[0] = (int16_t)i;
            ship->mission = 0;

            for (int j = i - 1; j >= 0; --j) {
                s_ship_data* other = &MOX::_ship[j];
                if (other->status == 9 && other->location == loc && other->owner == owner) {
                    ship_indices[group_count++] = (int16_t)j;
                    other->mission = 0;
                }
            }

            int16_t retreat_star = Closest_Other_Colony_Star_((int16_t)owner, loc);
            bool retreat_failed = false;
            int8_t fail_reason = -1;

            if (retreat_star == -1) {
                retreat_failed = true;
                fail_reason = 2;
            } else {
                ship_indices[group_count] = (int16_t)0xFFFF;
                SHIPMOVE::Ships_Try_To_Move_To_(ship_indices, retreat_star, &move_info);
                if (move_info.moving == 0) {
                    retreat_failed = true;
                    fail_reason = (move_info.hyperspace_flux == 0) ? 2 : 1;
                }
            }

            if (retreat_failed) {
                for (int k = 0; k < group_count; ++k) {
                    int16_t idx = ship_indices[k];
                    int16_t leader_idx = MOX::_ship[idx].officer_index;
                    if (leader_idx != -1) {
                        if (MOX::_leaders[leader_idx].eta < 1) {
                            OFFICER::Kill_Officer_(leader_idx);
                            report[owner][loc].killed_officers++;
                        } else {
                            OFFICER::Move_Officer_To_Pool_(leader_idx);
                        }
                    }
                    report[owner][loc].count++;
                    report[owner][loc].reason = fail_reason;
                    SHIPS::Kill_Ship_(idx);
                }
            } else {
                ship_indices[group_count] = (int16_t)0xFFFF;
                SHIPMOVE::Make_Ships_Move_To_(ship_indices, retreat_star, &move_info);
                for (int k = 0; k < group_count; ++k) {
                    int16_t idx = ship_indices[k];
                    MOX::_ship[idx].location -= 500;
                    MOX::_ship[idx].status = 1;
                }
            }
        } else if (ship->owner >= 8) {
            if (ship->owner == 8 && ship->status == 0) {
                antarans::Restock_Antaran_Ship_((int16_t)ship->d.size);
                SHIPS::Kill_Ship_((int16_t)i);
            } else if (ship->owner > 7 && ship->status == 0 && ship->d.speed != 0) {
                bool destroy_monster = false;
                if (ship->owner == 10) {
                    destroy_monster = true;
                } else if (ship->owner == 11 || ship->owner == 12 || ship->owner == 14) {
                    if (random::Random_(2) == 1) {
                        destroy_monster = true;
                    }
                }

                if (destroy_monster) {
                    SHIPS::Kill_Ship_((int16_t)i);
                    NETMOX::Russ_Event_Monster_Update_((int16_t)ship->owner, -1, -1);
                }
            }
        }
    }

    for (int p = 0; p < MOX::_NUM_PLAYERS; ++p) {
        for (int s = 0; s < MOX::_NUM_STARS + 1; ++s) {
            if (report[p][s].count != 0) {
                s_MSG msg;
                msg.msg_type = MSG_TYPE_FAILED_RETREAT;
                msg.u.retreat.star_idx = (char)s;
                msg.u.retreat.reason = report[p][s].reason;
                msg.u.retreat.ships_lost = report[p][s].count;
                msg.u.retreat.leader_count = report[p][s].killed_officers;
                MSG_::Add_Msg_(msg, p);
            }
        }
    }

    MAINSCR::Update_Ship_Icons_In_Between_Turns_();
}

void __cdecl Client_Combat_Loop_() {
    int8_t local_attack_confirmed[8][8];
    s_battle_results local_battle_results_1;
    s_battle_results local_battle_results_2;
    s_battle_results local_battle_results_main;
    int16_t target_colony;
    int32_t new_star_target;
    int16_t result;

    NETMOX::_net.combat_results_data = &local_battle_results_main;
    memset(local_attack_confirmed, 0, sizeof(local_attack_confirmed));
    COMBFIND::_attack_confirmed = (int8_t (*)[8][8])local_attack_confirmed;

    while (NETMOX::_net.sync_state >= 14 && NETMOX::_net.sync_state < 35 /* 0x23 */) {
        RUSS::Mox_Update_();
        MULTPLAY::Chat_Box_Input_Loop_();
        RUSS::Mox_Update_();

        switch (NETMOX::_net.sync_state) {
        case 15:
            while (NETMOX::_net.received_chunk_count < NETMOX::_net.total_chunk_count) {
                RUSS::Mox_Update_();
            }
            NETMOX::Decode_Pick_Target_Info_(NETMOX::_net.network_data_ptr, NETMOX::_net.total_bytes_received);
            NETMOX::_net.network_data_ptr = nullptr;
            allocate::Pop_Block_(MOX::_screen_seg);
            NETMOX::_net.total_chunk_count = 0;
            break;

        case 16:
            fields::Assign_Auto_Function_(SCORE::nullsub, 1);
            video::Set_Page_Off_();
            MOX::_dont_draw_main_screen_to_back = 1;
            MAINSCR::Draw_Mini_Main_Screen_();
            video::Copy_Off_To_Back_();
            video::Set_Refresh_Stencil_();
            ERICNET::Mox_Play_Sound_(51 /* 0x33 */);
            COMBFIND::Get_Local_Human_Target_(NETMOX::_net.combat_star_id, MOX::_PLAYER_NUM, &NETMOX::_net.combat_defender_id, &target_colony);
            NETMOX::_net.combat_planet_id = COMBFIND::Target_Colony_To_Planet_(target_colony);
            MULTPLAY::Load_Net_Next_Turn_Screen_(0);
            NETMOX::_net.sync_state = 17;
            if (target_colony == -1) {
                if (NETMOX::_net.combat_defender_id == -1) {
                    NETMOX::_net.combat_target_player_id = -1;
                    NETMOX::_net.sync_state = 14;
                } else {
                    NETMOX::_net.combat_target_player_id = NETMOX::_net.combat_defender_id;
                }
            } else {
                NETMOX::_net.combat_target_player_id = (int16_t)MOX::_colony[target_colony].owner;
            }
            NETMOX::Mox_Send_Message_(NETMOX::_net.local_net_id, 17, &NETMOX::_net.combat_defender_id, 6);
            break;

        case 18:
            while (NETMOX::_net.received_chunk_count < NETMOX::_net.total_chunk_count) {
                RUSS::Mox_Update_();
            }
            NETMOX::Decode_You_Are_Target_Info_(NETMOX::_net.network_data_ptr, NETMOX::_net.total_bytes_received);
            NETMOX::_net.network_data_ptr = nullptr;
            allocate::Pop_Block_(MOX::_screen_seg);
            NETMOX::_net.total_chunk_count = 0;
            break;

        case 19:
            if (NETMOX::_net.combat_attacker_id < 8 && MOX::_player[(int)NETMOX::_net.combat_attacker_id].objectives == 100) {
                NETMOX::_net.sync_state = 21;
            } else {
                NETMOX::_net.sync_state = 23;
            }
            ERICNET::Mox_Play_Sound_(51 /* 0x33 */);
            break;

        case 20:
            memset(&local_battle_results_1, 0, sizeof(s_battle_results));
            local_battle_results_1.packet_type = NETMOX::_net.sync_state;
            local_battle_results_1.combat_star_id = NETMOX::_net.combat_star_id;
            local_battle_results_1.combat_defender_id = NETMOX::_net.combat_defender_id;
            local_battle_results_1.combat_planet_id = NETMOX::_net.combat_planet_id;
            local_battle_results_1.combat_target_player_id = NETMOX::_net.combat_target_player_id;
            local_battle_results_1.planet_id = NETMOX::_net.combat_planet_id;
            local_battle_results_1.combat_type = NETMOX::_net.combat_type;
            local_battle_results_1.combat_attacker_id = NETMOX::_net.combat_attacker_id;
            local_battle_results_1.combat_result = NETMOX::_net.combat_result;
            local_battle_results_1.diplomacy_target_id = NETMOX::_net.diplomacy_target_id;
            result = COMBFIND::Do_1_Combat_(NETMOX::_net.combat_star_id, MOX::_PLAYER_NUM, NETMOX::_net.combat_defender_id, NETMOX::_net.combat_planet_id, &local_battle_results_1, 0);
            local_battle_results_1.combat_result = (int8_t)result;
            NETMOX::Mox_Send_Message_(MOX::_player[NETMOX::_net.combat_target_player_id].network_player_id, 24, &local_battle_results_1, 1742);
            while (NETMOX::_net.sync_state != 14) {
                RUSS::Mox_Update_();
            }
            NETMOX::Send_Battle_Results_(NETMOX::_net.combat_star_id, MOX::_PLAYER_NUM, NETMOX::_net.combat_target_player_id, NETMOX::_net.combat_defender_id, NETMOX::_net.combat_planet_id, (int16_t)local_battle_results_1.combat_result);
            break;

        case 21:
            result = COMBFIND::Do_1_Combat_(NETMOX::_net.combat_star_id, NETMOX::_net.combat_attacker_id, NETMOX::_net.combat_defender_id, NETMOX::_net.combat_planet_id, nullptr, 0);
            if (NETMOX::_net.sync_state != 26) {
                NETMOX::_net.sync_state = 14;
                while (NETMOX::_net.sync_state != 26) {
                    RUSS::Mox_Update_();
                }
            }
            COMBFIND::Bomb_Results_Popups_(NETMOX::_net.combat_results_data);
            NETMOX::Mox_Send_Message_(MOX::_player[(int)NETMOX::_net.combat_attacker_id].network_player_id, 25, nullptr, 0);
            NETMOX::_net.sync_state = 14;
            NETMOX::Send_Battle_Results_(NETMOX::_net.combat_star_id, NETMOX::_net.combat_attacker_id, MOX::_PLAYER_NUM, NETMOX::_net.combat_defender_id, NETMOX::_net.combat_planet_id, result);
            break;

        case 22:
            result = COMBFIND::Do_1_Combat_(NETMOX::_net.combat_star_id, MOX::_PLAYER_NUM, NETMOX::_net.combat_defender_id, NETMOX::_net.combat_planet_id, nullptr, 0);
            NETMOX::_net.sync_state = 14;
            NETMOX::Send_Battle_Results_(NETMOX::_net.combat_star_id, MOX::_PLAYER_NUM, NETMOX::_net.combat_target_player_id, NETMOX::_net.combat_defender_id, NETMOX::_net.combat_planet_id, result);
            break;

        case 23:
            result = COMBFIND::Do_1_Combat_(NETMOX::_net.combat_star_id, NETMOX::_net.combat_attacker_id, NETMOX::_net.combat_defender_id, NETMOX::_net.combat_planet_id, nullptr, 0);
            NETMOX::_net.sync_state = 14;
            MULTPLAY::Load_Net_Next_Turn_Screen_(1);
            NETMOX::Send_Battle_Results_(NETMOX::_net.combat_star_id, NETMOX::_net.combat_attacker_id, MOX::_PLAYER_NUM, NETMOX::_net.combat_defender_id, NETMOX::_net.combat_planet_id, result);
            break;

        case 24:
            memset(&local_battle_results_2, 0, sizeof(s_battle_results));
            local_battle_results_2.packet_type = NETMOX::_net.sync_state;
            local_battle_results_2.combat_star_id = NETMOX::_net.combat_star_id;
            local_battle_results_2.combat_defender_id = NETMOX::_net.combat_defender_id;
            local_battle_results_2.combat_planet_id = NETMOX::_net.combat_planet_id;
            local_battle_results_2.combat_target_player_id = NETMOX::_net.combat_target_player_id;
            local_battle_results_2.planet_id = NETMOX::_net.combat_planet_id;
            local_battle_results_2.combat_type = NETMOX::_net.combat_type;
            local_battle_results_2.combat_attacker_id = NETMOX::_net.combat_attacker_id;
            local_battle_results_2.combat_result = NETMOX::_net.combat_result;
            local_battle_results_2.diplomacy_target_id = NETMOX::_net.diplomacy_target_id;
            result = COMBFIND::Do_1_Combat_(NETMOX::_net.combat_star_id, MOX::_PLAYER_NUM, NETMOX::_net.combat_defender_id, NETMOX::_net.combat_planet_id, &local_battle_results_2, 0);
            local_battle_results_2.combat_result = (int8_t)result;
            MULTPLAY::Load_Net_Next_Turn_Screen_(1);
            if (NETMOX::_net.combat_target_player_id < 8 && MOX::_player[NETMOX::_net.combat_target_player_id].objectives == 100 /* 'd' */) {
                NETMOX::Mox_Send_Message_(MOX::_player[NETMOX::_net.combat_target_player_id].network_player_id, 22, &local_battle_results_2, 1742);
            }
            NETMOX::_net.sync_state = 14;
            NETMOX::Send_Battle_Results_(NETMOX::_net.combat_star_id, MOX::_PLAYER_NUM, NETMOX::_net.combat_target_player_id, NETMOX::_net.combat_defender_id, NETMOX::_net.combat_planet_id, (int16_t)local_battle_results_2.combat_result);
            break;

        case 25:
            COMBFIND::Strategic_Results_Popup_(NETMOX::_net.combat_results_data);
            COMBFIND::Bomb_Results_Popups_(NETMOX::_net.combat_results_data);
            MULTPLAY::Load_Net_Next_Turn_Screen_(0);
            NETMOX::_net.sync_state = 14;
            NETMOX::Mox_Send_Message_(NETMOX::_net.local_net_id, 23, nullptr, 0);
            break;

        case 28:
            result = MAINPUPS::Dragon_New_Star_Popup_(MOX::_PLAYER_NUM, NETMOX::_net.combat_star_id);
            new_star_target = (int32_t)result;
            MULTPLAY::Load_Net_Next_Turn_Screen_(0);
            if (new_star_target == -1) {
                NETMOX::_net.sync_state = 29;
                NETMOX::Mox_Send_Message_(NETMOX::_net.local_net_id, 36 /* 0x24 */, nullptr, 0);
            } else {
                NETMOX::_net.sync_state = 14;
                NETMOX::Mox_Send_Message_(NETMOX::_net.local_net_id, 37 /* 0x25 */, &new_star_target, 4);
            }
            break;

        case 34:
            while (NETMOX::_net.received_chunk_count < NETMOX::_net.total_chunk_count) {
                RUSS::Mox_Update_();
            }
            NETMOX::Decode_Battle_Results_(NETMOX::_net.network_data_ptr, NETMOX::_net.total_bytes_received);
            NETMOX::_net.network_data_ptr = nullptr;
            allocate::Pop_Block_(MOX::_screen_seg);
            NETMOX::_net.total_chunk_count = 0;
            NETMOX::_net.sync_state = 14;
            break;

        default:
            break;
        }
    }

    NETMOX::_net.combat_results_data = nullptr;
    COMBFIND::_attack_confirmed = nullptr;
    RUSS::Mox_Update_();
}

void __cdecl Do_Dragon_Bribe_(int player_idx, int star_idx, int16_t dest_idx) {
    int16_t ship_indices[2];
    s_ship_move_info move_info;

    ship_indices[0] = -1;
    ship_indices[1] = -1;

    for (int i = 0; i < (int)MOX::_NUM_SHIPS; ++i) {
        // Owner 12 is the Dragon/Guardian
        if (MOX::_ship[i].owner == 12 && MOX::_ship[i].location == (int16_t)star_idx) {
            ship_indices[0] = (int16_t)i;
        }
    }

    SHIPMOVE::Ships_Try_To_Move_To_(ship_indices, dest_idx, &move_info);
    SHIPMOVE::Make_Ships_Move_To_(ship_indices, dest_idx, &move_info);

    MOX::_player[player_idx].bc -= 500;

    (*_star_has_nonplayer)[star_idx] &= 0xEF;
}

int16_t __cdecl Closest_Other_Colony_Star_(int16_t player_idx, int16_t star_idx) {
    uint32_t min_dist_sq = 0xFFFFFFFF;
    int16_t closest_star_idx = -1;

    s_star_data* src_star = &MOX::_star[star_idx];
    int32_t src_x = src_star->x;
    int32_t src_y = src_star->y;

    for (int i = 0; i < MOX::_NUM_COLONIES; ++i) {
        s_colony* colony = &MOX::_colony[i];

        if (colony->owner == player_idx) {
            int8_t target_star_idx = MOX::_planet[colony->planet].star_index;

            if (target_star_idx != star_idx && target_star_idx != closest_star_idx) {
                if (!struct_::Test_Bit_Field_(src_star->black_hole_blocks, target_star_idx)) {
                    s_star_data* target_star = &MOX::_star[target_star_idx];

                    int32_t dx = (int32_t)target_star->x - src_x;
                    int32_t dy = (int32_t)target_star->y - src_y;
                    uint32_t dist_sq = (uint32_t)dx * (uint32_t)dx + (uint32_t)dy * (uint32_t)dy;

                    if (dist_sq < min_dist_sq) {
                        min_dist_sq = dist_sq;
                        closest_star_idx = target_star_idx;
                    }
                }
            }
        }
    }

    return closest_star_idx;
}

/**
 * @brief Selects a random attacker from the pool of players/entities that need to attack at a specific star.
 * 
 * The function checks bits 8-14 of the flags first (special attackers/factions). 
 * If any of those are set, it picks one randomly and ignores player bits (0-7).
 * Otherwise, it picks randomly from players 0 to _NUM_PLAYERS-1 whose bits are set.
 * 
 * @param star_idx The index of the star system.
 * @return The index of the selected attacker, or -1 if none found.
 */
void __cdecl Collect_Targets_For_Combat_(int16_t star_idx, int16_t player_idx) {
    _target.num_colony_targets = 0;

    if (star_idx < MOX::_NUM_STARS) {
        for (int i = 0; i < 5; ++i) {
            int16_t planet_idx = MOX::_star[star_idx].planet_index[i];
            if (planet_idx >= 0) {
                int16_t colony_idx = MOX::_planet[planet_idx].colony_index;
                if (colony_idx >= 0) {
                    if (Player_May_Attack_Player_(player_idx, MOX::_colony[colony_idx].owner)) {
                        _target.colony_targets[_target.num_colony_targets] = colony_idx;
                        _target.num_colony_targets++;
                    }
                }
            }
        }
    }

    _target.num_player_targets = 0;
    for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
        if (player_idx != i) {
            s_combfind_star_has_player* status = &(*_star_has_player)[star_idx][i];
            if (status->has_player && status->colony_count == 0) {
                if (Player_May_Attack_Player_(player_idx, i)) {
                    _target.player_targets[_target.num_player_targets] = i;
                    _target.num_player_targets++;
                }
            }
        }
    }

    uint8_t mask = (*_star_has_nonplayer)[star_idx];
    if (mask != 0) {
        for (int i = 0; i < 7; ++i) {
            // Indices 8-14 represent non-player factions/monsters
            if (player_idx != (i + 8)) {
                if ((mask >> i) & 1) {
                    _target.player_targets[_target.num_player_targets] = (int16_t)i + 8;
                    _target.num_player_targets++;
                }
            }
        }
    }
}

int16_t __cdecl Pick_Attacker_At_Star_(int16_t star_idx) {
    uint16_t needs_attack = (uint16_t)(*_player_needs_to_attack)[star_idx];
    int32_t attacker_idx = -1;
    int32_t count = 0;

    if (needs_attack == 0) {
        return -1;
    }

    // Check for special entities (bits 8-14)
    if (needs_attack >= 0x100 /* bit 8 set */) {
        for (int32_t i = 8; i < 15; ++i) {
            if ((needs_attack >> i) & 1) {
                count++;
                // Reservoir sampling for equal probability distribution
                if (random::Random_(count) == 1) {
                    attacker_idx = i;
                }
            }
        }
        return static_cast<int16_t>(attacker_idx);
    } else {
        // Check for players (bits 0-7)
        for (int32_t i = 0; i < static_cast<int32_t>(MOX::_NUM_PLAYERS); ++i) {
            if ((needs_attack >> i) & 1) {
                count++;
                if (random::Random_(count) == 1) {
                    attacker_idx = i;
                }
            }
        }
        return static_cast<int16_t>(attacker_idx);
    }
}

void __cdecl Pick_Star_And_Attacker_(int16_t* star_idx_ptr, int16_t* attacker_idx_ptr) {
    int16_t star_list_idx = _n_battle_stars - 1;
    *star_idx_ptr = -1;

    while (star_list_idx >= 0) {
        RUSS::Mox_Update_();

        // Get star index from the pending battle stars list
        uint8_t star_idx = (uint8_t)_battle_stars[star_list_idx];
        *star_idx_ptr = (int16_t)star_idx;

        if ((MOX::_game_type == 2 || MOX::_game_type == 3) && COMBFIND::Some_Star_Player_Is_Busy_(*star_idx_ptr)) {
            // Skip star if player logic is currently busy (e.g. multiplayer/hotseat)
            star_list_idx--;
            *star_idx_ptr = -1;
        } else {
            do {
                *attacker_idx_ptr = COMBFIND::Pick_Attacker_At_Star_(*star_idx_ptr);

                if (*attacker_idx_ptr == -1) {
                    // No valid attacker found at this star, remove the star from the list by swapping with last
                    int16_t current_list_idx = star_list_idx;
                    *star_idx_ptr = -1;
                    _n_battle_stars--;
                    star_list_idx--;
                    _battle_stars[current_list_idx] = _battle_stars[_n_battle_stars];
                } else {
                    // Mark this player as processed for this star in the current pick
                    (*_player_needs_to_attack)[*star_idx_ptr] &= ~(1 << *attacker_idx_ptr);

                    COMBFIND::Collect_Targets_For_Combat_(*star_idx_ptr, *attacker_idx_ptr);

                    // If no valid targets (player or colony) were found, try to find another attacker at the same star
                    if (_target.num_player_targets == 0 && _target.num_colony_targets == 0) {
                        *attacker_idx_ptr = -1;
                    }
                }
            } while (*attacker_idx_ptr == -1 && *star_idx_ptr != -1);
        }

        // If a valid star and attacker pair was found, exit
        if (*star_idx_ptr != -1) {
            break;
        }
    }
}

int16_t __cdecl Find_Planet_At_Star_Satisfying_(s_star_data* star, bool (__cdecl* predicate)(s_planet_data*)) {
    for (int16_t i = 0; i < 5; ++i) {
        int16_t planet_idx = star->planet_index[i];

        if (planet_idx != -1) {
            // Accessing the planet data via the global planet array
            if (predicate(&MOX::_planet[planet_idx])) {
                return planet_idx;
            }
        }
    }

    return -1;
}

bool __cdecl Planet_Has_Splinter_Colony_(s_planet_data* planet) {
    return planet->planet_special == 7 /* PLANET_SPECIAL_SPLINTER_COLONY */;
}

/**
 * @brief Translates a planet index to its corresponding colony index.
 * 
 * @param planet_idx The index of the planet in the global planet array.
 * @return The colony index associated with the planet, or -1 if the planet index is invalid or no colony exists.
 */
int16_t __cdecl Target_Planet_To_Colony_(int32_t planet_idx) {
    if (planet_idx == -1) {
        return -1;
    }

    // Each s_planet_data structure is 17 (0x11) bytes.
    // The assembly calculates the offset as (planet_idx * 16) + planet_idx.
    return MOX::_planet[planet_idx].colony_index;
}

s_colony* __cdecl Free_Colony_Slot_() {
    uint16_t i = 0;
    s_colony* col = MOX::_colony;

    if (i < MOX::_NUM_COLONIES) {
        do {
            // Check if the colony slot is empty
            if (col->planet == -1) {
                break;
            }
            i++;
            col++;
        } while (i < MOX::_NUM_COLONIES);

        // Found a free slot within the existing array bounds
        if (i < MOX::_NUM_COLONIES) {
            return col;
        }
    }

    // No free slot found; expand the colony count and return the next slot
    MOX::_NUM_COLONIES = i + 1;
    return col;
}

void __cdecl Do_System_Discoveries_At_Star_(s_star_data *star)
{
    int star_idx;
    int eligible_player_count;
    uint8_t player_present[8];
    int8_t player_idx;
    s_colony *colony;
    int16_t planet_idx;
    int16_t limit;
    int n_techs;
    int total_valid_techs;
    int picked_tech_id;
    s_MSG msg;

    star_idx = (int)(star - MOX::_star);
    Reset_One_Shot_System_Special_(star);

    if (star->system_special != 0 && (*_star_has_nonplayer)[star_idx] == 0) {
        eligible_player_count = 0;
        memset(player_present, 0, 8);

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if ((*_star_has_player)[star_idx][i].has_player != 0) {
                player_present[i] = 1;
                eligible_player_count++;
            }
        }

        if (eligible_player_count > 0) {
            player_idx = (int8_t)RUSS::Get_Weighted_Choice_Char_(player_present, MOX::_NUM_PLAYERS);

            switch (star->system_special) {
            case 2:
                MOX::_player[player_idx].bc += 50;
                star->system_special = player_idx + 50;
                break;

            case 3:
                MOX::_player[player_idx].bc += 100;
                star->system_special = player_idx + 60;
                break;

            case 7:
                star->system_special = player_idx + 70;
                (*_star_has_player)[star_idx][player_idx].colony_count = 1;
                colony = Free_Colony_Slot_();
                planet_idx = Find_Planet_At_Star_Satisfying_(star, Planet_Has_Splinter_Colony_);
                INITGAME::Init_Colony_((int)(colony - MOX::_colony), planet_idx, player_idx);
                
                limit = COLCALC::Colony_Race_Pop_Limit_(colony, player_idx);
                colony->n_pops = (int8_t)limit;
                if (colony->n_pops > 3) {
                    colony->n_pops = 3;
                }

                memset(colony->buildings, 0, 49);
                memset(colony->pop, 0, 168 /* 42 * 4 */);

                for (int i = 0; i < colony->n_pops; ++i) {
                    uint32_t pop_val = colony->pop[i];
                    
                    // Set owner bits (0-3) and source planet bits (4-6)
                    pop_val = (pop_val & ~0xF) | (player_idx & 0xF);
                    pop_val = (pop_val & ~0x70) | ((planet_idx << 4) & 0x70);

                    if (MOX::_planet[planet_idx].food_per_farmer == 0 || MOX::_player[player_idx].traits[18] != 0) {
                        int r = random::Random_(2);
                        pop_val = (pop_val & ~0x180) | ((r << 7) & 0x180);
                    } else if (MOX::_player[player_idx].traits[17] == 0) {
                        pop_val &= ~0x180;
                    } else if (i == 0) {
                        pop_val = (pop_val & ~0x100) | 0x80;
                    } else {
                        pop_val &= ~0x180;
                    }

                    pop_val |= 0x200; // Population unit exists flag
                    colony->pop[i] = pop_val;
                }

                COLCALC::Compute_Star_Colony_Stuff_(star);
                COLCALC::Colony_Calculation_((int16_t)(colony - MOX::_colony));
                colony->just_produced = -39 /* 0xFFD9 */;
                break;

            case 8:
                star->system_special = player_idx + 80;
                break;

            case 10:
                if (star->artifacts_gave_app == 0) {
                    star->system_special = player_idx + 90;
                    star->artifacts_gave_app = 1;
                    
                    // n_techs = (Random(4) / 4) + 1; Result is always 1
                    n_techs = (random::Random_(4) >> 2) + 1;
                    picked_tech_id = 0;

                    for (int i = n_techs; i != 0; --i) {
                        total_valid_techs = 0;
                        for (int tech_id = 1; tech_id < 204; ++tech_id) {
                            if (MOX::_player[player_idx].tech_fields[TECHDATA::_technology_applications[tech_id].tech_field_id] == 2 &&
                                COLCALC::Player_May_Get_Tech_App_(&MOX::_player[player_idx], tech_id)) {
                                
                                total_valid_techs++;
                                if (random::Random_(total_valid_techs) == 1) {
                                    picked_tech_id = tech_id;
                                }
                            }
                        }
                        if (total_valid_techs != 0) {
                            COLCALC::Player_Gets_Tech_App_(&MOX::_player[player_idx], picked_tech_id);
                        }
                    }

                    if (picked_tech_id != 0) {
                        memset(&msg, 0, sizeof(s_MSG));
                        msg.msg_type = 20 /* MSG_TYPE_ARTIFACTS */;
                        msg.priority = 1; // Default priority
                        msg.u.artifacts.star_idx = (uint8_t)star_idx;
                        msg.u.artifacts.tech1 = (int8_t)picked_tech_id;
                        msg.u.artifacts.tech2 = 0;
                        MSG_::Add_Msg_(msg, player_idx);
                    }
                }
                break;

            default:
                break;
            }
        }
    }
}

void __cdecl Reset_One_Shot_System_Special_(s_star_data* star) {
    // System specials with value >= 50 (ASCII '2') are treated as one-shot/temporary events.
    if (star->system_special >= 50) {
        for (int i = 0; i < 5; ++i) {
            int16_t planet_idx = star->planet_index[i];
            if (planet_idx != -1) {
                // Reset the special status on each planet belonging to this system
                MOX::_planet[planet_idx].planet_special = 0;
            }
        }
        // Clear the system special flag
        star->system_special = 0;
    }
}

/**
 * @address 004bd640
 */
void __cdecl Get_Local_Human_Target_(int16_t star_idx, int16_t player_idx, int16_t* target_player_idx_out, int16_t* target_colony_idx_out) {
    *target_colony_idx_out = -1;
    *target_player_idx_out = -1;

    if (star_idx == MOX::_NUM_STARS) {
        *target_player_idx_out = 8;
        return;
    }

    if (COMBFIND::_target.num_colony_targets > 0 || COMBFIND::_target.num_player_targets > 0) {
        video::Set_Page_Off_();
        MAINSCR::Draw_Mini_Main_Screen_();
        video::Copy_Off_To_Back_();
        RUSS::Mox_Sync_Update_();
        MAINSCR::Draw_Mini_Main_Screen_();

        REPORT::_something_interesting_happened[player_idx] = 1;
        MOX::_star[star_idx].visited |= (1 << player_idx);
        MOX::_PLAYER_NUM = player_idx;
        MAINSCR::Update_Ship_Icons_In_Between_Turns_();

        int16_t target_id;
        while (true) {
            target_id = -1;
            *target_colony_idx_out = -1;
            *target_player_idx_out = -1;

            int16_t selection = MAINPUPS::Defense_Colony_Selection_Popup_(star_idx, player_idx);

            if (selection != -1) {
                COMBFIND::_messed_up_fleet_palette = 1;
                if (selection < 15) {
                    target_id = selection;
                    *target_player_idx_out = selection;
                } else {
                    int16_t colony_idx = selection - 15;
                    *target_colony_idx_out = colony_idx;
                    target_id = (int16_t)MOX::_colony[colony_idx].owner;
                }
            }

            if (target_id == -1) {
                return;
            }

            if (COMBFIND::Human_Confirms_Attack_(player_idx, target_id)) {
                break;
            }
        }
    }
}

/**
 * @address 004bd580
 */
void __cdecl Get_AI_Target_(int16_t star_idx, int16_t player_idx, int16_t* target_player, int16_t* target_colony, int8_t* is_sneak_attack) {
    *is_sneak_attack = 0;
    *target_player = -1;
    *target_colony = -1;

    if (MOX::_NUM_STARS == star_idx) {
        *target_player = 8 /* STAR_ORION */;
        return;
    }

    if (COMBFIND::_target.num_colony_targets > 0 || COMBFIND::_target.num_player_targets > 0) {
        s_player* player = &MOX::_player[player_idx];
        int16_t sneak_attack_target = -1;

        if (player->sneak_attack_planet != -1) {
            if ((int16_t)MOX::_planet[player->sneak_attack_planet].star_index == star_idx) {
                sneak_attack_target = (int16_t)player->sneak_attack_player;
                player->sneak_attack_planet = -1;
                player->sneak_attack_player = -1;
                player->war_buildup_duration = 0;
            }
        }

        uint32_t bonus_info[4];
        uint32_t our_strength[15];
        uint32_t enemy_strength[15];

        COMBFIND::Get_Ship_Strength_Info_(enemy_strength, our_strength, bonus_info, star_idx, player_idx);

        uint32_t best_colony_score = 0;
        int16_t best_colony_idx = -1;

        COMBFIND::Get_Best_Colony_Target_(enemy_strength, our_strength, bonus_info, star_idx, player_idx, &best_colony_score, &best_colony_idx, sneak_attack_target);

        uint32_t best_player_score = 0;
        int16_t best_player_idx = -1;

        // Only consider player targets if there is no sneak attack target at this star or no valid colony target found
        if (sneak_attack_target == -1 || best_colony_idx == -1) {
            for (int16_t i = 0; i < COMBFIND::_target.num_player_targets; ++i) {
                int16_t enemy_idx = COMBFIND::_target.player_targets[i];
                uint32_t enemy_str = enemy_strength[enemy_idx];
                uint32_t our_str = our_strength[enemy_idx];

                if (enemy_str <= our_str) {
                    bool is_valid = false;
                    if (enemy_str < our_str) {
                        is_valid = true;
                    } else {
                        s_star_data* star = &MOX::_star[star_idx];
                        // Check if player has a colony here and if it is currently blockaded by the enemy
                        if (((star->has_colony >> player_idx) & 1) && ((star->blockaded_by[player_idx] >> enemy_idx) & 1)) {
                            is_valid = true;
                        } else if (enemy_idx == sneak_attack_target) {
                            is_valid = true;
                        } else {
                            switch (player->personality) {
                                case 0 /* PERSONALITY_AGGRESSIVE */:
                                case 1 /* PERSONALITY_EXPANSIONIST */:
                                    is_valid = true;
                                    break;
                                case 2 /* PERSONALITY_XENOPHOBIC */:
                                case 3 /* PERSONALITY_PACIFIST */:
                                    // Xenophobes and Pacifists have a 50% chance to engage in a tie-strength situation
                                    is_valid = (random::Random_(2) - 1) != 0;
                                    break;
                            }
                        }
                    }

                    if (is_valid) {
                        // Normalize strengths for scoring if the ratio is overwhelming
                        if (our_str > enemy_str * 3) {
                            our_str = 3;
                            enemy_str = 1;
                        } else if (our_str == 0) {
                            enemy_str = 1;
                        }

                        uint32_t score;
                        if (enemy_idx == sneak_attack_target) {
                            score = 0xFFFFFFFF;
                        } else {
                            score = (our_str << 4) / enemy_str;
                        }

                        if (best_player_score < score) {
                            best_player_score = score;
                            best_player_idx = enemy_idx;
                        }
                    }
                }
            }
        }

        if (best_colony_score < best_player_score) {
            *target_player = best_player_idx;
            if (best_player_idx == sneak_attack_target) {
                *is_sneak_attack = 1;
            }
        } else if (best_colony_score != 0) {
            *target_colony = best_colony_idx;
            if (MOX::_colony[best_colony_idx].owner == sneak_attack_target) {
                *is_sneak_attack = 1;
            }
        }
    }
}

void __cdecl Update_Player_Needs_To_Attack_(int star_idx, int player_a_idx, int player_b_idx) {
    // Handle first player
    if (player_a_idx < 8) {
        // Regular player: check presence via star-player status struct
        if ((*_star_has_player)[star_idx][player_a_idx].has_player == 0) {
            (*_player_needs_to_attack)[star_idx] &= ~(1 << (player_a_idx & 0x1f));
        }
    } else {
        // Non-player/Monster: check presence via bitmask
        if ((((*_star_has_nonplayer)[star_idx] >> (player_a_idx & 0x1f)) & 1) == 0) {
            (*_player_needs_to_attack)[star_idx] &= ~(1 << (player_a_idx & 0x1f));
        }
    }

    // Handle second player
    if (player_b_idx < 8) {
        // Regular player: check presence via star-player status struct
        if ((*_star_has_player)[star_idx][player_b_idx].has_player == 0) {
            (*_player_needs_to_attack)[star_idx] &= ~(1 << (player_b_idx & 0x1f));
        }
    } else {
        // Non-player/Monster: check presence via bitmask
        if ((((*_star_has_nonplayer)[star_idx] >> (player_b_idx & 0x1f)) & 1) == 0) {
            (*_player_needs_to_attack)[star_idx] &= ~(1 << (player_b_idx & 0x1f));
        }
    }
}

void __cdecl Get_NPC_Target_(int16_t star_idx, int16_t event_type, int16_t *player_target_ptr, int16_t *colony_target_ptr) {
    int lucky_count;
    int non_lucky_count;
    int lucky_target;
    int non_lucky_target;
    int i;

    *colony_target_ptr = -1;
    *player_target_ptr = -1;

    if (event_type == 13 /* EVENT_TYPE_13 */) {
        return;
    }

    if (MOX::_NUM_STARS == star_idx) {
        *player_target_ptr = 8;
        return;
    }

    if (COMBFIND::_target.num_colony_targets > 0) {
        lucky_count = 0;
        non_lucky_count = 0;
        lucky_target = -1;
        non_lucky_target = -1;

        for (i = 0; i < (int)COMBFIND::_target.num_colony_targets; ++i) {
            int16_t colony_idx = COMBFIND::_target.colony_targets[i];
            int8_t owner = MOX::_colony[colony_idx].owner;

            if (MOX::_player[owner].traits[26 /* TRAIT_LUCKY */] != 0) {
                lucky_count++;
                if (random::Random_(lucky_count) == 1) {
                    lucky_target = colony_idx;
                }
            }
            else {
                non_lucky_count++;
                if (random::Random_(non_lucky_count) == 1) {
                    non_lucky_target = colony_idx;
                }
            }
        }

        if (non_lucky_target != -1) {
            *colony_target_ptr = (int16_t)non_lucky_target;
        }
        else {
            *colony_target_ptr = (int16_t)lucky_target;
        }
        return;
    }

    if (COMBFIND::_target.num_player_targets > 0) {
        lucky_count = 0;
        non_lucky_count = 0;
        lucky_target = -1;
        non_lucky_target = -1;

        for (i = 0; i < (int)COMBFIND::_target.num_player_targets; ++i) {
            int16_t player_idx = COMBFIND::_target.player_targets[i];

            // If player is valid index and not lucky, prioritize them
            if (player_idx < 8 && MOX::_player[player_idx].traits[26 /* TRAIT_LUCKY */] == 0) {
                non_lucky_count++;
                if (random::Random_(non_lucky_count) == 1) {
                    non_lucky_target = player_idx;
                }
            }
            else {
                lucky_count++;
                if (random::Random_(lucky_count) == 1) {
                    lucky_target = player_idx;
                }
            }
        }

        if (non_lucky_target != -1) {
            *player_target_ptr = (int16_t)non_lucky_target;
        }
        else {
            *player_target_ptr = (int16_t)lucky_target;
        }
    }
}

/**
 * @address 004bc0d0
 */
char* __cdecl Load_Combfind_Strings_(char* strings_block) {
    char* current = strings_block;

    _combfind_strings_base = current;
    current += strlen(current) + 1;

    _you_have_following_treaties_msg = current;
    current += strlen(current) + 1;

    _trade_treaty_msg = current;
    current += strlen(current) + 1;

    _research_treaty_msg = current;
    current += strlen(current) + 1;

    _receiving_tribute_msg = current;
    current += strlen(current) + 1;

    _really_risk_war_msg = current;
    current += strlen(current) + 1;

    _outpost_destroyed_msg = current;
    current += strlen(current) + 1;

    _colony_seized_by_mind_control_msg = current;
    current += strlen(current) + 1;

    _s_attacks_you_at_s_msg = current;
    current += strlen(current) + 1;

    _combat_is_reported_between_s_and_s_msg = current;
    current += strlen(current) + 1;

    return current;
}

/**
 * @address 004bd750
 */
int8_t __cdecl Human_Confirms_Attack_(int player_idx, int target_idx) {
    char message_buffer[400];
    int8_t confirmed = 1;

    if (target_idx < 8 && (*_attack_confirmed)[player_idx][target_idx] == 0) {
        int treaty_count = 0;
        int offset = sprintf(message_buffer, s_format_string, _you_have_following_treaties_msg);
        char* dest_ptr = message_buffer + offset;

        // Check standard treaties (e.g., Non-Aggression, Alliance)
        int8_t treaty_type = MOX::_player[player_idx].treaty[target_idx];
        if (treaty_type > 0 && treaty_type < 4) {
            treaty_count = 1;
            offset = sprintf(dest_ptr, s_format_string, MOX::_treaty_labels[treaty_type]);
            dest_ptr += offset;
        }

        // Check trade treaty
        if (MOX::_player[player_idx].trade_treaty[target_idx] != 0) {
            treaty_count++;
            offset = sprintf(dest_ptr, s_format_string, _trade_treaty_msg);
            dest_ptr += offset;
        }

        // Check research treaty
        if (MOX::_player[player_idx].research_treaty[target_idx] != 0) {
            treaty_count++;
            offset = sprintf(dest_ptr, s_format_string, _research_treaty_msg);
            dest_ptr += offset;
        }

        // Check if player is receiving tribute from the target
        if (MOX::_player[target_idx].tribute_treaty[player_idx] != 0) {
            treaty_count++;
            offset = sprintf(dest_ptr, s_format_string, _receiving_tribute_msg);
            dest_ptr += offset;
        }

        if (treaty_count != 0) {
            sprintf(dest_ptr, _really_risk_war_msg);
            confirmed = (int8_t)GENDRAW::Confirmation_Box_(message_buffer);

            if (confirmed != 0) {
                (*_attack_confirmed)[player_idx][target_idx] = 1;
            }
        }
    }

    return confirmed;
}

/**
 * Determines if a specific player is allowed to initiate an attack on another player 
 * based on their current relationship status, treaties, and personality.
 */
void __cdecl Copy_Target_Info_(s_combfind_target* src) {
    _target = *src;
}

int8_t __cdecl Player_May_Attack_Player_(int attacker_idx, int target_idx) {
    if (target_idx == attacker_idx) {
        return 0;
    }

    // Indices >= 8 usually represent Antarans, Guardians, or space monsters
    if (attacker_idx > 7) {
        return 1;
    }

    s_player* attacker = &MOX::_player[attacker_idx];

    // 0x64 ('d') represents human player
    if (attacker->objectives == 100) {
        return 1;
    }

    int8_t treaty_status = attacker->treaty[target_idx];
    if (treaty_status != 0) {
        // Specific treaty values allow combat (e.g., War or specific hostilities)
        if (treaty_status >= 4 && treaty_status <= 6) {
            return 1;
        }
        return 0;
    }

    // If no formal treaty exists, check for non-aggression or active trade/research ties
    if (attacker->stop_blockading_duration[target_idx] == 0 &&
        attacker->trade_treaty[target_idx] == 0 &&
        attacker->research_treaty[target_idx] == 0) 
    {
        // Personality 0 is likely Ruthless/Aggressive
        if (attacker->personality == 0) {
            return 1;
        }

        // Personality 3 is likely erratic or opportunistic, having a small chance to attack
        if (attacker->personality == 3) {
            if (random::Random_(10) == 1) {
                return 1;
            }
        }
    }

    return 0;
}

int16_t __cdecl Target_Colony_To_Planet_(int32_t colony_idx) {
    if (colony_idx == -1) {
        return -1;
    }

    return MOX::_colony[colony_idx].planet;
}

int8_t __cdecl Some_Star_Player_Is_Busy_(int16_t star_idx) {
    for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
        if (MOX::_player[i].objectives == 100) {
            s_combfind_star_has_player& star_info = (*COMBFIND::_star_has_player)[star_idx][i];

            if (star_info.colony_count != 0 || star_info.has_player != 0) {
                if (NETMOX::_net.player_net_info[i].sync_phase_ready_flag != 0xFF) {
                    return 1;
                }

                if (MOX::_PLAYER_NUM == i) {
                    if (NETMOX::_net.sync_state >= 33 && NETMOX::_net.sync_state <= 34) {
                        return 1;
                    }

                    if (NETMOX::_net.network_data_ptr != nullptr) {
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

void __cdecl Decode_Star_Player_Presence_(uint8_t **buffer, int16_t star_idx) {
    s_combfind_star_has_player *dest;

    if (star_idx == -1) {
        dest = _current_star_has_player;
    } else {
        dest = (*_star_has_player)[star_idx];
    }

    memcpy(dest, *buffer, 88);
    *buffer += 88;

    if (star_idx == -1) {
        *_current_star_has_nonplayer = **buffer;
    } else {
        (*_star_has_nonplayer)[star_idx] = **buffer;
    }

    *buffer += 1;
}

void __cdecl Get_Combat_Ships_(int16_t* ships, int16_t star_idx, int16_t plr1_idx, int16_t* plr1_offset, int16_t* plr1_count, int16_t plr2_idx, int16_t* plr2_offset, int16_t* plr2_count) {
    *plr1_offset = 0;
    *plr1_count = 0;
    *plr2_offset = 500;
    *plr2_count = 0;

    for (int i = 0; i < (int)MOX::_NUM_SHIPS; ++i) {
        s_ship_data* ship = &MOX::_ship[i];

        if (ship->location == star_idx && ship->status == 0) {
            if (ship->owner == (int8_t)plr1_idx) {
                ships[*plr1_count] = (int16_t)i;
                *plr1_count = *plr1_count + 1;
            } else if (ship->owner == (int8_t)plr2_idx) {
                *plr2_offset = *plr2_offset - 1;
                ships[*plr2_offset] = (int16_t)i;
                *plr2_count = *plr2_count + 1;
            }
        }
    }
}

short __cdecl Do_1_Combat_(short star_idx, short plr2_idx, short plr1_idx, short planet_idx, s_battle_results *result, int8_t is_sneak_attack) {
    short colony_idx;
    int16_t plr1_ship_offset;
    int16_t plr2_ship_offset;
    int16_t plr1_ship_count;
    int16_t plr2_ship_count;
    char msg_buf[200];
    int16_t ships[500];
    s_battle_results local_results;

    colony_idx = Target_Planet_To_Colony_(planet_idx);

    if (plr1_idx == -1) {
        colony_idx = Target_Planet_To_Colony_(planet_idx);
        plr1_idx = (short)MOX::_colony[colony_idx].owner;
    }

    if (plr2_idx < 8 && plr1_idx < 8) {
        MOX::_player[plr2_idx].time_since_last_attack[plr1_idx] = 0;
    }

    if (result == nullptr) {
        result = &local_results;
        memset(result, 0, 1742);
    }

    if (MOX::_game_type < 2 || MOX::_game_type > 3) {
        _current_star_has_player = &(*_star_has_player)[star_idx][0];
        _current_star_has_nonplayer = &(*_star_has_nonplayer)[star_idx];
    } else {
        if (NETMOX::_net.local_net_id == NETMOX::_net.net_host_id) {
            _current_star_has_player = &(*_star_has_player)[star_idx][0];
            _current_star_has_nonplayer = &(*_star_has_nonplayer)[star_idx];
        }
        if (MOX::_PLAYER_NUM == plr1_idx && MOX::_settings.strategic_combat_flag == 0) {
            char* star_or_planet_name;
            if (planet_idx == -1) {
                star_or_planet_name = RUSS::Star_Name_(star_idx);
            } else {
                star_or_planet_name = HACCESS::Get_Planet_Name_(planet_idx);
            }
            char* attacker_name = COLCALC::Player_Name_(plr2_idx);
            sprintf(msg_buf, _s_attacks_you_at_s_msg, attacker_name, star_or_planet_name);
            GENDRAW::Message_Box_(msg_buf);
        }
    }

    Get_Combat_Ships_(ships, star_idx, plr2_idx, &plr2_ship_offset, &plr2_ship_count, plr1_idx, &plr1_ship_offset, &plr1_ship_count);

    short combat_status = Russ_Combat_(plr2_idx, &ships[plr2_ship_offset], plr2_ship_count, plr1_idx, &ships[plr1_ship_offset], plr1_ship_count, star_idx, colony_idx, is_sneak_attack, result);
    result->combat_result = (int8_t)combat_status;

    colony_idx = Target_Planet_To_Colony_(planet_idx);

    if (MOX::_game_type > 1 && MOX::_game_type < 4) {
        if (MOX::_PLAYER_NUM == plr2_idx || MOX::_PLAYER_NUM == plr1_idx) {
            MULTPLAY::Load_Net_Next_Turn_Screen_(1);
            MOX::_FULL_DRAW_SCREEN_FLAG = 1;
            MULTPLAY::Chat_Box_Input_Loop_();
        }
    }

    if (result->combat_result >= 0) {
        if (plr1_idx < 8) {
            if (_current_star_has_player[plr1_idx].ship_sizes[0] == 0 && colony_idx == -1) {
                Kill_Noncombat_Ships_(&ships[plr1_ship_offset], plr1_ship_count);
            } else {
                Determine_Retreat_Ships_(star_idx, plr1_idx, colony_idx);
            }
        } else {
            *_current_star_has_nonplayer &= ~(1 << (plr1_idx - 8));
        }
    }

    if (result->combat_result <= 0) {
        if (plr2_idx < 8) {
            Determine_Retreat_Ships_(star_idx, plr2_idx, -1);
        } else {
            *_current_star_has_nonplayer &= ~(1 << (plr2_idx - 8));
        }

        if (plr2_idx == 9) { // Guardian defeated
            MOX::_player[plr1_idx].captured_orion = 1;
            OFFICER::Give_Player_Loknar_Special_(plr1_idx, result->info.orion_techs, &result->info.rescued_hero_id);
            if (MOX::_player[plr1_idx].objectives == 100) {
                if (MOX::_game_type == 0 || MOX::_game_type == 1 || MOX::_settings.strategic_combat_flag == 0) {
                    ERIC::Play_Take_Orion_Flic_();
                    OFFICER::Rescued_Loknar_Popup_(plr1_idx, result->info.orion_techs, result->info.rescued_hero_id);
                }
            }
        }

        if (result->combat_result < 0) {
            Get_Combat_Ships_(ships, star_idx, plr2_idx, &plr2_ship_offset, &plr2_ship_count, plr1_idx, &plr1_ship_offset, &plr1_ship_count);
            if (plr1_ship_count == 0) {
                if (plr1_idx < 8) {
                    Reset_Combat_Ships_At_Current_Star_Has_Player_(plr1_idx);
                } else {
                    *_current_star_has_nonplayer &= ~(1 << (plr1_idx - 8));
                }
            }
        }
    } else {
        if (plr1_idx == 9) { // Guardian defeated by attacker
            MOX::_player[plr2_idx].captured_orion = 1;
            OFFICER::Give_Player_Loknar_Special_(plr2_idx, result->info.orion_techs, &result->info.rescued_hero_id);
            if (MOX::_player[plr2_idx].objectives == 100) {
                if (MOX::_game_type == 0 || MOX::_game_type == 1 || MOX::_settings.strategic_combat_flag == 0) {
                    ERIC::Play_Take_Orion_Flic_();
                    OFFICER::Rescued_Loknar_Popup_(plr2_idx, result->info.orion_techs, result->info.rescued_hero_id);
                }
            }
        }

        if (star_idx == MOX::_NUM_STARS) { // Antares victory
            MOX::_player[plr2_idx].captured_antares = 1;
            memset(_player_needs_to_attack, 0, 146);
        } else {
            Get_Combat_Ships_(ships, star_idx, plr2_idx, &plr2_ship_offset, &plr2_ship_count, plr1_idx, &plr1_ship_offset, &plr1_ship_count);
            if (plr2_ship_count == 0) {
                if (plr2_idx < 8) {
                    Reset_Combat_Ships_At_Current_Star_Has_Player_(plr2_idx);
                } else {
                    *_current_star_has_nonplayer &= ~(1 << (plr2_idx - 8));
                }
            }
            if (colony_idx != -1 && MOX::_colony[colony_idx].owner != -1) {
                Do_Attacker_Beat_Colony_Stuff_(&ships[plr2_ship_offset], (int16_t)plr2_ship_count, colony_idx, result);
            }
        }
    }

    return (short)result->combat_result;
}

int16_t __cdecl Russ_Combat_(int16_t attacker_idx, int16_t* attacker_ships, int16_t attacker_count, int16_t defender_idx, int16_t* defender_ships, int16_t defender_count, int16_t star_idx, int16_t colony_idx, int8_t is_sneak_attack, s_battle_results* results)
{
    bool not_multiplayer;
    bool plr1_is_human;
    bool plr2_is_human;
    uint8_t strategic_combat_flag;
    int16_t combat_result_code;

    if (MOX::_game_type == 2 || MOX::_game_type == 3) {
        not_multiplayer = false;
    } else {
        not_multiplayer = true;
    }

    strategic_combat_flag = MOX::_settings.strategic_combat_flag;

    if (attacker_idx < 8 && MOX::_player[attacker_idx].objectives == 100 /* HUMAN */) {
        plr1_is_human = true;
    } else {
        plr1_is_human = false;
    }

    if (defender_idx < 8 && MOX::_player[defender_idx].objectives == 100 /* HUMAN */) {
        plr2_is_human = true;
    } else {
        plr2_is_human = false;
    }

    // Early return if no actual combat possible and not at special star location
    if (((attacker_count == 0) || (defender_count == 0 && colony_idx == -1)) && (star_idx != MOX::_NUM_STARS)) {
        return 0;
    }

    if (attacker_idx < 8) {
        REPORT::_something_interesting_happened[attacker_idx] = 1;
    }
    if (defender_idx < 8) {
        REPORT::_something_interesting_happened[defender_idx] = 1;
    }

    // Display attacker alerts in single player mode
    if (not_multiplayer && plr2_is_human) {
        if (is_sneak_attack != 0) {
            DIP_SCRN::Show_Sneak_Attack_Message_(attacker_idx, defender_idx);
        }
        MAINPUPS::Player_Attacked_Popup_(attacker_idx, defender_idx, star_idx, colony_idx);
        COMBFIND::_messed_up_fleet_palette = 1;
    }

    // Force strategic combat if no human players are involved
    if (strategic_combat_flag == 0) {
        if (!plr1_is_human && !plr2_is_human) {
            strategic_combat_flag = 1;
        }
    }

    if (strategic_combat_flag != 0) {
        combat_result_code = COMBAT::Strategic_Combat_(attacker_ships, attacker_count, defender_ships, defender_count,
                                                       star_idx, colony_idx, results->info.summary, results->info.details);

        // Show result dialog if human is involved or it is a local single player game
        if (not_multiplayer || MOX::_PLAYER_NUM == attacker_idx) {
            if (plr2_is_human || plr1_is_human) {
                MAINSCR::Update_Ship_Icons_In_Between_Turns_();
                COMBAT::Message_Box_Titled_(results->info.summary, results->info.details);
                MAINSCR::Update_Ship_Icons_In_Between_Turns_();
                COMBFIND::_messed_up_fleet_palette = 1;
            }
        }
    } else {
        int32_t retreat_flag = 1;

        if (attacker_idx < 8 && defender_idx < 8) {
            if (plr1_is_human) {
                if (!plr2_is_human) {
                    retreat_flag = AI_Can_Retreat_(defender_idx, colony_idx);
                }
            } else {
                retreat_flag = AI_Can_Retreat_(attacker_idx, -1);
            }
        } else {
            retreat_flag = 0;
        }

        // Multiplayer synchronization for tactical combat
        if (plr1_is_human && plr2_is_human && (MOX::_game_type == 2 || MOX::_game_type == 3)) {
            int16_t remote_plr_idx = (MOX::_PLAYER_NUM == attacker_idx) ? defender_idx : attacker_idx;
            int16_t net_id = MOX::_player[remote_plr_idx].network_player_id;

            NETMOX::Mox_Send_Message_(net_id, 0x40 /* COMBAT_START */, nullptr, 0);

            while (NETMOX::_net.network_sync_complete_flag == 0) {
                RUSS::Mox_Update_();
            }

            combat_result_code = COMBINIT::Tactical_Combat_(attacker_ships, attacker_count, defender_ships, defender_count,
                                                            star_idx, colony_idx, retreat_flag);

            NETMOX::Mox_Send_Message_(net_id, 0x41 /* COMBAT_END */, nullptr, 0);

            while (NETMOX::_net.network_sync_complete_flag == 1) {
                RUSS::Mox_Update_();
            }

            NETMOX::_net.network_sync_complete_flag = 0;
        } else {
            combat_result_code = COMBINIT::Tactical_Combat_(attacker_ships, attacker_count, defender_ships, defender_count,
                                                            star_idx, colony_idx, retreat_flag);
        }
    }

    MAINSCR::Update_Ship_Icons_In_Between_Turns_();

    if (combat_result_code < 0) {
        return -1;
    }

    return (combat_result_code > 0);
}

void __cdecl Kill_Noncombat_Ships_(int16_t* ship_indices, int16_t count) {
    if (count <= 0) {
        return;
    }

    int8_t owner_idx = MOX::_ship[ship_indices[0]].owner;

    if (MOX::_player[owner_idx].objectives == 100 /* 'd' */ &&
        MOX::_game_type >= 0 && MOX::_game_type <= 1) {
        GENDRAW::Message_Box_(_noncombat_ships_destroyed_msg);
    }

    for (int16_t i = 0; i < count; ++i) {
        SHIPS::Kill_Ship_(ship_indices[i]);
    }

    _current_star_has_player[owner_idx].has_player = 0;
    MAINSCR::Update_Ship_Icons_In_Between_Turns_();
}

void __cdecl Determine_Retreat_Ships_(int16_t star_idx, int16_t player_idx, int16_t retreat_type) {
    int32_t p_idx = (int32_t)player_idx;
    bool should_retreat_or_die = false;

    if (retreat_type != -1 || COMBFIND::_current_star_has_player[p_idx].colony_count == 0) {
        if (COMBFIND::_current_star_has_player[p_idx].colony_count < 2) {
            should_retreat_or_die = true;
        }
    }

    if (should_retreat_or_die) {
        bool flux_blocks = false;
        if (player_idx < 8 && MOX::_player[p_idx].traits[29 /* TRAIT_TRANS_DIMENSIONAL */] == 0) {
            if (EVENTS::Event_Check_Hyperspace_Flux_() != 0) {
                flux_blocks = true;
            }
        }

        if (flux_blocks) {
            // Destruction logic due to Hyperspace Flux
            for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
                s_ship_data* ship = &MOX::_ship[i];
                if (ship->owner == player_idx && ship->location == star_idx && ship->status == 0) {
                    if (ship->officer_index != -1) {
                        if (MOX::_leaders[ship->officer_index].eta <= 0) {
                            OFFICER::Kill_Officer_(ship->officer_index);
                        } else {
                            OFFICER::Move_Officer_To_Pool_(ship->officer_index);
                        }
                    }
                    SHIPS::Kill_Ship_(i);
                }
            }
            COMBFIND::_current_star_has_player[p_idx].has_player = 0;
            return;
        } else {
            // Normal retreat logic
            for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
                s_ship_data* ship = &MOX::_ship[i];
                if (ship->owner == player_idx && ship->location == star_idx && ship->status == 0) {
                    ship->status = 9; // Status 9: retreating/travelling
                }
            }
            COMBFIND::_current_star_has_player[p_idx].has_player = 0;
            MAINSCR::Update_Ship_Icons_In_Between_Turns_();
            return;
        }
    }

    // Default case: verify if any active ships remain at the star
    bool ships_remaining = false;
    for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
        s_ship_data* ship = &MOX::_ship[i];
        if (ship->owner == player_idx && ship->location == star_idx && ship->status == 0) {
            ships_remaining = true;
            break;
        }
    }

    if (!ships_remaining) {
        COMBFIND::_current_star_has_player[p_idx].has_player = 0;
    }
}

void __cdecl Reset_Combat_Ships_At_Current_Star_Has_Player_(int16_t player_idx) {
    _current_star_has_player[player_idx].ship_sizes[0] = 0;
    _current_star_has_player[player_idx].has_player = 0;

    for (int i = 0; i < 5; ++i) {
        if (_current_star_has_player[player_idx].ship_sizes[i] != 0) {
            _current_star_has_player[player_idx].has_player = 1;
        }
    }
}

/**
 * Checks if the specified player owns any colony other than the one at excluded_colony_idx.
 * This is typically used to determine if an AI player can retreat (needs at least one other colony to retreat to).
 */
bool __cdecl AI_Can_Retreat_(int player_idx, int excluded_colony_idx) {
    for (int i = 0; i < (int)MOX::_NUM_COLONIES; ++i) {
        if (MOX::_colony[i].owner == (int8_t)player_idx && i != excluded_colony_idx) {
            return true;
        }
    }
    return false;
}

void __cdecl Get_Ship_Strength_Info_(uint32_t* enemy_strength, uint32_t* player_strength, uint32_t* shield_strength, int16_t star_idx, int16_t player_idx) {
    uint32_t ship_size_sum = 0;

    memset(enemy_strength, 0, 15 * sizeof(uint32_t));
    memset(player_strength, 0, 15 * sizeof(uint32_t));
    memset(shield_strength, 0, 4 * sizeof(uint32_t));

    int16_t num_ships = (int16_t)MOX::_NUM_SHIPS;
    s_ship_data* ship = MOX::_ship;

    for (int16_t i = 0; i < num_ships; ++i, ++ship) {
        if (ship->location == star_idx && ship->status == 0) {
            int16_t owner = (int16_t)ship->owner;
            
            if (player_idx == owner) {
                ship_size_sum += 1U << (ship->d.size & 0x1F);

                for (int16_t p = 0; p < MOX::_NUM_PLAYERS; ++p) {
                    if (player_idx != p) {
                        if ((*_star_has_player)[star_idx][p].has_player != 0 ||
                            (*_star_has_player)[star_idx][p].colony_count != 0) {
                            player_strength[p] += AIPOWER::Ship_Strength_Vs_Ships_(i, p);
                        }
                    }
                }

                for (int16_t np = 8; np < 15; ++np) {
                    if ((((*_star_has_nonplayer)[star_idx] >> (np - 8)) & 1) != 0 &&
                        (((*_player_needs_to_attack)[star_idx] >> np) & 1) == 0) {
                        player_strength[np] += AIPOWER::Ship_Strength_Vs_Ships_(i, np);
                    }
                }

                uint16_t shield[4];
                AIPOWER::Ship_Strength_Vs_Planetary_Shield_(i, shield);
                
                for (int j = 0; j < 4; ++j) {
                    shield_strength[j] += shield[j];
                }
            } else {
                enemy_strength[owner] += AIPOWER::Ship_Strength_Vs_Ships_(i, player_idx);
            }
        }
    }

    if (ship_size_sum > 150) {
        for (int i = 0; i < 15; ++i) {
            uint32_t val = player_strength[i];
            uint32_t new_val = (val / 150) * ship_size_sum;
            player_strength[i] = new_val;
            if (val > new_val) {
                player_strength[i] = val;
            }
        }
    }
}

void __cdecl Get_Best_Colony_Target_(
    uint32_t* enemy_fleet_space_str_arr,
    uint32_t* our_space_str_arr,
    uint32_t* our_ground_str_arr,
    int16_t star_idx,
    int16_t player_idx,
    uint32_t* best_score,
    int16_t* best_colony_idx,
    int32_t preferred_owner)
{
    *best_score = 0;
    *best_colony_idx = -1;

    uint32_t best_pref_score = 0;
    int32_t best_pref_colony = -1;

    if (_target.num_colony_targets > 0) {
        for (int16_t i = 0; i < _target.num_colony_targets; ++i) {
            int16_t colony_idx = _target.colony_targets[i];
            s_colony* colony = &MOX::_colony[colony_idx];

            int16_t shield_val = COLCALC::Colony_Planetary_Shield_(colony);
            int32_t owner = colony->owner;

            uint32_t our_space_str = our_space_str_arr[owner];
            uint32_t our_ground_str = our_ground_str_arr[shield_val];

            uint32_t enemy_space_str_vs = AIPOWER::Colony_Space_Strength_Vs_Player_(colony_idx, player_idx) & 0xFFFF;
            uint32_t enemy_space_str = enemy_fleet_space_str_arr[owner] + enemy_space_str_vs;

            uint32_t enemy_ground_str_vs = AIPOWER::Colony_Ground_Strength_Vs_Player_(colony_idx, player_idx) & 0xFFFF;
            uint32_t enemy_ground_str = enemy_ground_str_vs;

            bool player_is_omni = false;
            if (MOX::_player[player_idx].traits[25] != 0 && MOX::_player[owner].traits[25] == 0) {
                player_is_omni = true;
            }

            bool is_valid = false;
            if (our_space_str != 0 || our_ground_str != 0 ||
                (*_star_has_player)[star_idx][player_idx].has_transport != 0 ||
                player_is_omni ||
                colony->outpost_flag != 0)
            {
                if (our_space_str > enemy_space_str && our_ground_str > enemy_ground_str) {
                    bool condition2 = ((*_star_has_player)[star_idx][owner].colony_count < 2 ||
                                       our_ground_str != 0 ||
                                       (*_star_has_player)[star_idx][player_idx].has_transport != 0 ||
                                       player_is_omni);

                    if (condition2 || enemy_space_str_vs != 0) {
                        if (colony->n_pops != 1 ||
                            (colony->pop[0] & 0xF) != player_idx ||
                            (*_star_has_player)[star_idx][player_idx].ship_sizes[2] != 0 ||
                            MOX::_player[player_idx].personality == 1)
                        {
                            is_valid = true;
                        }
                    }
                }
            }

            if (is_valid) {
                uint32_t worth = static_cast<uint32_t>(AIMOVE::Enemy_Colony_Worth_To_Player_(colony_idx, player_idx));

                if (enemy_space_str * 3 < our_space_str) {
                    our_space_str = 3;
                    enemy_space_str = 1;
                } else if (our_space_str == 0) {
                    our_space_str = 1;
                    enemy_space_str = 1;
                }

                if (enemy_ground_str * 3 < our_ground_str) {
                    our_ground_str = 3;
                    enemy_ground_str = 1;
                } else if (our_ground_str == 0) {
                    our_ground_str = 1;
                    enemy_ground_str = 1;
                }

                uint32_t enemy_space_str_scaled = enemy_space_str;
                if (enemy_space_str_scaled > 0xFFFF) {
                    enemy_space_str_scaled >>= 8;
                    our_space_str >>= 8;
                }

                uint32_t enemy_ground_str_scaled = enemy_ground_str;
                if (enemy_ground_str_scaled > 0xFFFF) {
                    enemy_ground_str_scaled >>= 8;
                    our_ground_str >>= 8;
                }

                uint32_t score = ((worth * our_space_str) / enemy_space_str_scaled * our_ground_str) / enemy_ground_str_scaled;

                if (score > *best_score) {
                    *best_score = score;
                    *best_colony_idx = colony_idx;
                }

                if (owner == preferred_owner && score > best_pref_score) {
                    best_pref_score = score;
                    best_pref_colony = colony_idx;
                }
            }
        }
    }

    if (preferred_owner != -1 && best_pref_colony != -1) {
        *best_score = best_pref_score;
        *best_colony_idx = static_cast<int16_t>(best_pref_colony);
    }
}

void __cdecl Do_Attacker_Beat_Colony_Stuff_(int16_t* ship_list, int n_ships, uint32_t colony_idx, s_battle_results* results) {
    s_ship_data* ships = MOX::_ship;
    int16_t ship_idx = *ship_list;
    int8_t attacker_id = ships[ship_idx].owner;
    s_colony* colony = &MOX::_colony[colony_idx];
    int16_t planet_idx = colony->planet;
    int8_t star_idx = MOX::_planet[planet_idx].star_index;
    int8_t colony_owner = colony->owner;

    if (colony->outpost_flag != 0) {
        results->outpost_destroyed_flag = 1;
        BOMB::Destroy_Colony_(colony, 1);
        COMBFIND::_current_star_has_player[colony_owner].colony_count--;
    } else if (attacker_id < 8) {
        if (MOX::_player[attacker_id].objectives == 'd' /* ECON_INDUSTRY */) {
            bool is_active_player = false;
            if ((MOX::_game_type != 2 && MOX::_game_type != 3) || (MOX::_PLAYER_NUM == attacker_id)) {
                is_active_player = true;
            }

            if (is_active_player) {
                int32_t bombardment_res = Do_Bombardment_(attacker_id, colony_idx);
                bool has_stellar = Has_Stellar_Converter_(ship_list, n_ships);

                invasion::Get_Invasion_Info_(ship_list, n_ships, colony, &results->info.attacker_invasion, &results->info.defender_invasion);
                int seed = random::Get_Random_Seed_();
                int16_t selection = MAINPUPS::Colony_Selection_Popup_(&results->info.attacker_invasion, &results->info.defender_invasion, seed, (int16_t)colony_idx, bombardment_res, has_stellar);

                bool loop_done = false;
                do {
                    switch (selection) {
                        case 1: { // Stellar Converter
                            results->info.planet_destroyed_flag = 1;
                            BOMB::Destroy_Colony_(colony, 2);
                            ERIC::Play_Planetary_Destruction_Flic_();
                            COMBFIND::_current_star_has_player[colony_owner].colony_count--;
                            MOX::_planet[planet_idx].planet_type = 1 /* PLANET_TYPE_ASTEROIDS */;
                            if (MOX::_planet[planet_idx].planet_special != 0) {
                                MOX::_planet[planet_idx].planet_special = 0;
                                MOX::_star[star_idx].system_special = 0;
                            }
                            NETMOX::Russ_Change_Relations_(-10000, attacker_id, colony_owner, 10, star_idx, colony->n_pops);
                            loop_done = true;
                            break;
                        }
                        case 2: { // Bombing
                            int bomb_seed = random::Get_Random_Seed_();
                            memset(&results->bomb, 0, sizeof(s_combat_bomb_result));
                            results->bomb.attacker_idx = attacker_id;
                            results->bomb.colony_idx = (uint8_t)colony_idx;
                            COMBAT::Bombardment_(&results->bomb);

                            invasion::Get_Invasion_Info_(ship_list, n_ships, colony, &results->info.attacker_invasion, &results->info.defender_invasion);
                            selection = MAINPUPS::Colony_Bombing_Popup_(&results->bomb, &results->info.attacker_invasion, &results->info.defender_invasion, bomb_seed, (int16_t)colony_idx, bombardment_res, has_stellar);
                            NETMOX::Russ_Change_Relations_(-10000, attacker_id, colony_owner, 10, star_idx, results->bomb.population_killed);

                            if (results->bomb.colony_wiped_out != 0) {
                                BOMB::Destroy_Colony_(colony, 1);
                                COMBFIND::_current_star_has_player[colony_owner].colony_count--;
                                loop_done = true;
                            } else {
                                if (results->info.attacker_invasion.unit_counts[1] == 0 && !has_stellar && !ERIC::Player_Can_Mind_Control_Colony_(attacker_id, (int16_t)colony_idx)) {
                                    loop_done = true;
                                }
                            }
                            bombardment_res = -1;
                            break;
                        }
                        case 3: { // Invasion
                            invasion::Invade_(ship_list, n_ships, (int16_t)colony_idx, &results->info);
                            int combat_seed = random::Get_Random_Seed_();
                            ERIC::Force_Fade_To_Colony_Palette_(1);
                            COLGCBT::Colony_Combat_Screen_(&results->info.attacker_invasion, &results->info.defender_invasion, results->info.ground_defense_strength, (int16_t)colony_idx);
                            ERIC::Force_Fade_To_Colony_Palette_(0);
                            random::Set_Random_Seed_(combat_seed);

                            if (results->info.captured_tech_id != 0) {
                                SCIENCE::Show_Off_Captured_Tech_(results->info.captured_techs_data, results->info.captured_tech_id, results->info.captured_tech_planet_id);
                            }
                            NETMOX::Russ_Change_Relations_(-10000, attacker_id, colony_owner, 10, star_idx, 0);

                            if (colony->owner == attacker_id) {
                                COMBFIND::_current_star_has_player[attacker_id].colony_count++;
                                COMBFIND::_current_star_has_player[colony_owner].colony_count--;
                            }
                            COMBFIND::_messed_up_fleet_palette = 1;
                            loop_done = true;
                            break;
                        }
                        case 4: { // Surrender/Mind Control
                            invasion::Change_Colony_Ownership_(colony, attacker_id, &results->info);
                            results->info.is_mind_controlled = 1;
                            if (results->info.captured_tech_id != 0) {
                                SCIENCE::Show_Off_Captured_Tech_(results->info.captured_techs_data, results->info.captured_tech_id, colony->planet);
                            }
                            NETMOX::Russ_Change_Relations_(-10000, attacker_id, colony_owner, 10, star_idx, colony->n_pops);
                            COMBFIND::_current_star_has_player[attacker_id].colony_count++;
                            COMBFIND::_current_star_has_player[colony_owner].colony_count--;
                            loop_done = true;
                            break;
                        }
                        case -1:
                        default: {
                            loop_done = true;
                            break;
                        }
                    }
                } while (!loop_done);

                if (results->bomb.bio_pop_killed != 0) {
                    COMBINIT::Apply_Bio_Weapon_Penalty_(attacker_id, star_idx, results->bomb.bio_pop_killed);
                }

                if ((MOX::_game_type > 1 && MOX::_game_type < 4) && (MOX::_PLAYER_NUM == attacker_id || MOX::_PLAYER_NUM == colony_owner)) {
                    MULTPLAY::Load_Net_Next_Turn_Screen_(0);
                    MOX::_FULL_DRAW_SCREEN_FLAG = 1;
                    MULTPLAY::Chat_Box_Input_Loop_();
                }
            }
        } else {
            if (ERIC::Player_Can_Mind_Control_Colony_(attacker_id, (int16_t)colony_idx)) {
                results->info.is_mind_controlled = 1;
                invasion::Change_Colony_Ownership_(colony, attacker_id, &results->info);
                COMBFIND::_current_star_has_player[attacker_id].colony_count++;
                COMBFIND::_current_star_has_player[colony_owner].colony_count--;
            } else {
                bool has_stellar = Has_Stellar_Converter_(ship_list, n_ships);
                int enemy_star_idx = MOX::_planet[colony->planet].star_index;
                bool knows_attacker = false;

                for (int i = 0; i < colony->n_pops; ++i) {
                    if ((colony->pop[i] & 0xF) == (uint32_t)attacker_id) {
                        knows_attacker = true;
                        break;
                    }
                }

                memset(&results->bomb, 0, sizeof(s_combat_bomb_result));
                results->bomb.attacker_idx = attacker_id;
                results->bomb.colony_idx = (uint8_t)colony_idx;
                COMBAT::Bombardment_(&results->bomb);

                int bomb_hits = results->bomb.bomb_hits_remaining;
                int bio_hits = results->bomb.bio_hits_remaining;

                if (bomb_hits != 0 || bio_hits != 0 || has_stellar) {
                    bool surrender = false;
                    if (MOX::_player[colony_owner].objectives != 'd' /* ECON_INDUSTRY */ && MOX::_settings.game_difficulty > 2) {
                        if (COLCALC::May_Surrender_Star_(enemy_star_idx, colony_owner, attacker_id) && (int)(colony->n_pops * 2) <= bomb_hits) {
                            surrender = true;
                        }
                    }

                    if (surrender) {
                        MOX::_star[star_idx].surrender_to[colony_owner] = attacker_id + 8;
                        diplomac::Declare_Peace_(colony_owner, attacker_id);
                    } else if (knows_attacker) {
                        bool success = false;
                        bool done = false;
                        do {
                            if (aibomb::AI_Invasion_Will_Succeed_(attacker_id, ship_list, n_ships, colony)) {
                                success = true;
                            } else {
                                if (colony->n_pops == 1 && (colony->pop[0] & 0xF) == (uint32_t)attacker_id && 
                                    MOX::_player[attacker_id].personality != 1 && (COMBFIND::Player_Owns_Transports_(attacker_id) || 
                                    (MOX::_player[attacker_id].traits[25] != 0 && MOX::_player[colony_owner].traits[25] == 0))) {
                                    done = true;
                                } else if (bio_hits == 0) {
                                    int used = BOMB::Resolve_Bomb_Hit_(colony, &results->bomb, bomb_hits);
                                    if (used == 0) bomb_hits = 0; else bomb_hits -= used;
                                } else {
                                    BOMB::Resolve_Biological_Hit_(colony, &results->bomb);
                                    bio_hits--;
                                }
                            }
                        } while (!success && !done && (bio_hits + bomb_hits != 0));
                    } else {
                        int stars_in_range = N_Enemy_Stars_In_Range_(attacker_id, colony_owner);
                        if (stars_in_range < 2 && N_Enemy_Stars_(colony_owner) != 1) {
                            bool success = false;
                            bool done = false;
                            do {
                                if (aibomb::AI_Invasion_Will_Succeed_(attacker_id, ship_list, n_ships, colony)) {
                                    success = true;
                                } else {
                                    if (colony->n_pops == 1) {
                                        done = true;
                                    } else if (bio_hits == 0) {
                                        int used = BOMB::Resolve_Bomb_Hit_(colony, &results->bomb, bomb_hits);
                                        if (used == 0) bomb_hits = 0; else bomb_hits -= used;
                                    } else {
                                        BOMB::Resolve_Biological_Hit_(colony, &results->bomb);
                                        bio_hits--;
                                    }
                                }
                            } while (!success && !done && (bio_hits + bomb_hits != 0));
                        } else if (has_stellar) {
                            results->info.planet_destroyed_flag = 1;
                            BOMB::Destroy_Colony_(colony, 2);
                        } else {
                            BOMB::Apply_Bomb_Hits_(&results->bomb);
                        }
                    }

                    if (results->bomb.bio_pop_killed != 0) {
                        COMBINIT::Apply_Bio_Weapon_Penalty_(attacker_id, star_idx, results->bomb.bio_pop_killed);
                    }

                    if (MOX::_player[colony_owner].objectives == 'd' /* ECON_INDUSTRY */ && 
                        (MOX::_settings.strategic_combat_flag == 0 || (MOX::_game_type != 2 && MOX::_game_type != 3) || 
                        MOX::_player[colony_owner].network_player_id == NETMOX::_net.local_net_id) && results->bomb.damage_occurred != 0) {
                        COLBOMB::Colony_Bombing_Screen_(&results->bomb);
                    }

                    NETMOX::Russ_Change_Relations_(-10000, attacker_id, colony_owner, 10, star_idx, results->bomb.population_killed);

                    if (results->bomb.colony_wiped_out != 0) {
                        BOMB::Destroy_Colony_(colony, 1);
                    }
                }

                if (results->info.planet_destroyed_flag == 0 && results->bomb.colony_wiped_out == 0 && MOX::_star[star_idx].surrender_to[colony_owner] == 0 && 
                    invasion::Has_Defending_Troops_(attacker_id, ship_list, n_ships, (int16_t)colony_idx)) {
                    invasion::Invade_(ship_list, n_ships, (int16_t)colony_idx, &results->info);
                    if (MOX::_player[colony_owner].objectives == 'd' /* ECON_INDUSTRY */ && 
                        (MOX::_settings.strategic_combat_flag == 0 || (MOX::_game_type != 2 && MOX::_game_type != 3) || 
                        MOX::_player[colony_owner].network_player_id == NETMOX::_net.local_net_id)) {
                        int invade_seed = random::Get_Random_Seed_();
                        ERIC::Force_Fade_To_Colony_Palette_(1);
                        COLGCBT::Colony_Combat_Screen_(&results->info.attacker_invasion, &results->info.defender_invasion, results->info.ground_defense_strength, (int16_t)colony_idx);
                        ERIC::Force_Fade_To_Colony_Palette_(0);
                        random::Set_Random_Seed_(invade_seed);
                    }

                    NETMOX::Russ_Change_Relations_(-10000, attacker_id, colony_owner, 10, star_idx, 0);

                    if (colony->owner == attacker_id) {
                        COMBFIND::_current_star_has_player[attacker_id].colony_count++;
                        COMBFIND::_current_star_has_player[colony_owner].colony_count--;
                    }

                    if (MOX::_player[attacker_id].objectives == 'd' || MOX::_player[colony_owner].objectives == 'd') {
                        COMBFIND::_messed_up_fleet_palette = 1;
                    }
                }
            }
        }
    } else {
        if (attacker_id > 9 && attacker_id < 15) {
            NETMOX::Russ_Event_Monster_Update_(attacker_id, colony->planet, -1);
        }

        memset(&results->bomb, 0, sizeof(s_combat_bomb_result));
        results->bomb.attacker_idx = attacker_id;
        results->bomb.colony_idx = (uint8_t)colony_idx;
        if (attacker_id == 10) { // Amoeba
            results->bomb.bomb_hits_remaining = 20;
            results->bomb.bio_hits_remaining = 0;
        }

        COMBAT::Bombardment_(&results->bomb);
        BOMB::Apply_Bomb_Hits_(&results->bomb);

        if (MOX::_player[colony_owner].objectives == 'd' /* ECON_INDUSTRY */ && 
            (MOX::_settings.strategic_combat_flag == 0 || (MOX::_game_type != 2 && MOX::_game_type != 3) || 
            MOX::_player[colony_owner].network_player_id == NETMOX::_net.local_net_id) && results->bomb.damage_occurred != 0) {
            
            if (attacker_id == 10) {
                ERIC::Play_Amoeba_Flic_();
                COMBFIND::_messed_up_fleet_palette = 1;
                if (results->bomb.colony_wiped_out == 0) {
                    MOX::_planet[planet_idx].climate = 0;
                    MOX::_planet[planet_idx].food_per_farmer = 0;
                    COLCALC::Colony_Environmental_Stuff_(colony);
                    invasion::Enforce_Population_Limits_At_Colony_(colony);
                    COLCALC::Colony_Calculation_((int16_t)(((int)colony - (int)MOX::_colony) / sizeof(s_colony)));
                }
            }
            COLBOMB::Colony_Bombing_Screen_(&results->bomb);
        }

        if (results->bomb.colony_wiped_out != 0) {
            BOMB::Destroy_Colony_(colony, 1);
        }
    }

    if (attacker_id == 8) { // Antarans
        if (n_ships > 0) {
            int count = n_ships;
            int16_t* list = ship_list;
            while (count > 0) {
                antarans::Restock_Antaran_Ship_(MOX::_ship[*list].d.size);
                SHIPS::Kill_Ship_(*list);
                list++;
                count--;
            }
        }
        (*COMBFIND::_star_has_nonplayer)[star_idx] &= 0xFE;
    }

    MAINSCR::Update_Ship_Icons_In_Between_Turns_();
}

void __cdecl Strategic_Results_Popup_(s_battle_results* battle_results) {
    ERICNET::Mox_Play_Sound_(0x33);
    MAINSCR::Update_Ship_Icons_In_Between_Turns_();

    COMBAT::Message_Box_Titled_(battle_results->info.summary, battle_results->info.details);

    if (battle_results->info.rescued_hero_id != 0) {
        ERIC::Play_Take_Orion_Flic_();
        OFFICER::Rescued_Loknar_Popup_(
            MOX::_PLAYER_NUM,
            battle_results->info.orion_techs,
            battle_results->info.rescued_hero_id
        );
    }

    MULTPLAY::Load_Net_Next_Turn_Screen_(0);
}

void __cdecl Bomb_Results_Popups_(s_battle_results* res) {
    if (res->outpost_destroyed_flag == 0) {
        if (res->info.planet_destroyed_flag == 0) {
            if (res->bomb.damage_occurred != 0) {
                if (res->bomb.attacker_idx == 10) {
                    ERIC::Play_Amoeba_Flic_();
                }
                COLBOMB::Colony_Bombing_Screen_(&res->bomb);
            }
        } else {
            ERIC::Play_Planetary_Destruction_Flic_();
            _messed_up_fleet_palette = 1;
        }
    } else {
        GENDRAW::Message_Box_(_outpost_destroyed_msg);
    }

    if (res->info.attacker_invasion.unit_counts[1] != 0) {
        uint32_t seed = random::Get_Random_Seed_();

        COLGCBT::Colony_Combat_Screen_(
            &res->info.attacker_invasion,
            &res->info.defender_invasion,
            res->info.ground_defense_strength,
            (int16_t)MOX::_planet[res->combat_planet_id].colony_index
        );

        MULTPLAY::Load_Net_Next_Turn_Screen_(1);
        random::Set_Random_Seed_(seed);
    }

    if (res->info.is_mind_controlled != 0) {
        GENDRAW::Message_Box_(_colony_seized_by_mind_control_msg);
    }
}

int32_t __cdecl Do_Bombardment_(int8_t attacker_id, uint8_t colony_idx) {
    s_combat_bomb_result result;

    memset(&result, 0, sizeof(s_combat_bomb_result));

    result.attacker_idx = attacker_id;
    result.colony_idx = colony_idx;

    COMBAT::Bombardment_(&result);

    return result.bomb_hits_remaining + result.bio_hits_remaining;
}

uint8_t __cdecl Has_Stellar_Converter_(int16_t* ship_list, int16_t ship_count) {
    for (int32_t i = 0; i < ship_count; ++i) {
        int32_t ship_idx = ship_list[i];
        if (MOX::_ship[ship_idx].status == 0) {
            if (INITSHIP::Ship_Has_Weapon_(ship_idx, 0x26) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

int32_t __cdecl N_Enemy_Stars_In_Range_(int16_t player_idx, uint8_t enemy_player_idx) {
    int32_t count = 0;

    for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
        s_star_data* star = &MOX::_star[i];

        if ((star->has_colony >> enemy_player_idx) & 1) {
            if (SHIPMOVE::Star_In_Normal_Range_Of_Player_(star, player_idx)) {
                count++;
            }
        }
    }

    return count;
}

int32_t __cdecl N_Enemy_Stars_(uint8_t player_idx) {
    int32_t count = 0;

    for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
        if ((MOX::_star[i].has_colony >> player_idx) & 1) {
            count++;
        }
    }

    return count;
}

int8_t __cdecl Player_Owns_Transports_(int player_idx) {
    for (int16_t i = 0; i < MOX::_NUM_SHIPS; ++i) {
        if (MOX::_ship[i].owner == player_idx &&
            MOX::_ship[i].d.ship_type == SHIP_TYPE_TRANSPORT &&
            MOX::_ship[i].status < 5) {
            return 1;
        }
    }
    return 0;
}

void __cdecl Do_Net_Combat_Update_(bool message_sent) {
    (void)message_sent;
    RUSS::Mox_Update_();

    if (_in_net_update != 0) {
        RUSS::Mox_Update_();
        return;
    }

    _in_net_update = 1;

    if (MOX::_NUM_PLAYERS > 0) {
        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            RUSS::Mox_Update_();
            uint8_t state = NETMOX::_net.player_net_info[i].sync_state;

            switch (state) {
            case 0x11: {
                int16_t sync_colony_id = NETMOX::_net.player_net_info[i].sync_colony_id;
                if (sync_colony_id == -1) {
                    NETMOX::_net.player_net_info[i].sync_state = 0x0e;
                    for (int16_t j = 0; j < MOX::_NUM_PLAYERS; ++j) {
                        if ((uint8_t)NETMOX::_net.player_net_info[j].sync_phase_ready_flag == i) {
                            RUSS::Mox_Update_();
                            NETMOX::_net.player_net_info[j].sync_phase_ready_flag = (undefined)0xff;
                        }
                    }
                    NETMOX::_net.sync_phase_active_flag = 0;
                    MOX::_star[NETMOX::_net.player_net_info[i].combat_star_id].ignore_combat_ships |= (1 << i);
                } else {
                    MOX::_star[NETMOX::_net.player_net_info[i].combat_star_id].ignore_combat_ships = 0;

                    NETMOX::_net.player_net_info[sync_colony_id].sync_star_id = NETMOX::_net.player_net_info[i].sync_star_id;
                    NETMOX::_net.player_net_info[sync_colony_id].sync_planet_id = NETMOX::_net.player_net_info[i].sync_planet_id;
                    NETMOX::_net.player_net_info[sync_colony_id].sync_colony_id = NETMOX::_net.player_net_info[i].sync_colony_id;

                    for (int16_t j = 0; j < MOX::_NUM_PLAYERS; ++j) {
                        if (j != i && j != sync_colony_id && (uint8_t)NETMOX::_net.player_net_info[j].sync_phase_ready_flag == i) {
                            RUSS::Mox_Update_();
                            NETMOX::_net.player_net_info[j].sync_phase_ready_flag = (undefined)0xff;
                        }
                    }

                    GNN_Reports_Combat_(i, sync_colony_id);

                    uint8_t msg_type = 0;
                    if (MOX::_settings.strategic_combat_flag == 0) {
                        if (sync_colony_id < 8 && MOX::_player[sync_colony_id].objectives == 100 /* 'd' */) {
                            NETMOX::_net.player_net_info[i].sync_state = 0x14;
                            NETMOX::_net.player_net_info[sync_colony_id].sync_state = 0x15;
                            int16_t colony_id = Target_Planet_To_Colony_(NETMOX::_net.player_net_info[i].sync_planet_id);
                            NETMOX::Send_You_Are_Target_Info_(
                                sync_colony_id, i, NETMOX::_net.player_net_info[i].sync_star_id, colony_id, NETMOX::_net.player_net_info[i].combat_star_id);
                            msg_type = 0x14;
                        } else {
                            NETMOX::_net.player_net_info[i].sync_state = 0x16;
                            msg_type = 0x14;
                        }
                    } else {
                        NETMOX::_net.player_net_info[i].sync_state = 0x18;
                        NETMOX::_net.player_net_info[sync_colony_id].sync_state = 0x19;
                        msg_type = 0x14;
                    }
                    NETMOX::Mox_Send_Message_((int16_t)MOX::_player[i].network_player_id, msg_type, nullptr, 0);
                }
                break;
            }
            case 0x1b: {
                int16_t sync_phase_ready_flag = (uint8_t)NETMOX::_net.player_net_info[i].sync_phase_ready_flag;
                if (sync_phase_ready_flag == i) {
                    Update_Player_Needs_To_Attack_(
                        NETMOX::_net.player_net_info[i].combat_star_id,
                        i,
                        NETMOX::_net.player_net_info[i].sync_colony_id
                    );
                } else {
                    Update_Player_Needs_To_Attack_(
                        NETMOX::_net.player_net_info[i].combat_star_id,
                        sync_phase_ready_flag,
                        i
                    );
                }
                NETMOX::_net.player_net_info[i].sync_state = 0x0e;
                NETMOX::_net.player_net_info[i].sync_phase_ready_flag = (undefined)0xff;
                NETMOX::_net.sync_phase_active_flag = 0;
                break;
            }
            case 0x1d: {
                Do_Nonhuman_Attack_Against_Human_(i);
                (*_star_has_nonplayer)[NETMOX::_net.player_net_info[i].combat_star_id] &= 0xef;
                NETMOX::_net.player_net_info[i].sync_state = 0x19;
                NETMOX::_net.sync_phase_active_flag = 0;
                NETMOX::_net.field82_0x160 = (undefined)0xff;
                break;
            }
            case 0x1e: {
                Do_Dragon_Bribe_(
                    i,
                    NETMOX::_net.player_net_info[i].combat_star_id,
                    (int16_t)NETMOX::_net.field168_0x209
                );
                NETMOX::_net.player_net_info[i].sync_phase_ready_flag = (undefined)0xff;
                NETMOX::_net.player_net_info[i].sync_state = 0x0e;
                NETMOX::_net.sync_phase_active_flag = 0;
                NETMOX::_net.field82_0x160 = (undefined)0xff;
                break;
            }
            case 0x1f: {
                if (NETMOX::_net.sync_state == 0x0e) {
                    NETMOX::_net.sync_state = 0x21;
                    NETMOX::_net.player_net_info[i].sync_state = 0x20;
                    NETMOX::Mox_Send_Message_((int16_t)MOX::_player[i].network_player_id, 0x31, nullptr, 0);
                }
                break;
            }
            }
        }
    }

    if (_n_battle_stars == 0) {
        bool bVar3 = false;
        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (MOX::_player[i].objectives == 100 /* 'd' */ && NETMOX::_net.player_net_info[i].sync_state != 0x0e) {
                RUSS::Mox_Update_();
                bVar3 = true;
            }
        }
        if (!bVar3) {
            NETMOX::_net.sync_state = 0x23;
        }
        _in_net_update = 0;
        RUSS::Mox_Update_();
        return;
    }

    uint8_t state = NETMOX::_net.player_net_info[MOX::_PLAYER_NUM].sync_state;
    if (state == 0x10 || state == 0x0f || NETMOX::_net.sync_phase_active_flag != 0) {
        _in_net_update = 0;
        RUSS::Mox_Update_();
        return;
    }

    int16_t star_id;
    int16_t attacker_id;
    Pick_Star_And_Attacker_(&star_id, &attacker_id);

    if (star_id == -1) {
        NETMOX::_net.sync_phase_active_flag = 1;
        _in_net_update = 0;
        RUSS::Mox_Update_();
        return;
    }

    int8_t is_sneak_attack = 0;
    int16_t target_star_id = -1;
    int16_t target_planet_id = -1;

    for (int16_t j = 0; j < _target.num_player_targets; ++j) {
        int16_t target_player = _target.player_targets[j];
        NETMOX::_net.player_net_info[target_player].combat_star_id = (uint8_t)star_id;
        NETMOX::_net.player_net_info[target_player].sync_phase_ready_flag = (undefined)attacker_id;
    }

    for (int16_t j = 0; j < _target.num_colony_targets; ++j) {
        int16_t target_colony = _target.colony_targets[j];
        int8_t owner = MOX::_colony[target_colony].owner;
        NETMOX::_net.player_net_info[owner].combat_star_id = (uint8_t)star_id;
        NETMOX::_net.player_net_info[owner].sync_phase_ready_flag = (undefined)attacker_id;
    }

    NETMOX::_net.player_net_info[attacker_id].combat_star_id = (uint8_t)star_id;
    NETMOX::_net.player_net_info[attacker_id].sync_phase_ready_flag = (undefined)attacker_id;

    if (attacker_id < 8) {
        if (MOX::_player[attacker_id].objectives == 100 /* 'd' */) {
            if ((MOX::_star[star_id].ignore_combat_ships & (1 << attacker_id)) == 0) {
                NETMOX::_net.player_net_info[attacker_id].sync_state = 0x10;
                NETMOX::Send_Pick_Target_Info_(attacker_id, star_id, &_target);
                _in_net_update = 0;
                RUSS::Mox_Update_();
                return;
            }
        }
    }

    int16_t target_colony_id;
    if (attacker_id < 8 && MOX::_player[attacker_id].objectives == 100 /* 'd' */) {
        target_colony_id = -1;
    } else {
        if (attacker_id < 8) {
            Get_AI_Target_(star_id, attacker_id, &target_star_id, &target_planet_id, &is_sneak_attack);
        } else {
            Get_NPC_Target_(star_id, attacker_id, &target_star_id, &target_planet_id);
        }
        if (target_planet_id != -1) {
            target_colony_id = MOX::_colony[target_planet_id].owner;
            MOX::_player[target_colony_id].last_attacker = (int8_t)attacker_id;
        } else {
            target_colony_id = target_star_id;
        }
    }

    NETMOX::_net.player_net_info[attacker_id].sync_star_id = target_star_id;
    int16_t sync_planet_id = Target_Colony_To_Planet_(target_planet_id);
    NETMOX::_net.player_net_info[attacker_id].sync_planet_id = sync_planet_id;
    NETMOX::_net.player_net_info[attacker_id].sync_colony_id = target_colony_id;

    if (target_colony_id == -1) {
        NETMOX::_net.sync_phase_active_flag = 0;
        for (int16_t j = 0; j < MOX::_NUM_PLAYERS; ++j) {
            if ((uint8_t)NETMOX::_net.player_net_info[j].sync_phase_ready_flag == attacker_id) {
                RUSS::Mox_Update_();
                NETMOX::_net.player_net_info[j].sync_phase_ready_flag = (undefined)0xff;
            }
        }
    } else {
        MOX::_star[star_id].ignore_combat_ships = 0;
        NETMOX::_net.player_net_info[target_colony_id].sync_star_id = NETMOX::_net.player_net_info[attacker_id].sync_star_id;
        NETMOX::_net.player_net_info[target_colony_id].sync_planet_id = NETMOX::_net.player_net_info[attacker_id].sync_planet_id;
        NETMOX::_net.player_net_info[target_colony_id].sync_colony_id = NETMOX::_net.player_net_info[attacker_id].sync_colony_id;

        for (int16_t j = 0; j < MOX::_NUM_PLAYERS; ++j) {
            if (j != attacker_id && j != target_colony_id && (uint8_t)NETMOX::_net.player_net_info[j].sync_phase_ready_flag == attacker_id) {
                RUSS::Mox_Update_();
                NETMOX::_net.player_net_info[j].sync_phase_ready_flag = (undefined)0xff;
            }
        }

        if (target_colony_id < 8 && MOX::_player[target_colony_id].objectives == 100 /* 'd' */) {
            if (attacker_id == 12 && target_planet_id != -1) {
                NETMOX::_net.player_net_info[target_colony_id].sync_state = 0x1c;
                NETMOX::Send_Dragon_Info_(target_planet_id);
            } else {
                Do_Nonhuman_Attack_Against_Human_(target_colony_id);
            }
        } else {
            int16_t colony_id = Target_Colony_To_Planet_(target_planet_id);
            Do_1_Combat_(star_id, attacker_id, target_star_id, colony_id, nullptr, is_sneak_attack);
            Update_Player_Needs_To_Attack_(star_id, attacker_id, target_colony_id);
            NETMOX::_net.sync_phase_active_flag = 0;
            NETMOX::_net.player_net_info[attacker_id].sync_phase_ready_flag = (undefined)0xff;
            NETMOX::_net.player_net_info[target_colony_id].sync_phase_ready_flag = (undefined)0xff;
        }
    }

    _in_net_update = 0;
    RUSS::Mox_Update_();
}

void __cdecl Do_Nonhuman_Attack_Against_Human_(int player_idx) {
    s_battle_results local_6d0;

    int8_t attacker_id = (int8_t)NETMOX::_net.player_net_info[player_idx].sync_phase_ready_flag;
    int16_t combat_star_id = (int16_t)NETMOX::_net.player_net_info[player_idx].combat_star_id;

    GNN_Reports_Combat_(attacker_id, player_idx);

    if (MOX::_settings.strategic_combat_flag == 0) {
        int16_t colony_id = Target_Planet_To_Colony_(NETMOX::_net.player_net_info[player_idx].sync_planet_id);

        NETMOX::Send_You_Are_Target_Info_(
            (int16_t)player_idx,
            (int16_t)attacker_id,
            NETMOX::_net.player_net_info[player_idx].sync_star_id,
            colony_id,
            (int16_t)combat_star_id
        );

        NETMOX::_net.player_net_info[player_idx].sync_state = 0x17;
        return;
    }

    memset(&local_6d0, 0, sizeof(s_battle_results));

    // The first 12 bytes of s_battle_results map exactly to the 12 bytes of s_net_player_info.
    memcpy(&local_6d0, &NETMOX::_net.player_net_info[player_idx], 12);

    local_6d0.planet_id = local_6d0.combat_planet_id;

    local_6d0.combat_result = (int8_t)Do_1_Combat_(
        (int16_t)combat_star_id,
        (int16_t)attacker_id,
        NETMOX::_net.player_net_info[player_idx].sync_star_id,
        NETMOX::_net.player_net_info[player_idx].sync_planet_id,
        &local_6d0,
        0
    );

    Update_Player_Needs_To_Attack_(combat_star_id, attacker_id, player_idx);

    NETMOX::_net.sync_phase_active_flag = 0;
    NETMOX::_net.player_net_info[attacker_id].sync_phase_ready_flag = (undefined)0xFF; // -1
    NETMOX::_net.player_net_info[player_idx].sync_state = 0x19;

    NETMOX::Mox_Send_Message_(
        MOX::_player[player_idx].network_player_id,
        0x16,
        &local_6d0,
        (uint16_t)sizeof(s_battle_results)
    );
}

void __cdecl Send_Star_Player_Presence_(uint8_t **stream, int16_t star_idx) {
    s_combfind_star_has_player *src;
    if (star_idx == -1) {
        src = _current_star_has_player;
    } else {
        src = (*_star_has_player)[star_idx];
    }

    memcpy(*stream, src, 88); /* 8 * sizeof(s_combfind_star_has_player) */
    uint8_t *ptr = *stream + 88;
    *stream = ptr;

    if (star_idx != -1) {
        *ptr = (*_star_has_nonplayer)[star_idx];
    } else {
        *ptr = *_current_star_has_nonplayer;
    }
    *stream = ptr + 1;
}

void __cdecl GNN_Reports_Combat_(int player1_idx, int player2_idx) {
    char buf[200];
    int p1;
    int p2;

    if (random::Random_(2) == 1) {
        p1 = player1_idx;
        p2 = player2_idx;
    } else {
        p1 = player2_idx;
        p2 = player1_idx;
    }

    char* name2 = COLCALC::Player_Name_(p2);
    char* name1 = COLCALC::Player_Name_(p1);

    sprintf(buf, _combat_is_reported_between_s_and_s_msg, name1, name2);

    CHAT::Send_GNN_Chat_Msg_(buf);
}

} // namespace COMBFIND