#include "pch.h"

namespace NETMOX {
    int32_t __cdecl Net_Id_To_Player_Id_(int32_t player_idx) {
        for (int32_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (MOX::_player[i].network_player_id == player_idx) {
                return i;
            }
        }
        return -1;
    }

    char* __cdecl Load_Netmox_Strings_(char* strings) {
        _modem_disk_error = strings;
        strings += strlen(strings) + 1;

        _modem_diff_buffer_error = strings;
        strings += strlen(strings) + 1;

        _incompatible_versions_error = strings;
        strings += strlen(strings) + 1;

        _only_1_host_allowed_error = strings;
        strings += strlen(strings) + 1;

        _net_send_error = strings;
        strings += strlen(strings) + 1;

        _lost_net_user_error = strings;
        strings += strlen(strings) + 1;

        _sending_chunk_d_of_d_fmt = strings;
        strings += strlen(strings) + 1;

        _ambassador_left_before_you_arrived_fmt = strings;
        strings += strlen(strings) + 1;

        return strings;
    }

    int8_t __cdecl Net_May_Begin_AI_Diplomacy_() {
        int8_t uninitialized_ret;

        if (NETMOX::_net.sync_state >= 0x25) {
            if (NETMOX::_net.sync_state <= 0x26) {
                return 0;
            }

            if (NETMOX::_net.sync_state == 0x28) {
                return 1;
            }
        }

        // Returns an uninitialized stack value, matching assembly at 0x005195b9
        return uninitialized_ret;
    }

    void __cdecl Human_Abdicates_(int player_idx) {
        Send_Drop_Out_Info_(player_idx, 0x37);
    }

    void __cdecl Human_Surrenders_(int player_idx) {
        Send_Drop_Out_Info_(player_idx, 0x38);
    }

    void __cdecl Ditch_Leader_(int32_t leader_idx) {
        MOX::_leaders[leader_idx].eta = 0;
        MOX::_leaders[leader_idx].status = -1;
        MOX::_leaders[leader_idx].location = -1;
        MOX::_leaders[leader_idx].player_index = -1;
    }

    int32_t __cdecl Net_Diplomacy_With_(int16_t limit) {
        return _net.diplomacy_target_id;
    }

    void __cdecl Net_Cancel_Human_Diplomacy_Request_() {
        if (MOX::_game_type >= 2 && MOX::_game_type <= 3) {
            int32_t target_id = _net.diplomacy_target_id;
            _net.diplomacy_target_id = -1;
            _net.sync_state = 4;

            int16_t network_player_id = MOX::_player[target_id].network_player_id;
            if (network_player_id != -1) {
                Mox_Send_Message_(network_player_id, 0x1F, nullptr, 0);
            }
        }
    }

    void __cdecl Net_End_Human_Diplomacy_() {
        _net.diplomacy_target_id = -1;
        _net.sync_state = 4;
    }

    bool __cdecl Net_Human_Rejected_Diplomacy_() {
        bool result = false;

        if (_net.sync_state == 0x2A /* '*' */) {
            _net.sync_state = 0x04;
            result = true;
        }

        return result;
    }

    bool __cdecl Allowed_To_Cancel_Human_Diplomacy_() {
        return _net.sync_state == 0x29;
    }

    int8_t __cdecl Human_Hit_Next_Turn_(int16_t player_idx) {
        return _net.players_hit_next_turn[player_idx];
    }

    int8_t __cdecl Human_Diplomat_Says_Wait_() {
        int8_t wait_flag = _net.human_diplomat_wait_flag;
        _net.human_diplomat_wait_flag = 0;
        return wait_flag;
    }

    int8_t __cdecl Net_May_Begin_Human_Diplomacy_() {
        int8_t local_1; // Uninitialized stack value

        switch (_net.sync_state) {
        case 4:
        case 41: /* ')' */
        case 42: /* '*' */
        case 46: /* '.' */
        case 47: /* '/' */
            return 0;
        case 43: /* '+' */
            return 1;
        default:
            return local_1;
        }
    }

    bool __cdecl Net_Cancel_AI_Diplomacy_Confirmed_() {
        return _net.sync_state == 4;
    }

