#include "deps.h"

namespace COMBFIND {

void __cdecl Search_For_Battles_() {
    void* saved_auto_fn;
    MOX2::Get_Auto_Function_(&saved_auto_fn);

    if (MOX::_game_type < 2) {
        fields::Assign_Auto_Function_(MAINSCR::Draw_Mini_Main_Screen_, 1);
    }

    MAINSCR::Update_Ship_Icons_In_Between_Turns_();

    s_combfind_star_has_player star_has_player_stack[73][8];
    int8_t star_has_nonplayer_stack[73];
    int16_t player_needs_to_attack_stack[73];
    int8_t battle_stars_stack[76];
    uint32_t attack_confirmed_buf[16];

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
        _attack_confirmed = attack_confirmed_buf;

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
                    int16_t planet_idx = COLONY::Get_Colony_Planet_Id_(colony_idx);
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

} // namespace COMBFIND