    void __cdecl Net_End_AI_Diplomacy_() {
        if (MOX::_game_type >= 2 && MOX::_game_type <= 3) {
            s_net_pack_end_ai_diplomacy msg;

            int32_t target_player_id = _net.diplomacy_target_id;
            msg.target_player_id = target_player_id;
            msg.target_player = MOX::_player[target_player_id];

            int32_t player_num = MOX::_PLAYER_NUM;
            msg.player_num = player_num;
            msg.player = MOX::_player[player_num];

            for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
                msg.surrender_to[i] = MOX::_star[i].surrender_to[target_player_id];
            }

            _net.diplomacy_target_id = -1;
            _net.sync_state = 4;

            if (_net.host_election_winner_id != -1) {
                Mox_Send_Message_(_net.host_election_winner_id, 10, &msg, static_cast<int16_t>(sizeof(msg)));
            }
        }
    }

    void __cdecl Net_Cancel_AI_Diplomacy_Request_() {
        if (MOX::_game_type >= 2 && MOX::_game_type <= 3) {
            _net.diplomacy_target_id = -1;
            _net.sync_state = 0x27;
            if (_net.host_election_winner_id != -1) {
                Mox_Send_Message_(_net.host_election_winner_id, 8, &_net.diplomacy_target_id, 1);
            }
        }
    }

    bool __cdecl No_AI_Diplomacy_Allowed_() {
        if (_net.host_election_winner_id == -1) {
            _net.sync_state = 4;
            return true;
        }
        return false;
    }

    void __cdecl Net_Request_AI_Diplomacy_(int16_t player_idx) {
        _net.sync_state = 0x25;
        _net.diplomacy_target_id = (int8_t)player_idx;

        if (_net.host_election_winner_id != -1) {
            int8_t target_id = (int8_t)player_idx;
            Mox_Send_Message_(_net.host_election_winner_id, 5, &target_id, 1);
        }
    }

    void __cdecl Receive_Message_(int16_t net_id, uint16_t packet_id, uint32_t len, uint8_t* packet) {
        int32_t player_id = Net_Id_To_Player_Id_(net_id & 0xFFFF);
        _in_receive_message = 1;

        if (packet_id >= 2000) {
            DIP_SCRN::Diplomacy_Receive_Message_(net_id, packet_id, len, reinterpret_cast<s_dipscrn_net_message*>(packet));
        } else if (packet_id >= 1000) {
            CMBTMOV1::Tactical_Combat_Receive_Message_(net_id, packet_id, len, reinterpret_cast<s_net_combat_message*>(packet));
        } else {
            MAINSCR::Main_Receive_Message_(player_id, net_id, packet_id, len, packet);
        }

        _in_receive_message = 0;
    }

    void __cdecl Russ_Change_Relations_(int32_t change, int16_t attacker, int16_t defender, int32_t reason, int16_t star_idx, int32_t param6) {
        if (MOX::_game_type != 2 && MOX::_game_type != 3) {
            diplomac::Change_Relations_((int16_t)change, attacker, defender, (int16_t)reason, star_idx, (int16_t)param6);
        } else {
            s_net_pack_change_relation msg_data;
            msg_data.change = (int16_t)change;
            msg_data.attacker = attacker;
            msg_data.defender = defender;
            msg_data.reason = (int16_t)reason;
            msg_data.star_idx = star_idx;
            msg_data.param6 = (int16_t)param6;

            Mox_Send_Message_(_net.local_net_id, 0x43, &msg_data, 12);
        }
    }

    uint8_t Humans_Requesting_Diplomacy_() {
        return _net_players_active_mask;
    }

    int32_t __cdecl Human_Requesting_Diplomacy_() {
        int32_t target_player_idx = -1;
        uint32_t ready_count = 0;

        if (_net.players_ready_mask[0] != 0) {
            for (int32_t i = 0; i < (int32_t)MOX::_NUM_PLAYERS; ++i) {
                // Check if i-th bit is set in the first byte of the mask
                if ((_net.players_ready_mask[0] >> i) & 1) {
                    ready_count++;
                    // Reservoir sampling: select current index with probability 1/ready_count
                    if (random::Random_(ready_count) == 1) {
                        target_player_idx = i;
                    }
                }
            }

            // Clear the bit of the selected player index
            _net.players_ready_mask[0] &= ~(1 << (uint8_t)target_player_idx);
        }

        return target_player_idx;
    }

    int32_t __cdecl Mox_Send_Message_(int16_t param_1, uint16_t param_2, void* packet, uint16_t len) {
        uint8_t dummy_packet;
        uint8_t skip_send;
        int32_t result = 1;

        _sending_msg = 1;

        if (len == 0) {
            len = 1;
            packet = &dummy_packet;
        }

        Update_();

        if (_net.net_host_id == param_1) {
            Receive_Message_(param_1, param_2, len, static_cast<uint8_t*>(packet));
        } else {
            skip_send = 0;
            do {
                Update_();
            } while (netcode::Net_Is_Sending_() != 0);

            if (param_1 == 0x7FFE) {
                int32_t last_valid_id = -1;
                int32_t count = 0;

                for (int32_t i = 0; i < MOX::_NUM_PLAYERS; i++) {
                    int16_t net_id = MOX::_player[i].network_player_id;
                    if (net_id != -1 && _net.net_host_id != net_id) {
                        last_valid_id = net_id;
                        count++;
                    }
                }

                if (count == 0) {
                    skip_send = 1;
                } else if (count == 1) {
                    param_1 = static_cast<int16_t>(last_valid_id);
                }
            }

            if (skip_send == 0) {
                result = netcode::Net_Send_Message_(param_1, param_2, packet, len);
            }

            if (result == 0) {
                exitm::Exit_With_message2_(_net_send_error);
            }
        }

        _sending_msg = 0;
        return result;
    }

    /**
     * @brief Refuses a diplomacy request from another player and updates network state.
     * @param player_idx The index of the player whose request is being refused.
     */
    void __cdecl Refuse_Diplomacy_Request_(int player_idx) {
        uint8_t bit_mask = 1 << (player_idx & 0x1F);

        // Mark the player as being ignored in our local player data
        MOX::_player[MOX::_PLAYER_NUM].ignoring |= bit_mask;

        uint8_t clear_mask = ~bit_mask;

        // Remove player from active and ready masks in network state
        NETMOX::_net.players_active_mask &= clear_mask;
        NETMOX::_net.players_ready_mask[0] &= clear_mask;

        // Notify the remote player that the diplomacy request was refused
        NETMOX::Mox_Send_Message_(MOX::_player[player_idx].network_player_id, 29 /* MSG_TYPE_REFUSE_DIPLOMACY */, nullptr, 0);
    }

    void __cdecl Tell_Diplomacy_Requester_To_Wait_(int32_t player_idx) {
        Mox_Send_Message_(MOX::_player[player_idx].network_player_id, 38, nullptr, 0);
    }

    void __cdecl Accept_Diplomacy_Request_(int player_idx) {
        if (!((_net.players_active_mask >> player_idx) & 1)) {
            GENDRAW::Message_Box_(_ambassador_left_before_you_arrived_fmt);
            return;
        }

        _net.diplomacy_target_id = (int8_t)player_idx;
        _net.diplomacy_initiator_id = (int8_t)player_idx;

        uint8_t mask = ~(1 << player_idx);
        _net.players_active_mask &= mask;
        _net.players_ready_mask[0] &= mask;

        _net.sync_state = 0x2e /* 46, '.' */;
        _net.diplomacy_active_flag = 1;
        _net.is_diplomacy_initiator = 0;

        Mox_Send_Message_(
            MOX::_player[player_idx].network_player_id,
            0x1c,
            &MOX::_player[MOX::_PLAYER_NUM],
            0xea9 /* sizeof(s_player) */
        );

        while (_net.sync_state == 0x29 /* 41, ')' */ || _net.sync_state == 0x2e /* 46, '.' */) {
            RUSS::Mox_Update_();
        }

        if (Net_May_Begin_Human_Diplomacy_()) {
            DIP_SCRN::Diplomacy_Screen_(player_idx);
            Net_End_Human_Diplomacy_();
        } else {
            GENDRAW::Message_Box_(_ambassador_left_before_you_arrived_fmt);
            Net_Cancel_Human_Diplomacy_Request_();
        }
    }

    void __cdecl Update_() {
        sound::Sound_Update_();

        // Prevent recursive updates if currently processing messages
        if (NETMOX::_in_receive_message == 0) {
            netcode::Net_Update_();
        }
    }

    void __cdecl Russ_Event_Monster_Update_(int16_t param_1, int16_t param_2, int16_t param_3) {
        if (MOX::_game_type != 2 /* GAME_TYPE_NETWORK_HOST */ &&
            MOX::_game_type != 3 /* GAME_TYPE_NETWORK_CLIENT */) {
            EVENTS::Event_Monster_Update_(param_1, param_2, param_3);
            return;
        }

        s_net_pack_monster_update msg;
        msg.param_1 = static_cast<int32_t>(param_1);
        msg.param_2 = static_cast<int32_t>(param_2);
        msg.param_3 = static_cast<int32_t>(param_3);

        NETMOX::Mox_Send_Message_(_net.local_net_id, 0x3c /* MSG_TYPE_MONSTER_UPDATE */, &msg, sizeof(msg));
    }

    void __cdecl Send_Pick_Your_Position_(int16_t network_player_id) {
        allocate::Push_Block_(MOX::_screen_seg);

        // Allocation size 0x754a = 30026 bytes, enough for 8 players: 2 + (8 * 3753)
        s_net_pack_pick_your_position* packet = (s_net_pack_pick_your_position*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x754a);

        packet->num_players = MOX::_NUM_PLAYERS;

        // Copy the current player list into the network packet
        memcpy(packet->players, MOX::_player, (uint32_t)MOX::_NUM_PLAYERS * sizeof(s_player));

        uint32_t msg_size = sizeof(s_net_pack_pick_your_position);
        NETMOX::Mox_Send_Message_(network_player_id, 32 /* MSG_TYPE_PICK_YOUR_POSITION */, packet, (uint16_t)msg_size);

        allocate::Pop_Block_(MOX::_screen_seg);
    }

    void __cdecl Broadcast_Game_Data_Differential_(uint8_t use_diff) {
        allocate::Push_Block_(MOX::_screen_seg);
        uint8_t* block = static_cast<uint8_t*>(allocate::Allocate_Next_Block_(MOX::_screen_seg, 0xe1000));
        uint8_t* ptr = block;

        *reinterpret_cast<int16_t*>(ptr) = _net.host_election_id;
        ptr += sizeof(int16_t);

        *reinterpret_cast<uint32_t*>(ptr) = MOX::_stardate;
        ptr += sizeof(uint32_t);

        *ptr++ = MOX::_settings.random_events;
        *ptr++ = MOX::_settings.game_difficulty;
        *ptr++ = MOX::_settings.number_of_players;
        *ptr++ = MOX::_settings.galaxy_size;
        *ptr++ = MOX::_settings.galaxy_age;
        *ptr++ = MOX::_settings.strategic_combat_flag;
        *ptr++ = MOX::_settings.starting_civilization_level;
        *ptr++ = MOX::_settings.xenons_exist;

        RUSS::Mox_Update_();

        *reinterpret_cast<int32_t*>(ptr) = random::Get_Random_Seed_();
        ptr += sizeof(int32_t);

        *reinterpret_cast<uint16_t*>(ptr) = MOX::_NUM_PLAYERS;
        ptr += sizeof(uint16_t);

        uint32_t size = MOX::_NUM_PLAYERS * sizeof(s_player);
        memcpy(ptr, MOX::_player, size);
        ptr += size;

        *reinterpret_cast<uint16_t*>(ptr) = MOX::_NUM_STARS;
        ptr += sizeof(uint16_t);

        size = (MOX::_NUM_STARS + 1) * sizeof(s_star_data);
        memcpy(ptr, MOX::_star, size);
        ptr += size;

        *reinterpret_cast<uint16_t*>(ptr) = MOX::_planet_count;
        ptr += sizeof(uint16_t);

        size = MOX::_planet_count * sizeof(s_planet_data);
        memcpy(ptr, MOX::_planet, size);
        ptr += size;

        *reinterpret_cast<uint16_t*>(ptr) = MOX::_NUM_COLONIES;
        ptr += sizeof(uint16_t);

        size = 250 * sizeof(s_colony);
        memcpy(ptr, MOX::_colony, size);
        ptr += size;

        *reinterpret_cast<int16_t*>(ptr) = MOX::_NUM_SHIPS;
        ptr += sizeof(uint16_t);

        size = 500 * sizeof(s_ship_data);
        memcpy(ptr, MOX::_ship, size);
        ptr += size;

        RUSS::Mox_Update_();

        size = 71 * sizeof(s_leader_data);
        memcpy(ptr, MOX::_leaders, size);
        ptr += size;

        *reinterpret_cast<uint16_t*>(ptr) = REPORT::_n_reports;
        ptr += sizeof(uint16_t);

        size = 50 * sizeof(s_report);
        memcpy(ptr, REPORT::_reports, size);
        ptr += size;

        size = 36 * sizeof(s_event_data);
        memcpy(ptr, EVENTS::_event_data, size);
        ptr += size;

        size = MOX::_NUM_PLAYERS;
        memcpy(ptr, MSG_::_first_msg, size);
        ptr += size;

        memcpy(ptr, MSG_::_max_msgs, size);
        ptr += size;

        memcpy(ptr, MSG_::_n_msgs, size);
        ptr += size;

        size = 100 * sizeof(s_MSG);
        memcpy(ptr, MSG_::_msgs, size);
        ptr += size;

        *reinterpret_cast<uint16_t*>(ptr) = diplodef::_COUNCIL_FLAG;
        ptr += sizeof(uint16_t);

        *reinterpret_cast<uint16_t*>(ptr) = diplodef::_next_council_meeting_turn;
        ptr += sizeof(uint16_t);

        memcpy(ptr, &MOX::_antarans, sizeof(s_antaran));
        ptr += sizeof(s_antaran);

        *ptr++ = MOX::_NUM_NEBULAS;

        size = MOX::_NUM_NEBULAS * sizeof(s_nebula);
        memcpy(ptr, MOX::_nebula, size);
        ptr += size;

        *reinterpret_cast<uint32_t*>(ptr) = EVENTS::_last_event_year;
        ptr += sizeof(uint32_t);

        *reinterpret_cast<uint16_t*>(ptr) = EVENTS::_event_delay_count;
        ptr += sizeof(uint16_t);

        memcpy(ptr, MOX::_bill_savegame, 12);
        ptr += 12;

        RUSS::Mox_Update_();

        uint32_t block_size = ptr - block;
        uint8_t* send_ptr;
        int32_t send_size;
        s_net_modem_file* file;

        if (use_diff != 0) {
            file = Open_Modem_File_(s_wb_00551178, block_size);
            Write_Modem_File_(file, block, block_size);
            send_ptr = block;
            send_size = block_size;
        }
        else {
            file = Open_Modem_File_(s_rb_0055117c, 0);
            send_ptr = block + block_size;
            send_size = Make_Difference_Buffer_(file, block, block_size, send_ptr, 0xe1000 - block_size);
            Close_Modem_File_(file);

            file = Open_Modem_File_(s_wb_00551178, block_size);
            Write_Modem_File_(file, block, block_size);
        }

        RUSS::Mox_Update_();
        Close_Modem_File_(file);

        int16_t chunk_count = static_cast<int16_t>((send_size + 0x77ff) / 0x7800);
        Mox_Send_Message_(0x7ffe, 2, &chunk_count, sizeof(chunk_count));

        if (send_size > 0) {
            int16_t chunk_idx = 0;
            uint8_t* current_ptr = send_ptr;

            while (current_ptr < send_ptr + send_size) {
                uint32_t remaining = (send_ptr + send_size) - current_ptr;
                uint32_t chunk_size = remaining > 0x7800 ? 0x7800 : remaining;
                chunk_idx++;

                sprintf(MOX::_temp_string, _sending_chunk_d_of_d_fmt, chunk_idx, chunk_count);

                do {
                    video::Set_Page_Off_();
                    fields::Invoke_Auto_Function_();
                    RUSS::Mox_Sync_Update_();
                    RUSS::Mox_Update_();
                } while (netcode::Net_Is_Sending_() != 0);

                Mox_Send_Message_(0x7ffe, 3, current_ptr, static_cast<uint16_t>(chunk_size));
                current_ptr += 0x7800;
            }
        }

        allocate::Pop_Block_(MOX::_screen_seg);
    }

    int8_t __cdecl Decode_Broadcast_Game_Data_Differential_(void* buf, size_t size, uint8_t flag) {
        uint8_t* buffer = (uint8_t*)buf;
        uint8_t* ptr = (uint8_t*)buf;

        if (flag == 0) {
            ptr = buffer + size;
            s_net_modem_file* file_handle = NETMOX::Open_Modem_File_(NETMOX::s_rb_0055117c, 0);
            size = NETMOX::Decode_Difference_Buffer_(file_handle, ptr, 0xE1000 - size, buffer, size);
            NETMOX::Close_Modem_File_(file_handle);

            if (size == 0) {
                exitm::Exit_With_message2_(NETMOX::s_error_decoding_broadcast_0055ad58);
                return 0;
            }
        }

        s_net_modem_file* file_handle = NETMOX::Open_Modem_File_(NETMOX::s_wb_00551178, size);
        NETMOX::Write_Modem_File_(file_handle, ptr, size);
        NETMOX::Close_Modem_File_(file_handle);

        RUSS::Mox_Update_();

        _net.host_election_id = *reinterpret_cast<int16_t*>(ptr);
        ptr += sizeof(int16_t);

        MOX::_stardate = *reinterpret_cast<uint32_t*>(ptr);
        ptr += sizeof(uint32_t);

        MOX::_settings.random_events = *ptr;
        ptr += sizeof(uint8_t);

        MOX::_settings.game_difficulty = *ptr;
        ptr += sizeof(uint8_t);

        MOX::_settings.number_of_players = *ptr;
        ptr += sizeof(uint8_t);

        MOX::_settings.galaxy_size = *ptr;
        ptr += sizeof(uint8_t);

        MOX::_settings.galaxy_age = *ptr;
        ptr += sizeof(uint8_t);

        MOX::_settings.strategic_combat_flag = *ptr;
        ptr += sizeof(uint8_t);

        MOX::_settings.starting_civilization_level = *ptr;
        ptr += sizeof(uint8_t);

        MOX::_settings.xenons_exist = *ptr;
        ptr += sizeof(uint8_t);

        random::Set_Random_Seed_(*reinterpret_cast<uint32_t*>(ptr));
        ptr += sizeof(uint32_t);

        RUSS::Mox_Update_();

        MOX::_NUM_PLAYERS = *reinterpret_cast<int16_t*>(ptr);
        ptr += sizeof(int16_t);

        memcpy(MOX::_player, ptr, MOX::_NUM_PLAYERS * sizeof(s_player));
        ptr += MOX::_NUM_PLAYERS * sizeof(s_player);

        _net.num_connected_players = 0;
        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; i++) {
            if (MOX::_player[i].eliminated != 0) {
                MOX::_player[i].network_player_id = -1;
                MOX::_player[i].objectives = 0;
            } else {
                if (MOX::_player[i].objectives == 'd') {
                    _net.num_connected_players++;
                }
            }
        }

        MOX::_NUM_STARS = *reinterpret_cast<int16_t*>(ptr);
        ptr += sizeof(int16_t);

        memcpy(MOX::_star, ptr, (MOX::_NUM_STARS + 1) * sizeof(s_star_data));
        ptr += (MOX::_NUM_STARS + 1) * sizeof(s_star_data);

        RUSS::Mox_Update_();

        MOX::_planet_count = *reinterpret_cast<int16_t*>(ptr);
        ptr += sizeof(int16_t);

        memcpy(MOX::_planet, ptr, MOX::_planet_count * sizeof(s_planet_data));
        ptr += MOX::_planet_count * sizeof(s_planet_data);

        MOX::_NUM_COLONIES = *reinterpret_cast<int16_t*>(ptr);
        ptr += sizeof(int16_t);

        memcpy(MOX::_colony, ptr, 250 * sizeof(s_colony));
        ptr += 250 * sizeof(s_colony);

        MOX::_NUM_SHIPS = *reinterpret_cast<int16_t*>(ptr);
        ptr += sizeof(int16_t);

        memcpy(MOX::_ship, ptr, 500 * sizeof(s_ship_data));
        ptr += 500 * sizeof(s_ship_data);

        memcpy(MOX::_leaders, ptr, 67 * sizeof(s_leader_data));
        ptr += 67 * sizeof(s_leader_data);

        REPORT::_n_reports = *reinterpret_cast<int16_t*>(ptr);
        ptr += sizeof(int16_t);

        memcpy(REPORT::_reports, ptr, 50 * sizeof(s_report));
        ptr += 50 * sizeof(s_report);

        REPORT::Client_Initialize_Reports_();

        memcpy(EVENTS::_event_data, ptr, 36 * sizeof(s_event_data));
        ptr += 36 * sizeof(s_event_data);

        memcpy(MSG_::_first_msg, ptr, MOX::_NUM_PLAYERS);
        ptr += MOX::_NUM_PLAYERS;

        memcpy(MSG_::_max_msgs, ptr, MOX::_NUM_PLAYERS);
        ptr += MOX::_NUM_PLAYERS;

        memcpy(MSG_::_n_msgs, ptr, MOX::_NUM_PLAYERS);
        ptr += MOX::_NUM_PLAYERS;

        memcpy(MSG_::_msgs, ptr, 100 * sizeof(s_MSG));
        ptr += 100 * sizeof(s_MSG);

        RUSS::Mox_Update_();

        diplodef::_COUNCIL_FLAG = *reinterpret_cast<int16_t*>(ptr);
        ptr += sizeof(int16_t);

        diplodef::_next_council_meeting_turn = *reinterpret_cast<int16_t*>(ptr);
        ptr += sizeof(int16_t);

        memcpy(&MOX::_antarans, ptr, sizeof(s_antaran));
        ptr += sizeof(s_antaran);

        MOX::_NUM_NEBULAS = *ptr;
        ptr += sizeof(uint8_t);

        memcpy(MOX::_nebula, ptr, MOX::_NUM_NEBULAS * sizeof(s_nebula));
        ptr += MOX::_NUM_NEBULAS * sizeof(s_nebula);

        EVENTS::_last_event_year = *reinterpret_cast<uint32_t*>(ptr);
        ptr += sizeof(uint32_t);

        EVENTS::_event_delay_count = *reinterpret_cast<int16_t*>(ptr);
        ptr += sizeof(int16_t);

        memcpy(MOX::_bill_savegame, ptr, 12);

        RUSS::Mox_Update_();

        return 1;
    }

    void __cdecl Mox_Net_Update_() {
        if (MOX::_game_type != 2 && MOX::_game_type != 3) {
            return;
        }

        if (NETMOX::_is_updating || NETMOX::_in_receive_message) {
            return;
        }

        NETMOX::_is_updating = 1;

        netcode::Net_Update_();

        if (!NETMOX::_net.is_network_active) {
            NETMOX::_is_updating = 0;
            return;
        }

        int current_player_idx;
        if (MOX::_NUM_PLAYERS != 0) {
            current_player_idx = NETMOX::_next_update_player_counter % MOX::_NUM_PLAYERS;
        } else {
            current_player_idx = -1;
        }

        RUSS::Mox_Update_();

        NETMOX::_next_update_player_counter++;

        if (current_player_idx == -1 || netcode::Net_Is_Sending_()) {
            NETMOX::_is_updating = 0;
            return;
        }

        bool message_sent = false;

        uint8_t* player_phase_ready_sent_flags = reinterpret_cast<uint8_t*>(&NETMOX::_net.field_1db);

        if ((NETMOX::_net.sync_state == 4 || NETMOX::_net.sync_state == 8) && 
            !player_phase_ready_sent_flags[current_player_idx]) {
            
            player_phase_ready_sent_flags[current_player_idx] = 1;
            if (MOX::_player[current_player_idx].network_player_id != -1) {
                message_sent = true;
                NETMOX::Mox_Send_Message_(MOX::_player[current_player_idx].network_player_id, 0x36, nullptr, 0);
            }
        }

        if (!message_sent && (NETMOX::_net.ai_diplomacy_done_mask & (1 << current_player_idx))) {
            int8_t remote_player_id = NETMOX::_net.player_net_info[current_player_idx].remote_player_id;
            NETMOX::_net.ai_diplomacy_done_mask &= ~(1 << current_player_idx);
            message_sent = true;
            NETMOX::Mox_Send_Message_(MOX::_player[current_player_idx].network_player_id, 7, &remote_player_id, 1);
        }

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (NETMOX::_net.players_sync_mask[i] & (1 << current_player_idx)) {
                s_NET_MSG_SYNC_PLAYER sync_pkt;
                
                sync_pkt.player1_idx = i;
                memcpy(&sync_pkt.player1, &MOX::_player[i], sizeof(s_player));

                for (int star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
                    sync_pkt.surrender_to[star_idx] = MOX::_star[star_idx].surrender_to[i];
                }

                int32_t sync_target_idx = NETMOX::_net.diplomacy_sync_mask[i];
                sync_pkt.player2_idx = sync_target_idx;
                memcpy(&sync_pkt.player2, &MOX::_player[sync_target_idx], sizeof(s_player));

                if (MOX::_PLAYER_NUM != current_player_idx && sync_target_idx != current_player_idx) {
                    if (MOX::_player[current_player_idx].objectives == 'd' &&
                        !message_sent && NETMOX::_net.player_race_selected_flags[current_player_idx]) {
                        
                        message_sent = true;
                        NETMOX::Mox_Send_Message_(MOX::_player[current_player_idx].network_player_id, 0x22, &sync_pkt, 0x1DA2);
                    }
                }
                NETMOX::_net.players_sync_mask[i] &= ~(1 << current_player_idx);
            }
        }

        if (!message_sent) {
            if (NETMOX::_net.ai_diplomacy_active_mask & (1 << current_player_idx)) {
                int8_t remote_player_id = NETMOX::_net.player_net_info[current_player_idx].remote_player_id;
                if (!(NETMOX::_net.players_sync_mask[remote_player_id] & (1 << current_player_idx))) {
                    message_sent = true;
                    NETMOX::_net.ai_diplomacy_active_mask &= ~(1 << current_player_idx);
                    NETMOX::Mox_Send_Message_(MOX::_player[current_player_idx].network_player_id, 6, &remote_player_id, 1);
                }
            }
        }

        if (!message_sent) {
            if (NETMOX::_net.ai_diplomacy_cancel_mask & (1 << current_player_idx)) {
                int8_t remote_player_id = NETMOX::_net.player_net_info[current_player_idx].remote_player_id;
                NETMOX::_net.ai_diplomacy_cancel_mask &= ~(1 << current_player_idx);
                message_sent = true;
                NETMOX::Mox_Send_Message_(MOX::_player[current_player_idx].network_player_id, 9, &remote_player_id, 1);
            }
        }

        if (!message_sent) {
            if (NETMOX::_net.human_diplomacy_ignored_mask & (1 << current_player_idx)) {
                message_sent = true;
                NETMOX::_net.human_diplomacy_ignored_mask &= ~(1 << current_player_idx);
                NETMOX::Mox_Send_Message_(MOX::_player[current_player_idx].network_player_id, 0x1D, nullptr, 0);
            }
        }

        if (!message_sent) {
            int32_t player_num = MOX::_PLAYER_NUM;
            uint8_t* player_rank_sync_flags = reinterpret_cast<uint8_t*>(&NETMOX::_net.field134_0x1b6);
            
            if (current_player_idx != player_num && MOX::_player[current_player_idx].objectives == 'd') {
                int8_t local_rank = NETMOX::_net.host_election_player_rank[player_num];
                if (player_rank_sync_flags[current_player_idx] != local_rank) {
                    message_sent = true;
                    s_NET_MSG_RANK rank_msg;
                    rank_msg.rank = (int32_t)local_rank;
                    rank_msg.hit_next_turn = NETMOX::_net.players_hit_next_turn[player_num];
                    player_rank_sync_flags[current_player_idx] = local_rank;
                    NETMOX::Mox_Send_Message_(MOX::_player[current_player_idx].network_player_id, 4, &rank_msg, 5);
                }
            }
        }

        if (!message_sent) {
            if (NETMOX::_net.local_net_id == NETMOX::_net.net_host_id &&
                NETMOX::_net.sync_state >= 0xE && NETMOX::_net.sync_state < 0x23) {
                COMBFIND::Do_Net_Combat_Update_(message_sent);
            }
        }

        if (!message_sent) {
            if (NETMOX::_net.player_color_selected_flags[current_player_idx]) {
                message_sent = true;
                NETMOX::_net.player_color_selected_flags[current_player_idx] = 0;
                s_NET_MSG_COLOR color_msg;
                color_msg.color = MOX::_player[current_player_idx].color;
                color_msg.player_idx = (int8_t)current_player_idx;
                NETMOX::Mox_Send_Message_(0x7FFE, 0x2B, &color_msg, 2);
            }
        }

        if (!message_sent) {
            if (NETMOX::_net.player_race_selected_alt_flags[current_player_idx]) {
                message_sent = true;
                NETMOX::_net.player_race_selected_alt_flags[current_player_idx] = 0;
                s_NET_MSG_RACE race_msg;
                race_msg.race = MOX::_player[current_player_idx].race;
                race_msg.player_idx = (int8_t)current_player_idx;
                NETMOX::Mox_Send_Message_(0x7FFE, 0x39, &race_msg, 2);
            }
        }

        if (!message_sent) {
            if (NETMOX::_net.player_race_deselected_flags[current_player_idx]) {
                message_sent = true;
                NETMOX::_net.player_race_deselected_flags[current_player_idx] = 0;
                uint8_t* player_race_selection_sync_flags = reinterpret_cast<uint8_t*>(&NETMOX::_net.field146_0x1f3);
                player_race_selection_sync_flags[current_player_idx] = 1;
                uint8_t race = MOX::_player[current_player_idx].race;
                NETMOX::Mox_Send_Message_(0x7FFE, 0x3D, &race, 1);
            }
        }

        if (!message_sent) {
            uint8_t* player_race_selection_sync_flags = reinterpret_cast<uint8_t*>(&NETMOX::_net.field146_0x1f3);
            if (player_race_selection_sync_flags[current_player_idx]) {
                message_sent = true;
                player_race_selection_sync_flags[current_player_idx] = 0;
                NETMOX::Mox_Send_Message_(MOX::_player[current_player_idx].network_player_id, 0x3F, nullptr, 0);
            }
        }

        if (!message_sent) {
            if (NETMOX::_net.diplomacy_initiator_id != -1) {
                int8_t target_idx = NETMOX::_net.diplomacy_target_id;
                NETMOX::_net.diplomacy_initiator_id = -1;
                NETMOX::Mox_Send_Message_(MOX::_player[target_idx].network_player_id, 0x44, &MOX::_player[MOX::_PLAYER_NUM], sizeof(s_player));
            }
        }

        NETMOX::_is_updating = 0;
    }

    void __cdecl Send_Game_Orders_() {
        allocate::Push_Block_(MOX::_screen_seg);
        uint8_t* packet = reinterpret_cast<uint8_t*>(allocate::Allocate_Next_Block_(MOX::_screen_seg, 350000));

        uint8_t* out = packet;
        *reinterpret_cast<uint32_t*>(out) = MOX::_stardate;
        out += sizeof(uint32_t);

        *out++ = static_cast<uint8_t>(MOX::_PLAYER_NUM);

        memcpy(out, &MOX::_player[MOX::_PLAYER_NUM], sizeof(s_player));
        out += sizeof(s_player);

        RUSS::Mox_Update_();

        s_net_pack_star_order star_orders[72];
        memset(star_orders, 0, sizeof(star_orders));

        if (MOX::_NUM_STARS > 0) {
            for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
                s_net_pack_star_order* order = &star_orders[i];

                order->flags =
                    (((MOX::_star[i].ignore_colony_ships >> MOX::_PLAYER_NUM) & 1) << 0) |
                    (((MOX::_star[i].ignore_combat_ships >> MOX::_PLAYER_NUM) & 1) << 1) |
                    (((MOX::_star[i].has_colony >> MOX::_PLAYER_NUM) & 1) << 2) |
                    (((MOX::_star[i].has_warp_field_interdictor >> MOX::_PLAYER_NUM) & 1) << 3) |
                    (((MOX::_star[i].has_tachyon >> MOX::_PLAYER_NUM) & 1) << 4) |
                    (((MOX::_star[i].has_subspace >> MOX::_PLAYER_NUM) & 1) << 5) |
                    (((MOX::_star[i].has_stargate >> MOX::_PLAYER_NUM) & 1) << 6) |
                    (((MOX::_star[i].has_jumpgate >> MOX::_PLAYER_NUM) & 1) << 7) |
                    (((MOX::_star[i].has_artemis_net >> MOX::_PLAYER_NUM) & 1) << 8) |
                    (((MOX::_star[i].has_dimensional_portal >> MOX::_PLAYER_NUM) & 1) << 9);

                order->officer_index = MOX::_star[i].officer_index[MOX::_PLAYER_NUM];

                if (MOX::_star[i].colonize_player == MOX::_PLAYER_NUM) {
                    strcpy(order->name, MOX::_star[i].name);
                }

                order->surrender_to = MOX::_star[i].surrender_to[MOX::_PLAYER_NUM];
            }
        }

        memcpy(out, star_orders, MOX::_NUM_STARS * sizeof(s_net_pack_star_order));
        out += MOX::_NUM_STARS * sizeof(s_net_pack_star_order);

        RUSS::Mox_Update_();

        *reinterpret_cast<int16_t*>(out) = (int16_t)MOX::_NUM_COLONIES;
        out += sizeof(int16_t);

        if (MOX::_NUM_COLONIES > 0) {
            for (int16_t i = 0; i < (int16_t)MOX::_NUM_COLONIES; ++i) {
                if (MOX::_colony[i].owner == MOX::_PLAYER_NUM) {
                    *reinterpret_cast<int16_t*>(out) = i;
                    out += sizeof(int16_t);
                    memcpy(out, &MOX::_colony[i], sizeof(s_colony));
                    out += sizeof(s_colony);
                }
            }
        }

        *reinterpret_cast<int16_t*>(out) = -1;
        out += sizeof(int16_t);

        int16_t* p_num_ships = reinterpret_cast<int16_t*>(out);
        out += sizeof(int16_t);

        RUSS::Mox_Update_();

        *p_num_ships = (int16_t)MOX::_NUM_SHIPS;

        if (MOX::_NUM_SHIPS > 0) {
            for (int16_t i = 0; i < (int16_t)MOX::_NUM_SHIPS; ++i) {
                if (MOX::_ship[i].owner == MOX::_PLAYER_NUM && MOX::_ship[i].status != 5) {
                    *reinterpret_cast<int16_t*>(out) = i;
                    out += sizeof(int16_t);
                    memcpy(out, &MOX::_ship[i], sizeof(s_ship_data));
                    out += sizeof(s_ship_data);
                }
            }
        }

        *reinterpret_cast<int16_t*>(out) = -1;
        out += sizeof(int16_t);

        RUSS::Mox_Update_();

        for (int16_t i = 0; i < 67 /* 0x43 */; ++i) {
            if (MOX::_leaders[i].player_index == MOX::_PLAYER_NUM) {
                *reinterpret_cast<int16_t*>(out) = i;
                out += sizeof(int16_t);
                memcpy(out, &MOX::_leaders[i], sizeof(s_leader_data));
                out += sizeof(s_leader_data);
            }
        }

        *reinterpret_cast<int16_t*>(out) = -1;
        out += sizeof(int16_t);

        RUSS::Mox_Update_();

        uint16_t num_chunks = static_cast<uint16_t>((out - packet + 0x77FF) / 0x7800);

        NETMOX::Mox_Send_Message_(_net.local_net_id, 0x0D, &num_chunks, sizeof(uint16_t));

        uint8_t* chunk_ptr = packet;
        int chunk_idx = 0;

        while (chunk_ptr < out) {
            RUSS::Mox_Update_();

            int32_t chunk_size = out - chunk_ptr;
            if (chunk_size > 0x7800) {
                chunk_size = 0x7800;
            }

            if (num_chunks - chunk_idx == 1) {
                _net.sync_state = 0x0E;
                _net.combat_attacker_id = -1;
            }

            chunk_idx++;

            sprintf(MOX::_temp_string, NETMOX::_sending_chunk_d_of_d_fmt, chunk_idx, num_chunks);

            do {
                MULTPLAY::Chat_Box_Input_Loop_();
            } while (netcode::Net_Is_Sending_() != 0);

            NETMOX::Mox_Send_Message_(_net.local_net_id, 0x0E, chunk_ptr, (uint16_t)chunk_size);
            chunk_ptr += 0x7800;
        }

        allocate::Pop_Block_(MOX::_screen_seg);
    }

    void __cdecl Send_Drop_Out_Info_(int player_idx, uint16_t msg_type) {
        uint8_t any_processed;
        uint8_t player_process_flags[8];
        s_net_pack_drop_out_info packet;

        MOX::_player[player_idx].network_player_id = -1;
        RUSS::Make_Player_Into_AI_(player_idx);

        memcpy(&packet.player_data, &MOX::_player[player_idx], sizeof(s_player));

        if (MOX::_NUM_STARS > 0) {
            for (int i = 0; i < MOX::_NUM_STARS; ++i) {
                packet.star_surrender_to[i] = MOX::_star[i].surrender_to[player_idx];
            }
        }

        memset(player_process_flags, 1, MOX::_NUM_PLAYERS);
        player_process_flags[MOX::_PLAYER_NUM] = 0;

        do {
            any_processed = 0;
            if (MOX::_NUM_PLAYERS > 0) {
                for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                    if (player_process_flags[i] != 0) {
                        any_processed = 1;
                        if (MOX::_player[i].network_player_id == -1) {
                            player_process_flags[i] = 0;
                        } else if (_net.player_race_selected_flags[i] != 0) {
                            Mox_Send_Message_(MOX::_player[i].network_player_id, msg_type, &packet, sizeof(s_net_pack_drop_out_info));
                            do {
                                RUSS::Mox_Update_();
                            } while (netcode::Net_Is_Sending_() != 0);
                            player_process_flags[i] = 0;
                        }
                    }
                }
            }
        } while (any_processed != 0);
    }

    int __cdecl Decode_Difference_Buffer_(s_net_modem_file* modem_file, uint8_t* out_buffer, int max_out_len, uint8_t* diff_buffer, int diff_len) {
        uint8_t* out_ptr = out_buffer;
        uint8_t* diff_end = diff_buffer + diff_len;

        if (fseek_modem_file_(modem_file, 0, 0) != 0) {
            exitm::Exit_With_message2_(s_fseek_modem_file_0055adb4);
            return 0;
        }

        while (true) {
            if (diff_buffer >= diff_end) {
                return static_cast<int>(out_ptr - out_buffer);
            }

            uint8_t b = *diff_buffer;
            if (b <= 0xF4) {
                uint32_t size = b;
                if (fread_modem_file_(out_ptr, size, 1, modem_file) != 1) {
                    exitm::Exit_With_message2_(s_fread_modem_file_0055ada0);
                    return 0;
                }
                diff_buffer++;
                out_ptr += size;
                RUSS::Mox_Update_();
            } else if (b == 0xFF) {
                diff_buffer++;
                if (diff_buffer > diff_end || out_buffer + max_out_len < out_ptr) {
                    exitm::Exit_With_message2_(s_p_limit_p_diff_0055ad74);
                    return 0;
                }
            } else {
                uint32_t skip = b - 0xF5;
                if (fseek_modem_file_(modem_file, skip, 1) != 0) {
                    exitm::Exit_With_message2_(s_fseek_ahead_modem_file_0055ad88);
                    return 0;
                }
                memcpy(out_ptr, diff_buffer + 1, skip);
                diff_buffer += skip + 1;
                out_ptr += skip;
                RUSS::Mox_Update_();
            }
        }
    }

    size_t __cdecl fread_modem_file_(void* ptr, size_t size, size_t count, s_net_modem_file* modem_file) {
        if (modem_file->mode == 1) {
            return fread(ptr, size, count, modem_file->file);
        }

        if (modem_file->mode != 2) {
            return 0;
        }

        size_t total_size = count * size;
        memcpy(ptr, modem_file->current_ptr, total_size);

        modem_file->current_ptr += total_size;

        if (modem_file->buffer + modem_file->buffer_size < modem_file->current_ptr) {
            return 0;
        }

        return count;
    }

    size_t __cdecl fwrite_modem_file_(const void* ptr, size_t size, size_t count, s_net_modem_file* stream) {
        if (stream->mode == 1) {
            return fwrite(ptr, size, count, stream->file);
        }

        if (stream->mode == 2) {
            size_t total_size = count * size;

            // BUG: potential overflow if total_size > (buffer + buffer_size - current_ptr)
            memcpy(stream->current_ptr, ptr, total_size);

            uint8_t* new_ptr = stream->current_ptr + total_size;
            stream->current_ptr = new_ptr;

            if (stream->buffer + stream->buffer_size < new_ptr) {
                return 0;
            }

            return count;
        }

        return 0;
    }

    int32_t __cdecl fgetc_modem_file_(s_net_modem_file* modem_file) {
        if (modem_file->mode == 1) {
            return fgetc(modem_file->file);
        }

        if (modem_file->mode == 2) {
            if (modem_file->buffer + modem_file->buffer_size == modem_file->current_ptr) {
                return -1;
            }
            uint8_t val = *modem_file->current_ptr;
            modem_file->current_ptr++;
            return val;
        }

        return 0;
    }

    int32_t __cdecl Make_Difference_Buffer_(s_net_modem_file* file, uint8_t* buffer, int32_t buffer_size, uint8_t* diff_buffer, int32_t diff_buffer_max_size) {
        uint32_t count = 0;
        uint32_t state = 0;
        uint8_t* ptr = buffer;
        uint8_t* out_ptr = diff_buffer;

        if (NETMOX::fseek_modem_file_(file, 0, 0) != 0) {
            exitm::Exit_With_message2_(NETMOX::_modem_disk_error);
        }

        do {
            int c = NETMOX::fgetc_modem_file_(file);
            if (c == -1 && ptr != buffer + buffer_size) {
                exitm::Exit_With_message2_(NETMOX::_modem_disk_error);
            }

            switch (state) {
            case 0:
                if (c == -1) {
                    state = 4;
                } else if (*ptr == c) {
                    state = 1;
                    count = 1;
                } else {
                    state = 2;
                    count = 1;
                }
                break;

            case 1:
                if (*ptr == c) {
                    count++;
                    if (count == 0xF4) {
                        *out_ptr++ = 0xF4;
                        state = 0;
                    }
                } else {
                    *out_ptr++ = static_cast<uint8_t>(count);
                    if (c == -1) {
                        state = 4;
                    } else {
                        state = 2;
                        count = 1;
                    }
                }
                break;

            case 2:
                if (c == -1) {
                    *out_ptr++ = static_cast<uint8_t>(count - 11);
                    memcpy(out_ptr, ptr - count, count);
                    out_ptr += count;
                    state = 4;
                } else {
                    count++;
                    if (count == 9) {
                        *out_ptr++ = 0xFE;
                        memcpy(out_ptr, ptr - 8, 9);
                        out_ptr += 9;
                        state = 0;
                    } else if (*ptr == c) {
                        state = 3;
                    }
                }
                break;

            case 3:
                if (c == -1) {
                    *out_ptr++ = static_cast<uint8_t>(count - 11);
                    memcpy(out_ptr, ptr - count, count);
                    out_ptr += count;
                    state = 4;
                } else if (*ptr == c) {
                    *out_ptr++ = static_cast<uint8_t>(count - 12);
                    memcpy(out_ptr, ptr - count, count - 1);
                    out_ptr += count - 1;
                    count = 2;
                    state = 1;
                } else {
                    count++;
                    if (count == 9) {
                        *out_ptr++ = 0xFE;
                        memcpy(out_ptr, ptr - 8, 9);
                        out_ptr += 9;
                        state = 0;
                    } else {
                        state = 2;
                    }
                }
                break;
            }

            if (out_ptr >= diff_buffer + diff_buffer_max_size) {
                exitm::Exit_With_message2_(NETMOX::_modem_diff_buffer_error);
            }

            ptr++;

            if (((ptr - buffer) % 256) == 0) {
                RUSS::Mox_Update_();
            }
        } while (state != 4);

        *out_ptr = 0xFF;
        return static_cast<int32_t>(out_ptr + 1 - diff_buffer);
    }

    void __cdecl Write_Modem_File_(s_net_modem_file* stream, const void* buffer, size_t size) {
        RUSS::Mox_Update_();

        if (fseek_modem_file_(stream, 0, 0) != 0) {
            exitm::Exit_With_message2_(_modem_disk_error);
        }

        RUSS::Mox_Update_();

        if (fwrite_modem_file_(buffer, size, 1, stream) != 1) {
            exitm::Exit_With_message2_(_modem_disk_error);
        }

        RUSS::Mox_Update_();
    }

    void __cdecl Net_Request_Human_Diplomacy_(int16_t target_player_id, int16_t dummy) {
        _net.diplomacy_target_id = static_cast<int8_t>(target_player_id);

        if ((_net.players_active_mask & (1 << target_player_id)) != 0) {
            _net.sync_state = 0x2E; /* '.' */
            _net.diplomacy_active_flag = 1;
            _net.diplomacy_initiator_id = static_cast<int8_t>(target_player_id);

            _net.players_active_mask &= static_cast<uint8_t>(~(1 << target_player_id));
            _net.players_ready_mask[0] &= static_cast<uint8_t>(~(1 << target_player_id));
            _net.is_diplomacy_initiator = 0;

            Mox_Send_Message_(
                MOX::_player[target_player_id].network_player_id,
                0x1C,
                &MOX::_player[MOX::_PLAYER_NUM],
                0xEA9 /* sizeof(s_player) */
            );
        } else {
            _net.sync_state = 0x29; /* ')' */
            _net.diplomacy_active_flag = 0;

            if (Human_Hit_Next_Turn_(target_player_id) == 0) {
                s_net_pack_req_human_diplomacy packet;

                packet.player = MOX::_player[MOX::_PLAYER_NUM];

                for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
                    packet.surrender_to[i] = MOX::_star[i].surrender_to[MOX::_PLAYER_NUM];
                }

                Mox_Send_Message_(
                    MOX::_player[target_player_id].network_player_id,
                    0x1B,
                    &packet,
                    0xEF1 /* sizeof(s_packet_1B) */
                );
            }
        }
    }

    void __cdecl Close_Modem_File_(s_net_modem_file* file_handle) {
        RUSS::Mox_Update_();

        if (file_handle->mode == 1) {
            if (fclose(file_handle->file) != 0) {
                exitm::Exit_With_message2_(_modem_disk_error);
            }
            file_handle->file = nullptr;
            file_handle->mode = 0;
        }
    }

    int __cdecl fseek_modem_file_(s_net_modem_file* mod_file, int offset, int origin) {
        if (mod_file->mode == 1) {
            return fseek(mod_file->file, offset, origin);
        }

        if (mod_file->mode == 2) {
            if (origin == 0 /* SEEK_SET */) {
                mod_file->current_ptr = mod_file->buffer + offset;
                return 0;
            }
            if (origin == 1 /* SEEK_CUR */) {
                mod_file->current_ptr = mod_file->current_ptr + offset;
                return 0;
            }
        }

        return 0;
    }

    void __cdecl Decode_Game_Orders_(void* data, uint32_t len) {
        (void)len;
        uint8_t player_idx = *(uint8_t*)((uint8_t*)data + 4);
        uint8_t* ptr = (uint8_t*)data + 5;

        memcpy(&MOX::_player[player_idx], ptr, sizeof(s_player));
        ptr += sizeof(s_player);

        RUSS::Mox_Update_();

        uint8_t star_updates[1704]; // Supports up to 71 stars locally
        memcpy(star_updates, ptr, MOX::_NUM_STARS * 24);
        ptr += MOX::_NUM_STARS * 24;

        if (MOX::_NUM_STARS > 0) {
            for (int i = 0; i < MOX::_NUM_STARS; ++i) {
                uint8_t* update = star_updates + i * 24;
                uint32_t flags = *(uint32_t*)update;
                s_star_data* star = &MOX::_star[i];

                if (flags & 0x1) star->ignore_colony_ships |= (1 << player_idx);
                else star->ignore_colony_ships &= ~(1 << player_idx);

                if (flags & 0x2) star->ignore_combat_ships |= (1 << player_idx);
                else star->ignore_combat_ships &= ~(1 << player_idx);

                if (flags & 0x4) star->has_colony |= (1 << player_idx);
                else star->has_colony &= ~(1 << player_idx);

                if (flags & 0x8) star->has_warp_field_interdictor |= (1 << player_idx);
                else star->has_warp_field_interdictor &= ~(1 << player_idx);

                if (flags & 0x10) star->has_tachyon |= (1 << player_idx);
                else star->has_tachyon &= ~(1 << player_idx);

                if (flags & 0x20) star->has_subspace |= (1 << player_idx);
                else star->has_subspace &= ~(1 << player_idx);

                if (flags & 0x40) star->has_stargate |= (1 << player_idx);
                else star->has_stargate &= ~(1 << player_idx);

                if (flags & 0x80) star->has_jumpgate |= (1 << player_idx);
                else star->has_jumpgate &= ~(1 << player_idx);

                if (flags & 0x100) star->has_artemis_net |= (1 << player_idx);
                else star->has_artemis_net &= ~(1 << player_idx);

                if (flags & 0x200) star->has_dimensional_portal |= (1 << player_idx);
                else star->has_dimensional_portal &= ~(1 << player_idx);

                star->officer_index[player_idx] = update[0x13];

                if (star->colonize_player == player_idx) {
                    strcpy(star->name, (const char*)(update + 4));
                }

                star->surrender_to[player_idx] = update[0x17];
            }
        }

        RUSS::Mox_Update_();

        uint16_t max_colonies = *(uint16_t*)ptr;
        ptr += 2;
        if (MOX::_NUM_COLONIES < max_colonies) {
            MOX::_NUM_COLONIES = max_colonies;
        }

        while (true) {
            uint16_t colony_idx = *(uint16_t*)ptr;
            ptr += 2;
            if (colony_idx == 0xFFFF) {
                break;
            }

            memcpy(&MOX::_colony[colony_idx], ptr, sizeof(s_colony));
            ptr += sizeof(s_colony);

            int16_t planet_idx = MOX::_colony[colony_idx].planet;
            if (MOX::_planet[planet_idx].colony_index != colony_idx) {
                MOX::_planet[planet_idx].colony_index = colony_idx;
            }

            RUSS::Mox_Update_();
        }

        RUSS::Mox_Update_();

        uint16_t max_ships = *(uint16_t*)ptr;
        ptr += 2;
        if (MOX::_NUM_SHIPS < max_ships) {
            MOX::_NUM_SHIPS = max_ships;
        }

        int ship_idx_prev = 0;
        while (true) {
            uint16_t ship_idx = *(uint16_t*)ptr;
            ptr += 2;
            if (ship_idx == 0xFFFF) {
                break;
            }

            for (int i = ship_idx_prev; i < ship_idx; ++i) {
                if (MOX::_ship[i].owner == player_idx && MOX::_ship[i].status != 5) {
                    MOX::_ship[i].officer_index = -1;
                    SHIPS::Kill_Ship_(i);
                }
            }

            memcpy(&MOX::_ship[ship_idx], ptr, sizeof(s_ship_data));
            ptr += sizeof(s_ship_data);

            ship_idx_prev = ship_idx + 1;
            RUSS::Mox_Update_();
        }

        for (int i = ship_idx_prev; i < MOX::_NUM_SHIPS; ++i) {
            if (MOX::_ship[i].owner == player_idx && MOX::_ship[i].status != 5) {
                MOX::_ship[i].officer_index = -1;
                SHIPS::Kill_Ship_(i);
            }
        }

        RUSS::Mox_Update_();

        int leader_idx_prev = 0;
        while (true) {
            uint16_t leader_idx = *(uint16_t*)ptr;
            ptr += 2;
            if (leader_idx == 0xFFFF) {
                break;
            }

            for (int i = leader_idx_prev; i < leader_idx; ++i) {
                if (MOX::_leaders[i].player_index == player_idx) {
                    NETMOX::Ditch_Leader_(i);
                }
            }

            memcpy(&MOX::_leaders[leader_idx], ptr, sizeof(s_leader_data));
            ptr += sizeof(s_leader_data);

            leader_idx_prev = leader_idx + 1;
        }

        for (int i = leader_idx_prev; i < 67; ++i) {
            if (MOX::_leaders[i].player_index == player_idx) {
                NETMOX::Ditch_Leader_(i);
            }
        }

        RUSS::Mox_Update_();
    }

    s_net_modem_file* __cdecl Open_Modem_File_(char* mode_str, uint32_t size) {
        RUSS::Mox_Update_();

        if (_secret_modem_file.mode == 2 && size != 0 && size != _secret_modem_file.buffer_size) {
            _free(_secret_modem_file.buffer);
            _secret_modem_file.mode = 0;
        }

        if (_secret_modem_file.mode == 0) {
            _secret_modem_file.buffer = static_cast<uint8_t*>(_malloc(size));
            if (_secret_modem_file.buffer != nullptr) {
                _secret_modem_file.buffer_size = size;
                _secret_modem_file.current_ptr = _secret_modem_file.buffer;
                _secret_modem_file.mode = 2;
                return &_secret_modem_file;
            }

            _secret_modem_file.file = fopen(s_modem_tmp_0055ad4c, mode_str);
            if (_secret_modem_file.file == nullptr) {
                exitm::Exit_With_message2_(_modem_disk_error);
                return &_secret_modem_file;
            }

            _secret_modem_file.mode = 1;
        }

        return &_secret_modem_file;
    }

    int8_t __cdecl Get_Next_AI_Talker_(int32_t target_player_id) {
        uint8_t weights[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        uint8_t max_weight = 0;
        bool already_talking = false;

        if (MOX::_NUM_PLAYERS <= 0) {
            return -1;
        }

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (NETMOX::_net.player_net_info[i].remote_player_id == target_player_id) {
                if (NETMOX::_net.player_net_info[i].sync_state == '(') {
                    already_talking = true;
                }
                else {
                    // Determine diplomacy weight based on traits
                    // traits[20] = Charismatic, traits[19] = Repulsive
                    if (MOX::_player[i].traits[20] != 0) {
                        weights[i] = 3;
                    }
                    else if (MOX::_player[i].traits[19] != 0) {
                        weights[i] = 2;
                    }
                    else {
                        weights[i] = 1;
                    }

                    if (max_weight < weights[i]) {
                        max_weight = weights[i];
                    }
                }
            }
        }

        if (already_talking) {
            return -1;
        }

        // Filter out candidates that do not have the highest priority
        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (weights[i] < max_weight) {
                weights[i] = 0;
            }
        }

        return RUSS::Get_Weighted_Choice_Char_(weights, MOX::_NUM_PLAYERS);
    }

    void __cdecl Decode_Pick_Target_Info_(void* packet_data, uint32_t len) {
        uint8_t* ptr = static_cast<uint8_t*>(packet_data);

        // 1. Read Combat Star ID
        int16_t combat_star_id = *reinterpret_cast<int16_t*>(ptr);
        ptr += sizeof(int16_t); // Advance 2 bytes

        NETMOX::_net.combat_attacker_id = static_cast<int8_t>(MOX::_PLAYER_NUM);
        NETMOX::_net.combat_star_id = static_cast<int8_t>(combat_star_id);

        // 2. Read Target Info
        s_combfind_target target_info;
        memcpy(&target_info, ptr, sizeof(s_combfind_target)); // 44 bytes
        ptr += sizeof(s_combfind_target);

        COMBFIND::Copy_Target_Info_(&target_info);
        RUSS::Mox_Update_();

        // If we are not the host, unpack the state data
        if (NETMOX::_net.local_net_id != NETMOX::_net.net_host_id) {

            // Pass the remaining buffer pointer to Decode_Star_Player_Presence_
            COMBFIND::Decode_Star_Player_Presence_(&ptr, -1);

            // 3. Read Player Tech Applications
            if (MOX::_NUM_PLAYERS > 0) {
                for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                    memcpy(MOX::_player[i].tech_applications, ptr, sizeof(MOX::_player[i].tech_applications)); // 212 bytes
                    ptr += sizeof(MOX::_player[i].tech_applications);
                }
            }

            // 4. Read Leaders
            for (int i = 0; i < 67 /* 0x43 */; ++i) {
                MOX::_leaders[i].player_index = -1;
            }

            while (true) {
                int16_t leader_id = *reinterpret_cast<int16_t*>(ptr);
                ptr += sizeof(int16_t);

                if (leader_id == -1) {
                    break;
                }

                memcpy(&MOX::_leaders[leader_id], ptr, sizeof(s_leader_data)); // 59 bytes
                ptr += sizeof(s_leader_data);
                RUSS::Mox_Update_();
            }
            RUSS::Mox_Update_();

            // 5. Read Star Officer Indexes
            if (MOX::_NUM_PLAYERS > 0) {
                for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                    MOX::_star[combat_star_id].officer_index[i] = static_cast<char>(*ptr);
                    ptr += 1;
                }
            }
            RUSS::Mox_Update_();

            // 6. Read Planets and Colonies
            MOX::_NUM_COLONIES = *reinterpret_cast<int16_t*>(ptr);
            ptr += sizeof(int16_t);

            for (int i = 0; i < 5; ++i) {
                int16_t planet_idx = MOX::_star[combat_star_id].planet_index[i];

                if (planet_idx != -1) {
                    int16_t dummy_col_idx = *reinterpret_cast<int16_t*>(ptr);
                    ptr += sizeof(int16_t);

                    int8_t p_type = *reinterpret_cast<int8_t*>(ptr);
                    ptr += sizeof(int8_t);

                    char p_climate = *reinterpret_cast<char*>(ptr);
                    ptr += sizeof(char);

                    int16_t real_col_idx = *reinterpret_cast<int16_t*>(ptr);
                    ptr += sizeof(int16_t);

                    MOX::_planet[planet_idx].colony_index = dummy_col_idx;
                    MOX::_planet[planet_idx].planet_type = p_type;
                    MOX::_planet[planet_idx].climate = p_climate;

                    // Clear out existing colony mapping
                    if (MOX::_planet[planet_idx].colony_index != -1) {
                        MOX::_colony[MOX::_planet[planet_idx].colony_index].owner = -1;
                        MOX::_colony[MOX::_planet[planet_idx].colony_index].planet = -1;
                    }

                    MOX::_planet[planet_idx].colony_index = real_col_idx;

                    // Copy actual colony structure if a colony exists
                    if (MOX::_planet[planet_idx].colony_index != -1) {
                        memcpy(&MOX::_colony[MOX::_planet[planet_idx].colony_index], ptr, sizeof(s_colony)); // 361 bytes
                        ptr += sizeof(s_colony);
                    }
                }
            }
            RUSS::Mox_Update_();

            // 7. Read Ships
            MOX::_NUM_SHIPS = *reinterpret_cast<int16_t*>(ptr);
            ptr += sizeof(int16_t);

            if (MOX::_NUM_SHIPS > 0) {
                for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
                    MOX::_ship[i].status = 5; // Status 5 = travelling / in combat
                }
            }

            while (true) {
                int16_t ship_id = *reinterpret_cast<int16_t*>(ptr);
                ptr += sizeof(int16_t);

                if (ship_id == -1) {
                    break;
                }

                memcpy(&MOX::_ship[ship_id], ptr, sizeof(s_ship_design) + 30 /* Size of s_ship_data is 129 */);
                // NOTE: the code uses `sizeof(s_ship_data)` directly
                // memcpy(&MOX::_ship[ship_id], ptr, sizeof(s_ship_data));
                ptr += sizeof(s_ship_data);

                RUSS::Mox_Update_();
            }
        }

        // 8. Finalize synchronization state
        MAINSCR::Update_Ship_Icons_In_Between_Turns_();

        NETMOX::_net.player_net_info[MOX::_PLAYER_NUM].sync_state = 0x10;
        NETMOX::_net.sync_state = 0x10;
    }

    void __cdecl Decode_You_Are_Target_Info_(void* packet_data, uint32_t len) {
        uint8_t* ptr = static_cast<uint8_t*>(packet_data);

        // 1. Read Header
        int16_t star_id = *reinterpret_cast<int16_t*>(ptr);
        ptr += sizeof(int16_t);

        int16_t attacker_id = *reinterpret_cast<int16_t*>(ptr);
        ptr += sizeof(int16_t);

        int16_t defender_id = *reinterpret_cast<int16_t*>(ptr);
        ptr += sizeof(int16_t);

        int16_t target_colony_id = *reinterpret_cast<int16_t*>(ptr);
        ptr += sizeof(int16_t);

        NETMOX::_net.combat_star_id = static_cast<int8_t>(star_id);
        NETMOX::_net.combat_attacker_id = static_cast<int8_t>(attacker_id);
        NETMOX::_net.combat_defender_id = defender_id;
        NETMOX::_net.combat_target_player_id = MOX::_PLAYER_NUM;

        // If we are not the host, unpack the state data
        if (NETMOX::_net.local_net_id != NETMOX::_net.net_host_id) {

            // Pass the remaining buffer pointer
            COMBFIND::Decode_Star_Player_Presence_(&ptr, -1);

            // 2. Read Player Tech Applications
            if (attacker_id < 8) {
                memcpy(MOX::_player[attacker_id].tech_applications, ptr, sizeof(MOX::_player[attacker_id].tech_applications)); // 212 bytes
                ptr += sizeof(MOX::_player[attacker_id].tech_applications);
            }

            if (MOX::_PLAYER_NUM < 8) {
                memcpy(MOX::_player[MOX::_PLAYER_NUM].tech_applications, ptr, sizeof(MOX::_player[MOX::_PLAYER_NUM].tech_applications)); // 212 bytes
                ptr += sizeof(MOX::_player[MOX::_PLAYER_NUM].tech_applications);
            }

            // 3. Read Leaders
            for (int i = 0; i < 67 /* 0x43 */; ++i) {
                int16_t owner = MOX::_leaders[i].player_index;
                if (owner == attacker_id || owner == MOX::_PLAYER_NUM) {
                    MOX::_leaders[i].player_index = -1;
                }
            }

            while (true) {
                int16_t leader_id = *reinterpret_cast<int16_t*>(ptr);
                ptr += sizeof(int16_t);

                if (leader_id == -1) {
                    break;
                }

                memcpy(&MOX::_leaders[leader_id], ptr, sizeof(s_leader_data)); // 59 bytes
                ptr += sizeof(s_leader_data);
            }

            // 4. Read Star Officer Indexes
            if (attacker_id < 8) {
                MOX::_star[star_id].officer_index[attacker_id] = static_cast<char>(*ptr);
                ptr += 1;
            }

            if (MOX::_PLAYER_NUM < 8) {
                MOX::_star[star_id].officer_index[MOX::_PLAYER_NUM] = static_cast<char>(*ptr);
                ptr += 1;
            }

            // 5. Read Targeted Colony Data
            if (target_colony_id != -1) {
                MOX::_NUM_COLONIES = *reinterpret_cast<int16_t*>(ptr);
                ptr += sizeof(int16_t);

                int16_t planet_idx = *reinterpret_cast<int16_t*>(ptr);
                ptr += sizeof(int16_t);

                int16_t dummy_col_idx = *reinterpret_cast<int16_t*>(ptr);
                ptr += sizeof(int16_t);

                int8_t p_type = *reinterpret_cast<int8_t*>(ptr);
                ptr += sizeof(int8_t);

                char p_climate = *reinterpret_cast<char*>(ptr);
                ptr += sizeof(char);

                MOX::_planet[planet_idx].colony_index = dummy_col_idx;
                MOX::_planet[planet_idx].planet_type = p_type;
                MOX::_planet[planet_idx].climate = p_climate;

                memcpy(&MOX::_colony[target_colony_id], ptr, sizeof(s_colony)); // 361 bytes
                ptr += sizeof(s_colony);
            }

            // 6. Read Ships
            MOX::_NUM_SHIPS = *reinterpret_cast<int16_t*>(ptr);
            ptr += sizeof(int16_t);

            if (MOX::_NUM_SHIPS > 0) {
                for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
                    MOX::_ship[i].status = 5; // Status 5 = travelling / in combat
                }
            }

            while (true) {
                int16_t ship_id = *reinterpret_cast<int16_t*>(ptr);
                ptr += sizeof(int16_t);

                if (ship_id == -1) {
                    break;
                }

                memcpy(&MOX::_ship[ship_id], ptr, sizeof(s_ship_data)); // 129 bytes
                ptr += sizeof(s_ship_data);
            }
        }

        // 7. Finalize synchronization state
        NETMOX::_net.combat_planet_id = COMBFIND::Target_Colony_To_Planet_(target_colony_id);
        NETMOX::_net.sync_state = 0x13;
    }

    void __cdecl Decode_Pick_Your_Position_(s_net_pack_pick_your_position* param_1) {
        MOX::_NUM_PLAYERS = param_1->num_players;
        memcpy(MOX::_player, param_1->players, param_1->num_players * sizeof(s_player));
    }

    void __cdecl Decode_Battle_Results_(void* packet_data, uint32_t len) {
        uint8_t* ptr = static_cast<uint8_t*>(packet_data);

        // 1. Read Header
        int16_t reporting_player_id = *reinterpret_cast<int16_t*>(ptr);
        ptr += sizeof(int16_t);

        int16_t attacker_id = *reinterpret_cast<int16_t*>(ptr);
        ptr += sizeof(int16_t);

        int16_t defender_id = *reinterpret_cast<int16_t*>(ptr);
        ptr += sizeof(int16_t);

        int8_t combat_status = *reinterpret_cast<int8_t*>(ptr);
        ptr += sizeof(int8_t);

        NETMOX::_net.player_net_info[attacker_id].combat_status = combat_status;
        NETMOX::_net.player_net_info[defender_id].combat_status = combat_status;

        RUSS::Mox_Update_();

        // 2. Filter: Only unpack if we aren't the reporter
        if (MOX::_PLAYER_NUM != reporting_player_id) {

            bool skip_unpack = false;
            // Do not unpack if the reporter is the defender, the attacker is a valid player,
            // and the attacker is a human player (objective == 100 / 'd')
            if (defender_id == reporting_player_id && attacker_id < 8) {
                if (MOX::_player[attacker_id].objectives == 100) {
                    skip_unpack = true;
                }
            }

            if (!skip_unpack) {
                int16_t combat_star_id = *reinterpret_cast<int16_t*>(ptr);
                ptr += sizeof(int16_t);

                COMBFIND::Decode_Star_Player_Presence_(&ptr, combat_star_id);

                int16_t combat_planet_idx = *reinterpret_cast<int16_t*>(ptr);
                ptr += sizeof(int16_t);

                RUSS::Mox_Update_();

                // 3. Unpack Tech Applications and Officer Indexes
                if (attacker_id < 8) {
                    memcpy(MOX::_player[attacker_id].tech_applications, ptr, sizeof(MOX::_player[attacker_id].tech_applications)); // 212 bytes
                    ptr += sizeof(MOX::_player[attacker_id].tech_applications);

                    MOX::_star[combat_star_id].officer_index[attacker_id] = static_cast<char>(*ptr);
                    ptr += 1;
                }

                if (defender_id < 8) {
                    memcpy(MOX::_player[defender_id].tech_applications, ptr, sizeof(MOX::_player[defender_id].tech_applications)); // 212 bytes
                    ptr += sizeof(MOX::_player[defender_id].tech_applications);

                    MOX::_star[combat_star_id].officer_index[defender_id] = static_cast<char>(*ptr);
                    ptr += 1;
                }

                // 4. Unpack Surviving Leaders
                while (true) {
                    int16_t leader_id = *reinterpret_cast<int16_t*>(ptr);
                    ptr += sizeof(int16_t);

                    if (leader_id == -1) {
                        break;
                    }

                    memcpy(&MOX::_leaders[leader_id], ptr, sizeof(s_leader_data)); // 59 bytes
                    ptr += sizeof(s_leader_data);
                }
                RUSS::Mox_Update_();

                // 5. Unpack Colony Fate
                if (combat_planet_idx != -1) {
                    int16_t old_colony_index = MOX::_planet[combat_planet_idx].colony_index;

                    int16_t new_colony_index = *reinterpret_cast<int16_t*>(ptr);
                    ptr += sizeof(int16_t);

                    int8_t p_type = *reinterpret_cast<int8_t*>(ptr);
                    ptr += sizeof(int8_t);

                    char p_climate = *reinterpret_cast<char*>(ptr);
                    ptr += sizeof(char);

                    int16_t actual_col_idx = *reinterpret_cast<int16_t*>(ptr);
                    ptr += sizeof(int16_t);

                    MOX::_planet[combat_planet_idx].colony_index = new_colony_index;
                    MOX::_planet[combat_planet_idx].planet_type = p_type;
                    MOX::_planet[combat_planet_idx].climate = p_climate;

                    if (actual_col_idx == -1) {
                        if (old_colony_index != -1) {
                            BOMB::Destroy_Colony_(&MOX::_colony[old_colony_index], 1);
                        }
                    }
                    else {
                        memcpy(&MOX::_colony[actual_col_idx], ptr, sizeof(s_colony)); // 361 bytes
                        ptr += sizeof(s_colony);
                        COLCALC::Compute_Star_Colony_Stuff_(&MOX::_star[combat_star_id]);
                    }
                }
                RUSS::Mox_Update_();

                // 6. Unpack Ships and Process Casualties
                MOX::_NUM_SHIPS = *reinterpret_cast<int16_t*>(ptr);
                ptr += sizeof(int16_t);

                int16_t current_ship_idx = 0;

                while (true) {
                    int16_t surviving_ship_id = *reinterpret_cast<int16_t*>(ptr);
                    ptr += sizeof(int16_t);

                    if (surviving_ship_id == -1) {
                        break;
                    }

                    // Kill any ships that were skipped in the ID sequence (meaning they died)
                    while (current_ship_idx < surviving_ship_id) {
                        s_ship_data* ship = &MOX::_ship[current_ship_idx];
                        if ((ship->owner == attacker_id || ship->owner == defender_id) &&
                            ship->status != 5 && ship->location == combat_star_id) {
                            ship->officer_index = -1;
                            SHIPS::Kill_Ship_(current_ship_idx);
                        }
                        current_ship_idx++;
                    }

                    // Update the surviving ship
                    memcpy(&MOX::_ship[surviving_ship_id], ptr, sizeof(s_ship_data)); // 129 bytes
                    ptr += sizeof(s_ship_data);

                    current_ship_idx++; // Advance past the surviving ship we just updated
                    RUSS::Mox_Update_();
                }

                // Sweep any remaining ships at the end of the array that died
                while (current_ship_idx < MOX::_NUM_SHIPS) {
                    s_ship_data* ship = &MOX::_ship[current_ship_idx];
                    if ((ship->owner == attacker_id || ship->owner == defender_id) &&
                        ship->status != 5 && ship->location == combat_star_id) {
                        ship->officer_index = -1;
                        SHIPS::Kill_Ship_(current_ship_idx);
                    }
                    current_ship_idx++;
                }
            }
        }

        MAINSCR::Update_Ship_Icons_In_Between_Turns_();
        NETMOX::_net.player_net_info[reporting_player_id].sync_state = 0x1B;
    }

    void __cdecl Decode_Dragon_Info_(int32_t* colony_id_ptr) {
        int16_t planet_idx = MOX::_colony[*colony_id_ptr].planet;

        _net.combat_planet_id = planet_idx;
        _net.combat_star_id = MOX::_planet[planet_idx].star_index;
    }

    void __cdecl Send_Dragon_Info_(int32_t colony_id) {
        s_packet_dragon_info packet;
        packet.colony_id = colony_id;

        int8_t owner = MOX::_colony[colony_id].owner;
        int16_t network_player_id = MOX::_player[owner].network_player_id;

        Mox_Send_Message_(network_player_id, 0x23, &packet, (uint16_t)sizeof(packet));
    }

    void __cdecl Decode_Drop_Out_Info_(int32_t player_idx, s_net_pack_drop_out_info* drop_out_info) {
        if (MOX::_player[player_idx].network_player_id == _net.host_election_winner_id) {
            _net.host_election_winner_id = -1;
        }

        RUSS::Make_Player_Into_AI_(player_idx);

        memcpy(&MOX::_player[player_idx], &drop_out_info->player_data, sizeof(s_player));

        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            MOX::_star[i].surrender_to[player_idx] = drop_out_info->star_surrender_to[i];
        }

        _net.num_connected_players--;
        _net.players_hit_next_turn[player_idx] = 1;

        if (_net.local_net_id == _net.net_host_id) {
            if (_net.host_active_player_id == player_idx) {
                _net.host_active_player_id = -1;
            }

            _net.player_is_active_flags[player_idx] = 0;
            _net.host_sync_counter++;

            if (static_cast<uint8_t>(_net.host_sync_counter) == MOX::_NUM_PLAYERS) {
                _net.sync_state = 11;
            }
        }
    }

    void __cdecl Send_Battle_Results_(int16_t star_id, int16_t player_id, int16_t target_player_id, int16_t combat_defender_id, int16_t combat_planet_id, int16_t combat_result) {
        int16_t player_num = MOX::_PLAYER_NUM;

        allocate::Push_Block_(MOX::_screen_seg);
        uint8_t* packet = (uint8_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x7800);
        uint8_t* local_4 = packet;

        *reinterpret_cast<int16_t*>(local_4) = player_num;
        local_4 += 2;
        *reinterpret_cast<int16_t*>(local_4) = player_id;
        local_4 += 2;
        *reinterpret_cast<int16_t*>(local_4) = target_player_id;
        local_4 += 2;
        *reinterpret_cast<int16_t*>(local_4) = combat_result;
        local_4 += 2;

        if (_net.local_net_id != _net.net_host_id &&
            (player_num != target_player_id || player_id >= 8 || MOX::_player[player_id].objectives != 100 /* 'd' */)) {

            *reinterpret_cast<int16_t*>(local_4) = star_id;
            local_4 += 2;

            COMBFIND::Send_Star_Player_Presence_(&local_4, -1);

            *reinterpret_cast<int16_t*>(local_4) = combat_defender_id;
            local_4 += 2;
            *reinterpret_cast<int16_t*>(local_4) = combat_planet_id;
            local_4 += 2;

            RUSS::Mox_Update_();

            if (player_id < 8) {
                memcpy(local_4, MOX::_player[player_id].tech_applications, sizeof(MOX::_player[player_id].tech_applications));
                local_4 += sizeof(MOX::_player[player_id].tech_applications);
                *local_4 = MOX::_star[star_id].officer_index[player_id];
                local_4++;
            }

            if (target_player_id < 8) {
                memcpy(local_4, MOX::_player[target_player_id].tech_applications, sizeof(MOX::_player[target_player_id].tech_applications));
                local_4 += sizeof(MOX::_player[target_player_id].tech_applications);
                *local_4 = MOX::_star[star_id].officer_index[target_player_id];
                local_4++;
            }

            RUSS::Mox_Update_();

            for (int16_t i = 0; i < 67; i++) {
                if (MOX::_leaders[i].player_index == player_id || MOX::_leaders[i].player_index == target_player_id) {
                    *reinterpret_cast<int16_t*>(local_4) = i;
                    local_4 += 2;
                    memcpy(local_4, &MOX::_leaders[i], sizeof(s_leader_data));
                    local_4 += sizeof(s_leader_data);
                }
            }

            *reinterpret_cast<int16_t*>(local_4) = -1;
            local_4 += 2;

            RUSS::Mox_Update_();

            if (combat_planet_id != -1) {
                int16_t colony_idx = MOX::_planet[combat_planet_id].colony_index;
                *reinterpret_cast<int16_t*>(local_4) = colony_idx;
                local_4 += 2;
                *local_4 = MOX::_planet[combat_planet_id].planet_type;
                local_4++;
                *local_4 = MOX::_planet[combat_planet_id].climate;
                local_4++;
                *reinterpret_cast<int16_t*>(local_4) = colony_idx;
                local_4 += 2;

                if (colony_idx != -1) {
                    memcpy(local_4, &MOX::_colony[colony_idx], sizeof(s_colony));
                    local_4 += sizeof(s_colony);
                }
            }

            RUSS::Mox_Update_();

            *reinterpret_cast<int16_t*>(local_4) = (int16_t)MOX::_NUM_SHIPS;
            local_4 += 2;

            if (MOX::_NUM_SHIPS > 0) {
                for (int16_t i = 0; i < (int16_t)MOX::_NUM_SHIPS; i++) {
                    if (MOX::_ship[i].location == star_id && MOX::_ship[i].status != 5) {
                        if (MOX::_ship[i].owner == player_id || MOX::_ship[i].owner == target_player_id) {
                            *reinterpret_cast<int16_t*>(local_4) = i;
                            local_4 += 2;
                            memcpy(local_4, &MOX::_ship[i], sizeof(s_ship_data));
                            local_4 += sizeof(s_ship_data);
                        }
                    }
                }
            }

            *reinterpret_cast<int16_t*>(local_4) = -1;
            local_4 += 2;

            RUSS::Mox_Update_();
        }

        int32_t size = local_4 - packet;
        if (size <= 0x8000) {
            NETMOX::Mox_Send_Message_(_net.local_net_id, 0x15, packet, (uint16_t)size);
        } else {
            int16_t chunks = (int16_t)((size + 0x77FF) / 0x7800);
            _net.sync_state = 0x1F;
            NETMOX::Mox_Send_Message_(_net.local_net_id, 0x30, nullptr, 0);

            while (_net.sync_state != 0x20) {
                RUSS::Mox_Update_();
            }

            _net.sync_state = 0x0E;
            s_packet_32 packet_32;
            packet_32.chunks = chunks;
            NETMOX::Mox_Send_Message_(_net.local_net_id, 0x32, &packet_32, sizeof(s_packet_32));

            uint8_t* ptr = packet;
            while (ptr < local_4) {
                RUSS::Mox_Update_();
                int32_t chunk_size = local_4 - ptr;
                if (chunk_size > 0x7800) {
                    chunk_size = 0x7800;
                }

                do {
                    MULTPLAY::Chat_Box_Input_Loop_();
                } while (netcode::Net_Is_Sending_() != 0);

                NETMOX::Mox_Send_Message_(_net.local_net_id, 0x33, ptr, (uint16_t)chunk_size);
                ptr += 0x7800;
            }
        }

        allocate::Pop_Block_(MOX::_screen_seg);
    }

    void __cdecl Send_You_Are_Target_Info_(
        int16_t target_player_idx,
        int16_t other_player_idx,
        int16_t param_3,
        int16_t colony_idx,
        int16_t star_idx)
    {
        allocate::Push_Block_(MOX::_screen_seg);

        int32_t alloc_size = (target_player_idx == MOX::_PLAYER_NUM) ? 30 : 0x7800;
        uint8_t* packet = (uint8_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, alloc_size);

        s_packet_0x12* hdr = reinterpret_cast<s_packet_0x12*>(packet);
        hdr->star_idx = star_idx;
        hdr->other_player_idx = other_player_idx;
        hdr->param_3 = param_3;
        hdr->colony_idx = colony_idx;

        uint8_t* ptr = packet + sizeof(s_packet_0x12);

        if (MOX::_player[target_player_idx].network_player_id != _net.local_net_id) {
            COMBFIND::Send_Star_Player_Presence_(&ptr, star_idx);
            RUSS::Mox_Update_();

            if (other_player_idx < 8) {
                memcpy(ptr, MOX::_player[other_player_idx].tech_applications, 212);
                ptr += 212;
            }

            if (target_player_idx < 8) {
                memcpy(ptr, MOX::_player[target_player_idx].tech_applications, 212);
                ptr += 212;
            }

            RUSS::Mox_Update_();

            for (int16_t i = 0; i < 67; ++i) {
                if (MOX::_leaders[i].player_index == other_player_idx || MOX::_leaders[i].player_index == target_player_idx) {
                    *reinterpret_cast<int16_t*>(ptr) = i;
                    ptr += sizeof(int16_t);

                    memcpy(ptr, &MOX::_leaders[i], sizeof(s_leader_data));
                    ptr += sizeof(s_leader_data);
                }
            }

            *reinterpret_cast<int16_t*>(ptr) = -1;
            ptr += sizeof(int16_t);

            if (other_player_idx < 8) {
                *ptr = MOX::_star[star_idx].officer_index[other_player_idx];
                ptr++;
            }

            if (target_player_idx < 8) {
                *ptr = MOX::_star[star_idx].officer_index[target_player_idx];
                ptr++;
            }

            RUSS::Mox_Update_();

            if (colony_idx != -1) {
                int16_t planet_idx = MOX::_colony[colony_idx].planet;

                *reinterpret_cast<int16_t*>(ptr) = (int16_t)MOX::_NUM_COLONIES;
                ptr += sizeof(int16_t);
                *reinterpret_cast<int16_t*>(ptr) = planet_idx;
                ptr += sizeof(int16_t);
                *reinterpret_cast<int16_t*>(ptr) = MOX::_planet[planet_idx].colony_index;
                ptr += sizeof(int16_t);

                *ptr = MOX::_planet[planet_idx].planet_type;
                ptr++;
                *ptr = MOX::_planet[planet_idx].climate;
                ptr++;

                memcpy(ptr, &MOX::_colony[colony_idx], sizeof(s_colony));
                ptr += sizeof(s_colony);
            }

            RUSS::Mox_Update_();

            *reinterpret_cast<int16_t*>(ptr) = (int16_t)MOX::_NUM_SHIPS;
            ptr += sizeof(int16_t);

            if (MOX::_NUM_SHIPS > 0) {
                for (int16_t i = 0; i < (int16_t)MOX::_NUM_SHIPS; ++i) {
                    if (MOX::_ship[i].location == star_idx && MOX::_ship[i].status != 5) {
                        if (MOX::_ship[i].owner == other_player_idx || MOX::_ship[i].owner == target_player_idx) {
                            *reinterpret_cast<int16_t*>(ptr) = i;
                            ptr += sizeof(int16_t);

                            memcpy(ptr, &MOX::_ship[i], sizeof(s_ship_data));
                            ptr += sizeof(s_ship_data);
                        }
                    }
                }
            }

            *reinterpret_cast<int16_t*>(ptr) = -1;
            ptr += sizeof(int16_t);

            RUSS::Mox_Update_();
        }

        int32_t msg_len = ptr - packet;

        if (msg_len <= 0x8000) {
            NETMOX::Mox_Send_Message_(MOX::_player[target_player_idx].network_player_id, 0x12, packet, (uint16_t)msg_len);
        } else {
            s_packet_0x34 chunk_info;
            chunk_info.chunks = (int16_t)((msg_len + 0x77FF) / 0x7800);

            NETMOX::Mox_Send_Message_(MOX::_player[target_player_idx].network_player_id, 0x34, &chunk_info, sizeof(s_packet_0x34));

            uint8_t* send_ptr = packet;
            while (send_ptr < ptr) {
                RUSS::Mox_Update_();

                int32_t chunk_len = ptr - send_ptr;
                if (chunk_len > 0x7800) {
                    chunk_len = 0x7800;
                }

                do {
                    MULTPLAY::Chat_Box_Input_Loop_();
                } while (netcode::Net_Is_Sending_() != 0);

                NETMOX::Mox_Send_Message_(MOX::_player[target_player_idx].network_player_id, 0x35, send_ptr, (uint16_t)chunk_len);
                send_ptr += 0x7800;
            }
        }

        allocate::Pop_Block_(MOX::_screen_seg);
    }

    void __cdecl Send_Pick_Target_Info_(int16_t player_idx, int16_t star_idx, s_combfind_target* target_info) {
        allocate::Push_Block_(MOX::_screen_seg);

        uint32_t alloc_size = ((player_idx == MOX::_PLAYER_NUM) ? 0xFFFF8814 : 0) + 0x7800;
        uint8_t* packet = static_cast<uint8_t*>(allocate::Allocate_Next_Block_(MOX::_screen_seg, alloc_size));
        uint8_t* local_8 = packet;

        *reinterpret_cast<int16_t*>(local_8) = star_idx;
        local_8 += 2;

        memcpy(local_8, target_info, sizeof(s_combfind_target));
        local_8 += sizeof(s_combfind_target);

        if (MOX::_player[player_idx].network_player_id != _net.local_net_id) {
            COMBFIND::Send_Star_Player_Presence_(&local_8, star_idx);

            if (MOX::_NUM_PLAYERS > 0) {
                for (int16_t i = 0; i < MOX::_NUM_PLAYERS; i++) {
                    memcpy(local_8, MOX::_player[i].tech_applications, sizeof(MOX::_player[i].tech_applications));
                    local_8 += sizeof(MOX::_player[i].tech_applications);
                }
            }

            RUSS::Mox_Update_();

            for (int16_t i = 0; i < 67; i++) {
                *reinterpret_cast<int16_t*>(local_8) = i;
                local_8 += 2;
                memcpy(local_8, &MOX::_leaders[i], sizeof(s_leader_data));
                local_8 += sizeof(s_leader_data);
                RUSS::Mox_Update_();
            }

            *reinterpret_cast<int16_t*>(local_8) = -1;
            local_8 += 2;

            if (MOX::_NUM_PLAYERS > 0) {
                for (int16_t i = 0; i < MOX::_NUM_PLAYERS; i++) {
                    *local_8 = MOX::_star[star_idx].officer_index[i];
                    local_8++;
                }
            }

            RUSS::Mox_Update_();

            *reinterpret_cast<int16_t*>(local_8) = (int16_t)MOX::_NUM_COLONIES;
            local_8 += 2;

            for (int i = 0; i < 5; i++) {
                int16_t planet_idx = MOX::_star[star_idx].planet_index[i];
                if (planet_idx != -1) {
                    *reinterpret_cast<int16_t*>(local_8) = MOX::_planet[planet_idx].colony_index;
                    local_8 += 2;
                    *local_8 = MOX::_planet[planet_idx].planet_type;
                    local_8++;
                    *local_8 = MOX::_planet[planet_idx].climate;
                    local_8++;
                    *reinterpret_cast<int16_t*>(local_8) = MOX::_planet[planet_idx].colony_index;
                    local_8 += 2;

                    if (MOX::_planet[planet_idx].colony_index != -1) {
                        memcpy(local_8, &MOX::_colony[MOX::_planet[planet_idx].colony_index], sizeof(s_colony));
                        local_8 += sizeof(s_colony);
                    }
                }
            }

            RUSS::Mox_Update_();

            *reinterpret_cast<int16_t*>(local_8) = (int16_t)MOX::_NUM_SHIPS;
            local_8 += 2;

            if (MOX::_NUM_SHIPS > 0) {
                for (int16_t i = 0; i < (int16_t)MOX::_NUM_SHIPS; i++) {
                    if (MOX::_ship[i].location == star_idx && MOX::_ship[i].status != 5) {
                        *reinterpret_cast<int16_t*>(local_8) = i;
                        local_8 += 2;
                        memcpy(local_8, &MOX::_ship[i], sizeof(s_ship_data));
                        local_8 += sizeof(s_ship_data);
                        RUSS::Mox_Update_();
                    }
                }
            }

            *reinterpret_cast<int16_t*>(local_8) = -1;
            local_8 += 2;
        }

        RUSS::Mox_Update_();

        int32_t diff = static_cast<int32_t>(local_8 - packet);
        if (diff <= 0x8000) {
            NETMOX::Mox_Send_Message_(MOX::_player[player_idx].network_player_id, 0x10, packet, (uint16_t)diff);
        } else {
            s_packet_2E_chunks chunk_msg;
            chunk_msg.chunks = static_cast<int16_t>((diff + 0x77FF) / 0x7800);
            NETMOX::Mox_Send_Message_(MOX::_player[player_idx].network_player_id, 0x2E, &chunk_msg, 2);

            while (packet < local_8) {
                RUSS::Mox_Update_();
                int32_t chunk_size = static_cast<int32_t>(local_8 - packet);
                if (chunk_size > 0x7800) {
                    chunk_size = 0x7800;
                }

                do {
                    MULTPLAY::Chat_Box_Input_Loop_();
                } while (netcode::Net_Is_Sending_() != 0);

                uint8_t* current_packet = packet;
                packet += 0x7800;
                NETMOX::Mox_Send_Message_(MOX::_player[player_idx].network_player_id, 0x2F, current_packet, (uint16_t)chunk_size);
            }
        }

        allocate::Pop_Block_(MOX::_screen_seg);
    }
}