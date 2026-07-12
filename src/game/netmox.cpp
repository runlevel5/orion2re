#include "pch.h"

#include "game/netproto.h"

namespace NETMOX {
    s_network _net;

    uint8_t _sending_msg;

    char* _lost_net_user_error;

    char* _only_1_host_allowed_error;

    char* _net_snapshot_diff_buffer_error;

    char* _net_snapshot_cache_error;

    s_net_snapshot_cache _net_snapshot_cache;

    char* _incompatible_versions_error;

    char* _ambassador_left_before_you_arrived_fmt;

    char* _net_send_error;

    uint8_t _in_receive_message;

    uint8_t _is_updating;

    uint8_t _next_update_player_counter;

    namespace {
        uint32_t _suspended_net_update_depth;
        int8_t _net_debug_initialized;
        int8_t _net_debug_enabled;

        struct Scoped_Net_Update_Suspension_ {
            Scoped_Net_Update_Suspension_() {
                ++_suspended_net_update_depth;
            }

            ~Scoped_Net_Update_Suspension_() {
                --_suspended_net_update_depth;
            }
        };

#pragma pack(push, 1)
        struct s_net_pack_player_disconnected_info_ {
            int16_t player_idx;
            s_net_pack_drop_out_info drop_out_info;
        };
#pragma pack(pop)

        // Verified with a size probe against current packed structures:
        // Send_Battle_Results_ max ~= 70489 bytes,
        // Send_You_Are_Target_Info_ max ~= 70485 bytes,
        // Send_Pick_Target_Info_ max ~= 73269 bytes.
        // Send_Game_Orders_ max ~= 165833 bytes.
        constexpr uint32_t k_medium_network_payload_bytes = 200 * 1024u;
        constexpr size_t k_star_player_presence_payload_bytes = MAX_PLAYERS * sizeof(s_combfind_star_has_player) + sizeof(uint8_t);

        bool Is_Valid_Game_Player_(int player_idx) {
            return player_idx >= 0 && player_idx < MOX::_NUM_PLAYERS && player_idx < MAX_PLAYERS;
        }

        bool Is_Connected_Game_Player_(int player_idx) {
            return Is_Valid_Game_Player_(player_idx) && MOX::_player[player_idx].network_player_id != -1;
        }

        int Elect_Disconnect_Recovery_Coordinator_(int disconnected_player_idx) {
            for (int i = 0; i < MOX::_NUM_PLAYERS && i < MAX_PLAYERS; ++i) {
                if (i != disconnected_player_idx && MOX::_player[i].network_player_id != -1) {
                    return i;
                }
            }
            return -1;
        }

        bool Is_Local_Player_Slot_(int player_idx) {
            return Is_Valid_Game_Player_(player_idx) && player_idx == MOX::_PLAYER_NUM;
        }

        void Promote_Recovery_Coordinator_If_Needed_(int disconnected_player_idx, int16_t disconnected_network_id) {
            int coordinator_idx = Elect_Disconnect_Recovery_Coordinator_(disconnected_player_idx);
            if (coordinator_idx < 0) {
                return;
            }

            int16_t coordinator_network_id = MOX::_player[coordinator_idx].network_player_id;
            if (coordinator_network_id == -1) {
                return;
            }

            if (_net.host_election_id == disconnected_network_id || _net.host_election_id == -1) {
                _net.host_election_id = coordinator_network_id;
            }
            if (_net.host_election_winner_id == disconnected_network_id || _net.host_election_winner_id == -1) {
                _net.host_election_winner_id = coordinator_network_id;
            }
            if (_net.local_net_id == disconnected_network_id || _net.local_net_id == -1) {
                _net.local_net_id = coordinator_network_id;
            }
        }

        void Apply_Drop_Out_Bookkeeping_(int player_idx, int16_t disconnected_network_id, bool promote_recovery_coordinator) {
            if (promote_recovery_coordinator) {
                Promote_Recovery_Coordinator_If_Needed_(player_idx, disconnected_network_id);
            }

            if (_net.num_connected_players > 0) {
                _net.num_connected_players--;
            }
            _net.players_hit_next_turn[player_idx] = 1;

            if (_net.local_net_id == _net.net_host_id) {
                if (_net.host_active_player_id == player_idx) {
                    _net.host_active_player_id = -1;
                }

                _net.player_is_active_flags[player_idx] = 0;
                _net.host_sync_counter++;

                if (static_cast<uint8_t>(_net.host_sync_counter) == MOX::_NUM_PLAYERS) {
                    _net.sync_state = NET_SYNC_STATE_HOST_BEGIN_TURN;
                }
            }
        }

        void Build_Drop_Out_Info_From_Local_State_(int player_idx, s_net_pack_drop_out_info* packet) {
            MOX::_player[player_idx].network_player_id = -1;
            RUSS::Make_Player_Into_AI_(player_idx);

            memcpy(&packet->player_data, &MOX::_player[player_idx], sizeof(s_player));

            if (MOX::_NUM_STARS > 0) {
                for (int i = 0; i < MOX::_NUM_STARS; ++i) {
                    packet->star_surrender_to[i] = MOX::_star[i].surrender_to[player_idx];
                }
            }
        }

        void Apply_Drop_Out_Info_(int32_t player_idx, s_net_pack_drop_out_info* drop_out_info, bool promote_recovery_coordinator) {
            if (!Is_Valid_Game_Player_(player_idx) || drop_out_info == nullptr) {
                return;
            }

            int16_t disconnected_network_id = MOX::_player[player_idx].network_player_id;
            if (MOX::_player[player_idx].network_player_id == _net.host_election_winner_id) {
                _net.host_election_winner_id = -1;
            }

            RUSS::Make_Player_Into_AI_(player_idx);

            memcpy(&MOX::_player[player_idx], &drop_out_info->player_data, sizeof(s_player));

            for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
                MOX::_star[i].surrender_to[player_idx] = drop_out_info->star_surrender_to[i];
            }

            Apply_Drop_Out_Bookkeeping_(player_idx, disconnected_network_id, promote_recovery_coordinator);
        }
    }

    int32_t __cdecl Net_Id_To_Player_Id_(int32_t player_idx) {
        for (int32_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (MOX::_player[i].network_player_id == player_idx) {
                return i;
            }
        }
        return -1;
    }

    char* __cdecl Load_Netmox_Strings_(char* strings) {
        _net_snapshot_cache_error = strings;
        strings += strlen(strings) + 1;

        _net_snapshot_diff_buffer_error = strings;
        strings += strlen(strings) + 1;

        _incompatible_versions_error = strings;
        strings += strlen(strings) + 1;

        _only_1_host_allowed_error = strings;
        strings += strlen(strings) + 1;

        _net_send_error = strings;
        strings += strlen(strings) + 1;

        _lost_net_user_error = strings;
        strings += strlen(strings) + 1;

        strings += strlen(strings) + 1;

        _ambassador_left_before_you_arrived_fmt = strings;
        strings += strlen(strings) + 1;

        return strings;
    }

    int8_t __cdecl Net_May_Begin_AI_Diplomacy_() {
        if (NETMOX::_net.sync_state >= NET_SYNC_STATE_AI_DIPLOMACY_REQUESTED) {
            if (NETMOX::_net.sync_state <= NET_SYNC_STATE_AI_DIPLOMACY_DONE) {
                return 0;
            }

            if (NETMOX::_net.sync_state == NET_SYNC_STATE_AI_DIPLOMACY_READY) {
                return 1;
            }
        }

        // The binary returns an uninitialized stack byte here; use a stable "not allowed" fallback.
        return 0;
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

    int32_t __cdecl Net_Diplomacy_With_() {
        return _net.diplomacy_target_id;
    }

    void __cdecl Net_Cancel_Human_Diplomacy_Request_() {
        if (MOX::_game_type >= 2 && MOX::_game_type <= 3) {
            int32_t target_id = _net.diplomacy_target_id;
            _net.diplomacy_target_id = -1;
            _net.sync_state = NET_SYNC_STATE_READY;

            int16_t network_player_id = MOX::_player[target_id].network_player_id;
            if (network_player_id != -1) {
                Mox_Send_Message_(network_player_id, NET_PACKET_CANCEL_HUMAN_DIPLOMACY, nullptr, 0);
            }
        }
    }

    void __cdecl Net_End_Human_Diplomacy_() {
        _net.diplomacy_target_id = -1;
        _net.sync_state = NET_SYNC_STATE_READY;
    }

    bool __cdecl Net_Human_Rejected_Diplomacy_() {
        bool result = false;

        if (_net.sync_state == NET_SYNC_STATE_HUMAN_DIPLOMACY_REJECTED) {
            _net.sync_state = NET_SYNC_STATE_READY;
            result = true;
        }

        return result;
    }

    bool __cdecl Allowed_To_Cancel_Human_Diplomacy_() {
        return _net.sync_state == NET_SYNC_STATE_HUMAN_DIPLOMACY_REQUESTED;
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
        switch (_net.sync_state) {
        case NET_SYNC_STATE_READY:
        case NET_SYNC_STATE_HUMAN_DIPLOMACY_REQUESTED:
        case NET_SYNC_STATE_HUMAN_DIPLOMACY_REJECTED:
        case NET_SYNC_STATE_HUMAN_DIPLOMACY_CONNECTING:
        case NET_SYNC_STATE_HUMAN_DIPLOMACY_BLOCKED:
            return 0;
        case NET_SYNC_STATE_HUMAN_DIPLOMACY_READY:
            return 1;
        default:
            // The binary returns an uninitialized stack byte here; use a stable "not allowed" fallback.
            return 0;
        }
    }

    bool __cdecl Net_Cancel_AI_Diplomacy_Confirmed_() {
        return _net.sync_state == NET_SYNC_STATE_READY;
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
            _net.sync_state = NET_SYNC_STATE_READY;

            if (_net.host_election_winner_id != -1) {
                Mox_Send_Message_(_net.host_election_winner_id, NET_PACKET_AI_DIPLOMACY_END_STATE, &msg, static_cast<int16_t>(sizeof(msg)));
            }
        }
    }

    void __cdecl Net_Cancel_AI_Diplomacy_Request_() {
        if (MOX::_game_type >= 2 && MOX::_game_type <= 3) {
            _net.diplomacy_target_id = -1;
            _net.sync_state = NET_SYNC_STATE_AI_DIPLOMACY_CANCEL_PENDING;
            if (_net.host_election_winner_id != -1) {
                Mox_Send_Message_(_net.host_election_winner_id, NET_PACKET_AI_DIPLOMACY_CANCEL_REQUEST, &_net.diplomacy_target_id, sizeof(_net.diplomacy_target_id));
            }
        }
    }

    bool __cdecl No_AI_Diplomacy_Allowed_() {
        if (_net.host_election_winner_id == -1) {
            _net.sync_state = NET_SYNC_STATE_READY;
            return true;
        }
        return false;
    }

    void __cdecl Net_Request_AI_Diplomacy_(uint8_t player_idx) {
        _net.sync_state = NET_SYNC_STATE_AI_DIPLOMACY_REQUESTED;
        _net.diplomacy_target_id = player_idx;

        if (_net.host_election_winner_id != -1) {
            Mox_Send_Message_(_net.host_election_winner_id, NET_PACKET_AI_DIPLOMACY_REQUEST, &player_idx, sizeof(player_idx));
        }
    }

    void __cdecl Receive_Message_(int16_t net_id, uint16_t packet_id, uint32_t len, uint8_t* packet) {
        int32_t player_id = Net_Id_To_Player_Id_(net_id & 0xFFFF);
        _in_receive_message = 1;

        if (packet_id == NET_PACKET_TACTICAL_COMBAT_START || packet_id == NET_PACKET_TACTICAL_COMBAT_END) {
            Net_Debug_Log_(
                "tactical handshake recv packet=%u sender_net_id=%d sender_player=%d local_player=%d state_before=%u",
                (unsigned int)packet_id,
                (int)net_id,
                (int)player_id,
                (int)MOX::_PLAYER_NUM,
                (unsigned int)_net.tactical_combat_sync_state);
        }

        if (packet_id >= DIP_NET_PACKET_MESSAGE) {
            DIP_SCRN::Diplomacy_Receive_Message_(net_id, packet_id, len, reinterpret_cast<s_dipscrn_net_message*>(packet));
        } else if (packet_id >= TACTICAL_NET_PACKET_ACTION) {
            CMBTMOV1::Tactical_Combat_Receive_Message_(net_id, packet_id, len, reinterpret_cast<s_net_tactical_combat_message*>(packet));
        } else {
            MAINSCR::Main_Receive_Message_(player_id, net_id, packet_id, len, packet);
        }

        _in_receive_message = 0;
    }

    void __cdecl Russ_Change_Relations_(int16_t change, int16_t attacker, int16_t defender, int16_t reason, int16_t star_idx, int16_t param6) {
        if (MOX::_game_type != 2 && MOX::_game_type != 3) {
            diplomac::Change_Relations_(change, attacker, defender, reason, star_idx, param6);
        } else {
            s_net_pack_change_relation msg_data;
            msg_data.change = change;
            msg_data.attacker = attacker;
            msg_data.defender = defender;
            msg_data.reason = reason;
            msg_data.star_idx = star_idx;
            msg_data.param6 = param6;

            Mox_Send_Message_(_net.local_net_id, NET_PACKET_CHANGE_RELATIONS, &msg_data, sizeof(msg_data));
        }
    }

    uint8_t Humans_Requesting_Diplomacy_() {
        return _net.players_active_mask;
    }

    int32_t __cdecl Human_Requesting_Diplomacy_() {
        int32_t target_player_idx = -1;
        int32_t ready_count = 0;

        if (_net.players_ready_mask[0] != 0) {
            for (int32_t i = 0; i < (int32_t)MOX::_NUM_PLAYERS; ++i) {
                uint8_t ready_mask = _net.players_ready_mask[0];
                if (((uint8_t)(ready_mask >> ((uint8_t)i & 0x1F)) & 1) != 0) {
                    ++ready_count;
                    if (game_random::Random_(ready_count) == 1) {
                        target_player_idx = i;
                    }
                }
            }

            uint8_t mask = (uint8_t)~((uint8_t)(1u << ((uint8_t)target_player_idx & 0x1F)));
            _net.players_ready_mask[0] &= mask;
        }

        return target_player_idx;
    }

    int32_t __cdecl Mox_Send_Message_(int16_t recipient_net_id, uint16_t msg_type, void* packet, uint32_t packet_len) {
        uint8_t dummy_packet;
        uint8_t skip_send;
        int32_t result = 1;

        if (msg_type == NET_PACKET_TACTICAL_COMBAT_START || msg_type == NET_PACKET_TACTICAL_COMBAT_END) {
            Net_Debug_Log_(
                "tactical handshake send packet=%u recipient_net_id=%d local_player=%d state=%u turn=%d",
                (unsigned int)msg_type,
                (int)recipient_net_id,
                (int)MOX::_PLAYER_NUM,
                (unsigned int)_net.tactical_combat_sync_state,
                (int)MOX::_combat_turn);
        }

        _sending_msg = 1;

        if (packet_len == 0) {
            packet_len = 1;
            packet = &dummy_packet;
        }

        Update_();

        if (_net.net_host_id == recipient_net_id) {
            Receive_Message_(recipient_net_id, msg_type, packet_len, (uint8_t*)packet);
        } else {
            skip_send = 0;
            do {
                Update_();
            } while (netcode::Net_Is_Sending_() != 0);

            if (recipient_net_id == (int16_t)netadapter::NETADAPTER_BROADCAST_EXCLUDING_SELF) {
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
                    recipient_net_id = (int16_t)last_valid_id;
                }
            }

            if (skip_send == 0) {
                result = netcode::Net_Send_Message_(recipient_net_id, msg_type, packet, packet_len);
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
        NETMOX::Mox_Send_Message_(MOX::_player[player_idx].network_player_id, NET_PACKET_HUMAN_DIPLOMACY_IGNORED, nullptr, 0);
    }

    void __cdecl Tell_Diplomacy_Requester_To_Wait_(int32_t player_idx) {
        Mox_Send_Message_(MOX::_player[player_idx].network_player_id, NET_PACKET_HUMAN_DIPLOMAT_WAIT, nullptr, 0);
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

        _net.sync_state = NET_SYNC_STATE_HUMAN_DIPLOMACY_CONNECTING;
        _net.diplomacy_active_flag = 1;
        _net.is_diplomacy_initiator = 0;

        Mox_Send_Message_(
            MOX::_player[player_idx].network_player_id,
            NET_PACKET_ACCEPT_HUMAN_DIPLOMACY,
            &MOX::_player[MOX::_PLAYER_NUM],
            sizeof(MOX::_player[MOX::_PLAYER_NUM])
        );

        while (_net.sync_state == NET_SYNC_STATE_HUMAN_DIPLOMACY_REQUESTED || _net.sync_state == NET_SYNC_STATE_HUMAN_DIPLOMACY_CONNECTING) {
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
        if (NETMOX::_in_receive_message == 0 && _suspended_net_update_depth == 0) {
            netcode::Net_Update_();
        }
    }

    bool __cdecl Net_Debug_Enabled_() {
        if (_net_debug_initialized == 0) {
            const char* value = SDL_getenv("ORION2RE_NET_DEBUG");
            if (value != nullptr) {
                _net_debug_enabled = (value[0] != '\0' && value[0] != '0');
            } else {
#if defined(ORION2RE_DEBUG_BUILD)
                _net_debug_enabled = 1;
#else
                _net_debug_enabled = 0;
#endif
            }
            _net_debug_initialized = 1;
        }

        return _net_debug_enabled != 0;
    }

    void __cdecl Net_Debug_Log_(const char* format, ...) {
        if (!Net_Debug_Enabled_()) {
            return;
        }

        char buffer[512];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "netdbg: %s", buffer);
    }

    void __cdecl Russ_Event_Monster_Update_(int16_t owner, int16_t param2, int16_t param3) {
        if (MOX::_game_type != 2 /* GAME_TYPE_NETWORK_HOST */ &&
            MOX::_game_type != 3 /* GAME_TYPE_NETWORK_CLIENT */) {
            EVENTS::Event_Monster_Update_(owner, param2, param3);
            return;
        }

        s_net_pack_monster_update msg;
        msg.param_1 = (int32_t)owner;
        msg.param_2 = (int32_t)param2;
        msg.param_3 = (int32_t)param3;

        NETMOX::Mox_Send_Message_(_net.local_net_id, NET_PACKET_MONSTER_UPDATE, &msg, sizeof(msg));
    }

    void __cdecl Send_Pick_Your_Position_(int16_t network_player_id) {
        allocate::Push_Block_(MOX::_screen_seg);

        s_net_pack_pick_your_position* packet =
            (s_net_pack_pick_your_position*)allocate::Allocate_Next_Block_(MOX::_screen_seg, sizeof(s_net_pack_pick_your_position));

        int32_t num_players = MOX::_NUM_PLAYERS;
        packet->num_players = MOX::_NUM_PLAYERS;

        // Copy the current player list into the network packet
        memcpy(packet->players, MOX::_player, num_players * (int32_t)sizeof(s_player));

        uint16_t msg_size = (uint16_t)(sizeof(packet->num_players) + num_players * (int32_t)sizeof(packet->players[0]));
        NETMOX::Mox_Send_Message_(network_player_id, NET_PACKET_PICK_YOUR_POSITION, packet, msg_size);

        allocate::Pop_Block_(MOX::_screen_seg);
    }

    uint32_t __cdecl Encode_Broadcast_Game_Data_Snapshot_(void* buffer, uint32_t len) {
        Scoped_Net_Update_Suspension_ suspend_net_updates;
        NETPROTO::Packet_Writer_ writer(buffer, len, "invalid broadcast game data payload");

        // Assemble a packed byte stream from primitive values using the original x86 little-endian layout.
        writer.Write_Int16_(_net.host_election_id);
        writer.Write_Uint32_(MOX::_stardate);
        writer.Write_Uint8_(MOX::_settings.random_events);
        writer.Write_Uint8_(MOX::_settings.game_difficulty);
        writer.Write_Uint8_(MOX::_settings.number_of_players);
        writer.Write_Uint8_(MOX::_settings.galaxy_size);
        writer.Write_Uint8_(MOX::_settings.galaxy_age);
        writer.Write_Uint8_(MOX::_settings.strategic_combat_flag);
        writer.Write_Uint8_(MOX::_settings.starting_civilization_level);
        writer.Write_Uint8_(MOX::_settings.xenons_exist);

        RUSS::Mox_Update_();

        writer.Write_Int32_(game_random::Get_Random_Seed_());

        writer.Write_Uint16_((uint16_t)MOX::_NUM_PLAYERS);

        uint32_t size = MOX::_NUM_PLAYERS * sizeof(s_player);
        writer.Write_Bytes_(MOX::_player, size);

        writer.Write_Uint16_((uint16_t)MOX::_NUM_STARS);

        size = (MOX::_NUM_STARS + 1) * sizeof(s_star_data);
        writer.Write_Bytes_(MOX::_star, size);

        writer.Write_Uint16_((uint16_t)MOX::_planet_count);

        size = MOX::_planet_count * sizeof(s_planet_data);
        writer.Write_Bytes_(MOX::_planet, size);

        writer.Write_Uint16_((uint16_t)MOX::_NUM_COLONIES);

        size = MAX_COLONIES * sizeof(s_colony);
        writer.Write_Bytes_(MOX::_colony, size);

        writer.Write_Int16_((int16_t)MOX::_NUM_SHIPS);

        size = MAX_SHIPS * sizeof(s_ship_data);
        writer.Write_Bytes_(MOX::_ship, size);

        RUSS::Mox_Update_();

        size = MAX_LEADERS * sizeof(s_leader_data);
        writer.Write_Bytes_(MOX::_leaders, size);

        writer.Write_Uint16_((uint16_t)REPORT::_n_reports);

        size = sizeof(REPORT::_reports);
        writer.Write_Bytes_(REPORT::_reports, size);

        size = sizeof(EVENTS::_event_data);
        writer.Write_Bytes_(EVENTS::_event_data, size);

        size = MOX::_NUM_PLAYERS;
        writer.Write_Bytes_(MSG_::_first_msg, size);

        writer.Write_Bytes_(MSG_::_max_msgs, size);

        writer.Write_Bytes_(MSG_::_n_msgs, size);

        size = sizeof(MSG_::_msgs);
        writer.Write_Bytes_(MSG_::_msgs, size);

        writer.Write_Uint16_((uint16_t)diplodef::_COUNCIL_FLAG);

        writer.Write_Uint16_((uint16_t)diplodef::_next_council_meeting_turn);

        writer.Write_Bytes_(&MOX::_antarans, sizeof(s_antaran));

        writer.Write_Uint8_(MOX::_NUM_NEBULAS);

        size = MOX::_NUM_NEBULAS * sizeof(s_nebula);
        writer.Write_Bytes_(MOX::_nebula, size);

        writer.Write_Uint32_(EVENTS::_last_event_year);

        writer.Write_Uint16_(EVENTS::_event_delay_count);

        writer.Write_Bytes_(MOX::_bill_savegame, sizeof(MOX::_bill_savegame));

        RUSS::Mox_Update_();

        return writer.Length_();
    }

    void __cdecl Broadcast_Game_Data_Differential_(uint8_t use_diff) {
        Scoped_Net_Update_Suspension_ suspend_net_updates;
        allocate::Push_Block_(MOX::_screen_seg);
        uint8_t* block = (uint8_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, NETMOX_MAX_GAME_DATA_PAYLOAD_BYTES);
        uint8_t* send_ptr;
        int32_t send_size = 0;
        s_net_snapshot_cache* cache;
        uint32_t block_size = Encode_Broadcast_Game_Data_Snapshot_(block, NETMOX_MAX_GAME_DATA_PAYLOAD_BYTES);

        Net_Debug_Log_(
            "Broadcast_Game_Data_Differential_: player=%d stardate=%u use_diff=%u block_size=%u sync=%u",
            (int)MOX::_PLAYER_NUM,
            (unsigned int)MOX::_stardate,
            (unsigned int)use_diff,
            (unsigned int)block_size,
            (unsigned int)_net.sync_state);

        if (use_diff != 0) {
            cache = Open_Net_Snapshot_Cache_(block_size);
            Write_Net_Snapshot_Cache_(cache, block, block_size);
            send_ptr = block;
            send_size = block_size;
        }
        else {
            cache = Open_Net_Snapshot_Cache_(0);
            send_ptr = block + block_size;
            send_size = Make_Difference_Buffer_(cache, block, block_size, send_ptr, NETMOX_MAX_GAME_DATA_PAYLOAD_BYTES - block_size);

            cache = Open_Net_Snapshot_Cache_(block_size);
            Write_Net_Snapshot_Cache_(cache, block, block_size);
        }

        RUSS::Mox_Update_();

        if (send_size > 0) {
            Net_Debug_Log_(
                "Broadcast_Game_Data_Differential_: sending payload bytes=%d use_diff=%u",
                send_size,
                (unsigned int)use_diff);
            Mox_Send_Message_(0x7ffe, NET_PACKET_GAME_DATA_PAYLOAD, send_ptr, (uint32_t)send_size);
        }

        allocate::Pop_Block_(MOX::_screen_seg);
    }

    int8_t __cdecl Decode_Broadcast_Game_Data_Differential_(void* buf, size_t size, uint8_t flag) {
        Scoped_Net_Update_Suspension_ suspend_net_updates;
        uint8_t* buffer = (uint8_t*)buf;
        uint8_t* ptr = (uint8_t*)buf;

        Net_Debug_Log_(
            "Decode_Broadcast_Game_Data_Differential_: player=%d stardate_before=%u flag=%u size=%u",
            (int)MOX::_PLAYER_NUM,
            (unsigned int)MOX::_stardate,
            (unsigned int)flag,
            (unsigned int)size);

        if (flag == 0) {
            ptr = buffer + size;
            s_net_snapshot_cache* cache = NETMOX::Open_Net_Snapshot_Cache_(0);
            size = NETMOX::Decode_Difference_Buffer_(cache, ptr, (int)(NETMOX_MAX_GAME_DATA_PAYLOAD_BYTES - size), buffer, (int)size);

            if (size == 0) {
                exitm::Exit_With_message2_("error decoding broadcast");
                return 0;
            }
        }

        s_net_snapshot_cache* cache = NETMOX::Open_Net_Snapshot_Cache_((uint32_t)size);
        NETMOX::Write_Net_Snapshot_Cache_(cache, ptr, size);

        RUSS::Mox_Update_();

        NETPROTO::Packet_Reader_ reader(ptr, (uint32_t)size, "invalid broadcast game data payload");

        // Interpret the packed byte stream using the original x86 little-endian layout.
        _net.host_election_id = reader.Read_Int16_();

        MOX::_stardate = reader.Read_Uint32_();

        MOX::_settings.random_events = reader.Read_Uint8_();
        MOX::_settings.game_difficulty = reader.Read_Uint8_();
        MOX::_settings.number_of_players = reader.Read_Uint8_();
        MOX::_settings.galaxy_size = reader.Read_Uint8_();
        MOX::_settings.galaxy_age = reader.Read_Uint8_();
        MOX::_settings.strategic_combat_flag = reader.Read_Uint8_();
        MOX::_settings.starting_civilization_level = reader.Read_Uint8_();
        MOX::_settings.xenons_exist = reader.Read_Uint8_();

        game_random::Set_Random_Seed_(reader.Read_Uint32_());

        RUSS::Mox_Update_();

        MOX::_NUM_PLAYERS = reader.Read_Int16_();

        reader.Read_Bytes_(MOX::_player, MOX::_NUM_PLAYERS * sizeof(s_player));

        _net.num_connected_players = 0;
        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; i++) {
            if (MOX::_player[i].eliminated != 0) {
                MOX::_player[i].network_player_id = -1;
                MOX::_player[i].objectives = 0;
            } else {
                if (MOX::_player[i].objectives == PLAYER_OBJECTIVE_HUMAN) {
                    _net.num_connected_players++;
                }
            }
        }

        MOX::_NUM_STARS = reader.Read_Int16_();

        reader.Read_Bytes_(MOX::_star, (MOX::_NUM_STARS + 1) * sizeof(s_star_data));

        RUSS::Mox_Update_();

        MOX::_planet_count = reader.Read_Int16_();

        reader.Read_Bytes_(MOX::_planet, MOX::_planet_count * sizeof(s_planet_data));

        MOX::_NUM_COLONIES = reader.Read_Int16_();

        reader.Read_Bytes_(MOX::_colony, MAX_COLONIES * sizeof(s_colony));

        MOX::_NUM_SHIPS = reader.Read_Int16_();

        reader.Read_Bytes_(MOX::_ship, MAX_SHIPS * sizeof(s_ship_data));

        reader.Read_Bytes_(MOX::_leaders, MAX_LEADERS * sizeof(s_leader_data));

        REPORT::_n_reports = reader.Read_Int16_();

        reader.Read_Bytes_(REPORT::_reports, sizeof(REPORT::_reports));
        REPORT::Client_Initialize_Reports_();

        reader.Read_Bytes_(EVENTS::_event_data, sizeof(EVENTS::_event_data));
        reader.Read_Bytes_(MSG_::_first_msg, MOX::_NUM_PLAYERS);

        reader.Read_Bytes_(MSG_::_max_msgs, MOX::_NUM_PLAYERS);

        reader.Read_Bytes_(MSG_::_n_msgs, MOX::_NUM_PLAYERS);

        reader.Read_Bytes_(MSG_::_msgs, sizeof(MSG_::_msgs));
        RUSS::Mox_Update_();

        diplodef::_COUNCIL_FLAG = reader.Read_Int16_();

        diplodef::_next_council_meeting_turn = reader.Read_Int16_();

        reader.Read_Bytes_(&MOX::_antarans, sizeof(s_antaran));

        MOX::_NUM_NEBULAS = reader.Read_Uint8_();

        reader.Read_Bytes_(MOX::_nebula, MOX::_NUM_NEBULAS * sizeof(s_nebula));

        EVENTS::_last_event_year = reader.Read_Uint32_();

        EVENTS::_event_delay_count = reader.Read_Int16_();

        reader.Read_Bytes_(MOX::_bill_savegame, sizeof(MOX::_bill_savegame));

        RUSS::Mox_Update_();

        Net_Debug_Log_(
            "Decode_Broadcast_Game_Data_Differential_: player=%d stardate_after=%u final_size=%u num_players=%d num_stars=%d num_ships=%d",
            (int)MOX::_PLAYER_NUM,
            (unsigned int)MOX::_stardate,
            (unsigned int)size,
            (int)MOX::_NUM_PLAYERS,
            (int)MOX::_NUM_STARS,
            (int)MOX::_NUM_SHIPS);

        return 1;
    }

    void __cdecl Mox_Net_Update_() {
        if (MOX::_game_type != 2 && MOX::_game_type != 3) {
            return;
        }

        if (NETMOX::_is_updating || NETMOX::_in_receive_message || _suspended_net_update_depth != 0) {
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

        if ((NETMOX::_net.sync_state == NET_SYNC_STATE_READY || NETMOX::_net.sync_state == NET_SYNC_STATE_NEXT_TURN_READY) && 
            !player_phase_ready_sent_flags[current_player_idx]) {
            
            player_phase_ready_sent_flags[current_player_idx] = 1;
            if (MOX::_player[current_player_idx].network_player_id != -1) {
                message_sent = true;
                NETMOX::Mox_Send_Message_(MOX::_player[current_player_idx].network_player_id, NET_PACKET_PLAYER_PHASE_READY, nullptr, 0);
            }
        }

        if (!message_sent && (NETMOX::_net.ai_diplomacy_done_mask & (1 << current_player_idx))) {
            int8_t remote_player_id = NETMOX::_net.player_net_info[current_player_idx].diplomacy_target_id;
            NETMOX::_net.ai_diplomacy_done_mask &= ~(1 << current_player_idx);
            message_sent = true;
            NETMOX::Mox_Send_Message_(MOX::_player[current_player_idx].network_player_id, NET_PACKET_AI_DIPLOMACY_DONE, &remote_player_id, sizeof(remote_player_id));
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
                    if (MOX::_player[current_player_idx].objectives == PLAYER_OBJECTIVE_HUMAN &&
                        !message_sent && NETMOX::_net.player_race_selected_flags[current_player_idx]) {
                        
                        message_sent = true;
                        NETMOX::Mox_Send_Message_(MOX::_player[current_player_idx].network_player_id, NET_PACKET_SYNC_PLAYER_STATE, &sync_pkt, sizeof(sync_pkt));
                    }
                }
                NETMOX::_net.players_sync_mask[i] &= ~(1 << current_player_idx);
            }
        }

        if (!message_sent) {
            if (NETMOX::_net.ai_diplomacy_active_mask & (1 << current_player_idx)) {
                int8_t remote_player_id = NETMOX::_net.player_net_info[current_player_idx].diplomacy_target_id;
                if (!(NETMOX::_net.players_sync_mask[remote_player_id] & (1 << current_player_idx))) {
                    message_sent = true;
                    NETMOX::_net.ai_diplomacy_active_mask &= ~(1 << current_player_idx);
                    NETMOX::Mox_Send_Message_(MOX::_player[current_player_idx].network_player_id, NET_PACKET_AI_DIPLOMACY_BEGIN, &remote_player_id, sizeof(remote_player_id));
                }
            }
        }

        if (!message_sent) {
            if (NETMOX::_net.ai_diplomacy_cancel_mask & (1 << current_player_idx)) {
                int8_t remote_player_id = NETMOX::_net.player_net_info[current_player_idx].diplomacy_target_id;
                NETMOX::_net.ai_diplomacy_cancel_mask &= ~(1 << current_player_idx);
                message_sent = true;
                NETMOX::Mox_Send_Message_(MOX::_player[current_player_idx].network_player_id, NET_PACKET_AI_DIPLOMACY_CANCELLED, &remote_player_id, sizeof(remote_player_id));
            }
        }

        if (!message_sent) {
            if (NETMOX::_net.human_diplomacy_ignored_mask & (1 << current_player_idx)) {
                message_sent = true;
                NETMOX::_net.human_diplomacy_ignored_mask &= ~(1 << current_player_idx);
                NETMOX::Mox_Send_Message_(MOX::_player[current_player_idx].network_player_id, NET_PACKET_HUMAN_DIPLOMACY_IGNORED, nullptr, 0);
            }
        }

        if (!message_sent) {
            int32_t player_num = MOX::_PLAYER_NUM;
            int8_t* player_rank_sync_flags = NETMOX::_net.host_election_rank_sync_cache;
            
            if (current_player_idx != player_num && MOX::_player[current_player_idx].objectives == PLAYER_OBJECTIVE_HUMAN) {
                int8_t local_rank = NETMOX::_net.host_election_player_rank[player_num];
                if (player_rank_sync_flags[current_player_idx] != local_rank) {
                    message_sent = true;
                    s_NET_MSG_RANK rank_msg;
                    rank_msg.rank = (int32_t)local_rank;
                    rank_msg.hit_next_turn = NETMOX::_net.players_hit_next_turn[player_num];
                    player_rank_sync_flags[current_player_idx] = local_rank;
                    NETMOX::Mox_Send_Message_(MOX::_player[current_player_idx].network_player_id, NET_PACKET_HOST_ELECTION_RANK, &rank_msg, sizeof(rank_msg));
                }
            }
        }

        if (!message_sent) {
            if (NETMOX::_net.local_net_id == NETMOX::_net.net_host_id &&
                NETMOX::_net.sync_state >= NET_SYNC_STATE_COMBAT_READY && NETMOX::_net.sync_state < NET_SYNC_STATE_COMBAT_COMPLETE) {
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
                NETMOX::Mox_Send_Message_((int16_t)netadapter::NETADAPTER_BROADCAST_EXCLUDING_SELF, NET_PACKET_PLAYER_COLOR_SELECTED, &color_msg, sizeof(color_msg));
            }
        }

        if (!message_sent) {
            if (NETMOX::_net.player_race_selected_alt_flags[current_player_idx]) {
                message_sent = true;
                NETMOX::_net.player_race_selected_alt_flags[current_player_idx] = 0;
                s_NET_MSG_RACE race_msg;
                race_msg.race = MOX::_player[current_player_idx].race;
                race_msg.player_idx = (int8_t)current_player_idx;
                NETMOX::Mox_Send_Message_((int16_t)netadapter::NETADAPTER_BROADCAST_EXCLUDING_SELF, NET_PACKET_PLAYER_RACE_SELECTED, &race_msg, sizeof(race_msg));
            }
        }

        if (!message_sent) {
            if (NETMOX::_net.player_race_deselected_flags[current_player_idx]) {
                message_sent = true;
                NETMOX::_net.player_race_deselected_flags[current_player_idx] = 0;
                uint8_t* player_race_selection_sync_flags = reinterpret_cast<uint8_t*>(&NETMOX::_net.field146_0x1f3);
                player_race_selection_sync_flags[current_player_idx] = 1;
                uint8_t race = MOX::_player[current_player_idx].race;
                NETMOX::Mox_Send_Message_((int16_t)netadapter::NETADAPTER_BROADCAST_EXCLUDING_SELF, NET_PACKET_PLAYER_RACE_DESELECTED, &race, sizeof(race));
            }
        }

        if (!message_sent) {
            uint8_t* player_race_selection_sync_flags = reinterpret_cast<uint8_t*>(&NETMOX::_net.field146_0x1f3);
            if (player_race_selection_sync_flags[current_player_idx]) {
                message_sent = true;
                player_race_selection_sync_flags[current_player_idx] = 0;
                NETMOX::Mox_Send_Message_(MOX::_player[current_player_idx].network_player_id, NET_PACKET_RACE_SELECTION_SYNCED, nullptr, 0);
            }
        }

        if (!message_sent) {
            if (NETMOX::_net.diplomacy_initiator_id != -1) {
                int8_t target_idx = NETMOX::_net.diplomacy_target_id;
                NETMOX::_net.diplomacy_initiator_id = -1;
                NETMOX::Mox_Send_Message_(MOX::_player[target_idx].network_player_id, NET_PACKET_HUMAN_DIPLOMACY_READY_DATA, &MOX::_player[MOX::_PLAYER_NUM], sizeof(s_player));
            }
        }

        NETMOX::_is_updating = 0;
    }

    void __cdecl Send_Game_Orders_() {
        Scoped_Net_Update_Suspension_ suspend_net_updates;
        allocate::Push_Block_(MOX::_screen_seg);
        uint8_t* packet = reinterpret_cast<uint8_t*>(allocate::Allocate_Next_Block_(MOX::_screen_seg, 350000));
        NETPROTO::Packet_Writer_ writer(packet, 350000, "invalid game orders payload");

        Net_Debug_Log_(
            "Send_Game_Orders_: player=%d stardate=%u sync=%u",
            (int)MOX::_PLAYER_NUM,
            (unsigned int)MOX::_stardate,
            (unsigned int)_net.sync_state);

        writer.Write_Uint32_(MOX::_stardate);
        writer.Write_Uint8_(static_cast<uint8_t>(MOX::_PLAYER_NUM));
        writer.Write_Bytes_(&MOX::_player[MOX::_PLAYER_NUM], sizeof(s_player));

        RUSS::Mox_Update_();

        s_net_pack_star_order star_orders[MAX_STARS];
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
                    strlcpy(order->name, MOX::_star[i].name, sizeof(order->name));
                }

                order->surrender_to = MOX::_star[i].surrender_to[MOX::_PLAYER_NUM];
            }
        }

        writer.Write_Bytes_(star_orders, MOX::_NUM_STARS * sizeof(s_net_pack_star_order));

        RUSS::Mox_Update_();

        writer.Write_Int16_((int16_t)MOX::_NUM_COLONIES);

        if (MOX::_NUM_COLONIES > 0) {
            for (int16_t i = 0; i < (int16_t)MOX::_NUM_COLONIES; ++i) {
                if (MOX::_colony[i].owner == MOX::_PLAYER_NUM) {
                    writer.Write_Int16_(i);
                    writer.Write_Bytes_(&MOX::_colony[i], sizeof(s_colony));
                }
            }
        }

        writer.Write_Int16_(-1);

        RUSS::Mox_Update_();

        writer.Write_Int16_((int16_t)MOX::_NUM_SHIPS);

        if (MOX::_NUM_SHIPS > 0) {
            for (int16_t i = 0; i < (int16_t)MOX::_NUM_SHIPS; ++i) {
                if (MOX::_ship[i].owner == MOX::_PLAYER_NUM && MOX::_ship[i].status != 5) {
                    writer.Write_Int16_(i);
                    writer.Write_Bytes_(&MOX::_ship[i], sizeof(s_ship_data));
                }
            }
        }

        writer.Write_Int16_(-1);

        RUSS::Mox_Update_();

        for (int16_t i = 0; i < MAX_LEADERS; ++i) {
            if (MOX::_leaders[i].player_index == MOX::_PLAYER_NUM) {
                writer.Write_Int16_(i);
                writer.Write_Bytes_(&MOX::_leaders[i], sizeof(s_leader_data));
            }
        }

        writer.Write_Int16_(-1);

        RUSS::Mox_Update_();

        int32_t packet_size = (int32_t)writer.Length_();
        _net.sync_state = NET_SYNC_STATE_COMBAT_READY;
        _net.combat_attacker_id = -1;
        Net_Debug_Log_(
            "Send_Game_Orders_: player=%d packet_size=%d new_sync=%u",
            (int)MOX::_PLAYER_NUM,
            packet_size,
            (unsigned int)_net.sync_state);
        NETMOX::Mox_Send_Message_(_net.local_net_id, NET_PACKET_GAME_ORDERS_PAYLOAD, packet, (uint32_t)packet_size);

        allocate::Pop_Block_(MOX::_screen_seg);
    }

    void __cdecl Send_Drop_Out_Info_(int player_idx, uint16_t msg_type) {
        uint8_t any_processed;
        uint8_t player_process_flags[MAX_PLAYERS];
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

    void __cdecl Host_Send_Player_Disconnected_Info_(int player_idx) {
        if (!Is_Connected_Game_Player_(player_idx)) {
            return;
        }

        int16_t disconnected_network_id = MOX::_player[player_idx].network_player_id;
        s_net_pack_player_disconnected_info_ packet;
        packet.player_idx = (int16_t)player_idx;
        Build_Drop_Out_Info_From_Local_State_(player_idx, &packet.drop_out_info);
        Apply_Drop_Out_Bookkeeping_(player_idx, disconnected_network_id, true);

        Mox_Send_Message_((int16_t)netadapter::NETADAPTER_BROADCAST_EXCLUDING_SELF, NET_PACKET_PLAYER_DISCONNECTED, &packet, (uint16_t)sizeof(packet));
    }

    void __cdecl Handle_Network_Player_Disconnected_(int player_idx) {
        if (_net.is_network_active == 0 || !Is_Connected_Game_Player_(player_idx) || Is_Local_Player_Slot_(player_idx)) {
            return;
        }

        int coordinator_idx = Elect_Disconnect_Recovery_Coordinator_(player_idx);
        if (coordinator_idx >= 0 && Is_Local_Player_Slot_(coordinator_idx)) {
            Host_Send_Player_Disconnected_Info_(player_idx);
        }
    }

    int __cdecl Decode_Difference_Buffer_(s_net_snapshot_cache* cache, uint8_t* out_buffer, int max_out_len, uint8_t* diff_buffer, int diff_len) {
        uint8_t* out_ptr = out_buffer;
        uint8_t* diff_end = diff_buffer + diff_len;

        if (fseek_net_snapshot_cache_(cache, 0, 0) != 0) {
            exitm::Exit_With_message2_("fseek net snapshot cache");
            return 0;
        }

        while (true) {
            if (diff_buffer >= diff_end) {
                return static_cast<int>(out_ptr - out_buffer);
            }

            uint8_t b = *diff_buffer;
            if (b <= 0xF4) {
                uint32_t size = b;
                if (fread_net_snapshot_cache_(out_ptr, size, 1, cache) != 1) {
                    exitm::Exit_With_message2_("fread net snapshot cache");
                    return 0;
                }
                diff_buffer++;
                out_ptr += size;
                RUSS::Mox_Update_();
            } else if (b == 0xFF) {
                diff_buffer++;
                if (diff_buffer > diff_end || out_buffer + max_out_len < out_ptr) {
                    exitm::Exit_With_message2_("p_limit < p_diff");
                    return 0;
                }
            } else {
                uint32_t skip = b - 0xF5;
                if (fseek_net_snapshot_cache_(cache, skip, 1) != 0) {
                    exitm::Exit_With_message2_("fseek ahead net snapshot cache");
                    return 0;
                }
                memcpy(out_ptr, diff_buffer + 1, skip);
                diff_buffer += skip + 1;
                out_ptr += skip;
                RUSS::Mox_Update_();
            }
        }
    }

    size_t __cdecl fread_net_snapshot_cache_(void* ptr, size_t size, size_t count, s_net_snapshot_cache* cache) {
        if (cache->buffer == nullptr) {
            return 0;
        }

        size_t total_size = count * size;
        memcpy(ptr, cache->current_ptr, total_size);

        cache->current_ptr += total_size;

        if (cache->buffer + cache->buffer_size < cache->current_ptr) {
            return 0;
        }

        return count;
    }

    size_t __cdecl fwrite_net_snapshot_cache_(const void* ptr, size_t size, size_t count, s_net_snapshot_cache* cache) {
        if (cache->buffer != nullptr) {
            size_t total_size = count * size;

            // BUG: potential overflow if total_size > (buffer + buffer_size - current_ptr)
            memcpy(cache->current_ptr, ptr, total_size);

            uint8_t* new_ptr = cache->current_ptr + total_size;
            cache->current_ptr = new_ptr;

            if (cache->buffer + cache->buffer_size < new_ptr) {
                return 0;
            }

            return count;
        }

        return 0;
    }

    int32_t __cdecl fgetc_net_snapshot_cache_(s_net_snapshot_cache* cache) {
        if (cache->buffer != nullptr) {
            if (cache->buffer + cache->buffer_size == cache->current_ptr) {
                return -1;
            }
            uint8_t val = *cache->current_ptr;
            cache->current_ptr++;
            return val;
        }

        return 0;
    }

    int32_t __cdecl Make_Difference_Buffer_(s_net_snapshot_cache* cache, uint8_t* buffer, int32_t buffer_size, uint8_t* diff_buffer, int32_t diff_buffer_max_size) {
        uint32_t count = 0;
        uint32_t state = 0;
        uint8_t* ptr = buffer;
        uint8_t* out_ptr = diff_buffer;

        if (NETMOX::fseek_net_snapshot_cache_(cache, 0, 0) != 0) {
            exitm::Exit_With_message2_(NETMOX::_net_snapshot_cache_error);
        }

        do {
            int c = NETMOX::fgetc_net_snapshot_cache_(cache);
            if (c == -1 && ptr != buffer + buffer_size) {
                exitm::Exit_With_message2_(NETMOX::_net_snapshot_cache_error);
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
                        memcpy(out_ptr, ptr - 8, sizeof(uint8_t[9]));
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
                        memcpy(out_ptr, ptr - 8, sizeof(uint8_t[9]));
                        out_ptr += 9;
                        state = 0;
                    } else {
                        state = 2;
                    }
                }
                break;
            }

            if (out_ptr >= diff_buffer + diff_buffer_max_size) {
                exitm::Exit_With_message2_(NETMOX::_net_snapshot_diff_buffer_error);
            }

            ptr++;

            if (((ptr - buffer) % 256) == 0) {
                RUSS::Mox_Update_();
            }
        } while (state != 4);

        *out_ptr = 0xFF;
        return static_cast<int32_t>(out_ptr + 1 - diff_buffer);
    }

    void __cdecl Write_Net_Snapshot_Cache_(s_net_snapshot_cache* cache, const void* buffer, size_t size) {
        RUSS::Mox_Update_();

        if (fseek_net_snapshot_cache_(cache, 0, 0) != 0) {
            exitm::Exit_With_message2_(_net_snapshot_cache_error);
        }

        RUSS::Mox_Update_();

        if (fwrite_net_snapshot_cache_(buffer, size, 1, cache) != 1) {
            exitm::Exit_With_message2_(_net_snapshot_cache_error);
        }

        RUSS::Mox_Update_();
    }

    void __cdecl Net_Request_Human_Diplomacy_(uint8_t target_player_id) {
        _net.diplomacy_target_id = (int8_t)target_player_id;

        if (((_net.players_active_mask >> (target_player_id & 0x1F)) & 1) != 0) {
            _net.sync_state = NET_SYNC_STATE_HUMAN_DIPLOMACY_CONNECTING;
            _net.diplomacy_active_flag = 1;
            _net.diplomacy_initiator_id = (int8_t)target_player_id;

            uint8_t mask = (uint8_t)~(1 << (target_player_id & 0x1F));
            _net.players_active_mask &= mask;
            _net.players_ready_mask[0] &= mask;
            _net.is_diplomacy_initiator = 0;

            Mox_Send_Message_(
                MOX::_player[target_player_id].network_player_id,
                NET_PACKET_ACCEPT_HUMAN_DIPLOMACY,
                MOX::_player + MOX::_PLAYER_NUM,
                sizeof(MOX::_player[MOX::_PLAYER_NUM])
            );
        } else {
            _net.sync_state = NET_SYNC_STATE_HUMAN_DIPLOMACY_REQUESTED;
            _net.diplomacy_active_flag = 0;

            if (Human_Hit_Next_Turn_(target_player_id) == 0) {
                s_net_pack_req_human_diplomacy packet;

                memcpy(&packet.player, MOX::_player + MOX::_PLAYER_NUM, sizeof(s_player));

                for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
                    packet.surrender_to[i] = MOX::_star[i].surrender_to[MOX::_PLAYER_NUM];
                }

                Mox_Send_Message_(
                    MOX::_player[target_player_id].network_player_id,
                    NET_PACKET_REQUEST_HUMAN_DIPLOMACY,
                    &packet,
                    sizeof(packet)
                );
            }
        }
    }

    int __cdecl fseek_net_snapshot_cache_(s_net_snapshot_cache* cache, int offset, int origin) {
        if (cache->buffer == nullptr) {
            return -1;
        }

        if (origin == 0 /* SEEK_SET */) {
            cache->current_ptr = cache->buffer + offset;
            return 0;
        }
        if (origin == 1 /* SEEK_CUR */) {
            cache->current_ptr = cache->current_ptr + offset;
            return 0;
        }

        return -1;
    }

    void __cdecl Decode_Game_Orders_(void* data, uint32_t len) {
        Scoped_Net_Update_Suspension_ suspend_net_updates;
        NETPROTO::Packet_Reader_ reader(data, len, "invalid game orders payload");

        reader.Skip_(sizeof(uint32_t));

        uint8_t player_idx = reader.Read_Uint8_();

        Net_Debug_Log_(
            "Decode_Game_Orders_: local_player=%d sender_player=%u len=%u stardate=%u",
            (int)MOX::_PLAYER_NUM,
            (unsigned int)player_idx,
            (unsigned int)len,
            (unsigned int)MOX::_stardate);

        if (player_idx >= MAX_PLAYERS || player_idx >= MOX::_NUM_PLAYERS) {
            exitm::Exit_With_Message("invalid game orders payload");
        }

        reader.Require_(sizeof(s_player) + MOX::_NUM_STARS * sizeof(s_net_pack_star_order) + sizeof(int16_t));

        reader.Read_Bytes_(&MOX::_player[player_idx], sizeof(s_player));

        RUSS::Mox_Update_();

        uint8_t star_updates[MAX_STARS * sizeof(s_net_pack_star_order)];
        reader.Read_Bytes_(star_updates, MOX::_NUM_STARS * sizeof(s_net_pack_star_order));

        if (MOX::_NUM_STARS > 0) {
            for (int i = 0; i < MOX::_NUM_STARS; ++i) {
                uint8_t* update = star_updates + i * sizeof(s_net_pack_star_order);
                uint32_t flags;
                memcpy(&flags, update, sizeof(flags));
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
                    strlcpy(star->name, (const char*)(update + 4), sizeof(star->name));
                }

                star->surrender_to[player_idx] = update[0x17];
            }
        }

        RUSS::Mox_Update_();

        int16_t max_colonies = reader.Read_Int16_();
        if (MOX::_NUM_COLONIES < max_colonies) {
            MOX::_NUM_COLONIES = max_colonies;
        }

        while (true) {
            int16_t colony_idx = reader.Read_Int16_();
            if (colony_idx == -1) {
                break;
            }

            reader.Read_Bytes_(&MOX::_colony[colony_idx], sizeof(s_colony));

            int16_t planet_idx = MOX::_colony[colony_idx].planet;
            if (MOX::_planet[planet_idx].colony_index != colony_idx) {
                MOX::_planet[planet_idx].colony_index = colony_idx;
            }

            RUSS::Mox_Update_();
        }

        RUSS::Mox_Update_();

        int16_t max_ships = reader.Read_Int16_();
        if (MOX::_NUM_SHIPS < max_ships) {
            MOX::_NUM_SHIPS = max_ships;
        }

        int ship_idx_prev = 0;
        while (true) {
            int16_t ship_idx = reader.Read_Int16_();
            if (ship_idx == -1) {
                break;
            }

            for (int i = ship_idx_prev; i < ship_idx; ++i) {
                if (MOX::_ship[i].owner == player_idx && MOX::_ship[i].status != 5) {
                    MOX::_ship[i].officer_index = -1;
                    SHIPS::Kill_Ship_(i);
                }
            }

            reader.Read_Bytes_(&MOX::_ship[ship_idx], sizeof(s_ship_data));

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
            int16_t leader_idx = reader.Read_Int16_();
            if (leader_idx == -1) {
                break;
            }

            for (int i = leader_idx_prev; i < leader_idx; ++i) {
                if (MOX::_leaders[i].player_index == player_idx) {
                    NETMOX::Ditch_Leader_(i);
                }
            }

            reader.Read_Bytes_(&MOX::_leaders[leader_idx], sizeof(s_leader_data));

            leader_idx_prev = leader_idx + 1;
        }

        for (int i = leader_idx_prev; i < MAX_LEADERS; ++i) {
            if (MOX::_leaders[i].player_index == player_idx) {
                NETMOX::Ditch_Leader_(i);
            }
        }

        RUSS::Mox_Update_();
    }

    s_net_snapshot_cache* __cdecl Open_Net_Snapshot_Cache_(uint32_t size) {
        RUSS::Mox_Update_();

        if (_net_snapshot_cache.buffer != nullptr && size != 0 && size != _net_snapshot_cache.buffer_size) {
            free(_net_snapshot_cache.buffer);
            _net_snapshot_cache.buffer = nullptr;
        }

        if (_net_snapshot_cache.buffer == nullptr) {
            if (size == 0) {
                exitm::Exit_With_message2_(_net_snapshot_cache_error);
                return &_net_snapshot_cache;
            }

            _net_snapshot_cache.buffer = static_cast<uint8_t*>(malloc(size));
            if (_net_snapshot_cache.buffer == nullptr) {
                exitm::Exit_With_message2_(_net_snapshot_cache_error);
                return &_net_snapshot_cache;
            }

            _net_snapshot_cache.buffer_size = size;
        }

        _net_snapshot_cache.current_ptr = _net_snapshot_cache.buffer;
        return &_net_snapshot_cache;
    }

    int16_t __cdecl Get_Next_AI_Talker_(int32_t target_player_id) {
        uint8_t weights[MAX_PLAYERS] = { 0 };
        uint8_t max_weight = 0;
        bool already_talking = false;

        if (MOX::_NUM_PLAYERS <= 0) {
            return -1;
        }

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (NETMOX::_net.player_net_info[i].diplomacy_target_id == target_player_id) {
                if (NETMOX::_net.player_net_info[i].player_state == NET_PLAYER_STATE_AI_DIPLOMACY_ACTIVE) {
                    already_talking = true;
                }
                else {
                    // Preserve assembly logic exactly:
                    // TRAIT_CHARISMATIC != 0 => 3, else TRAIT_REPULSIVE == 0 => 2, else 1
                    if (MOX::_player[i].traits[TRAIT_CHARISMATIC] != 0) {
                        weights[i] = 3;
                    }
                    else {
                        weights[i] = 1;
                        if (MOX::_player[i].traits[TRAIT_REPULSIVE] == 0) {
                            weights[i] = 2;
                        }
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
        NETPROTO::Packet_Reader_ reader(packet_data, len, "invalid combat payload");

        // 1. Read Combat Star ID
        reader.Require_(sizeof(int16_t) + sizeof(s_combfind_target));
        int16_t combat_star_id = reader.Read_Int16_();

        NETMOX::_net.combat_attacker_id = static_cast<int8_t>(MOX::_PLAYER_NUM);
        NETMOX::_net.combat_star_id = static_cast<int8_t>(combat_star_id);

        // 2. Read Target Info
        s_combfind_target target_info;
        reader.Read_Bytes_(&target_info, sizeof(s_combfind_target));

        COMBFIND::Copy_Target_Info_(&target_info);
        RUSS::Mox_Update_();

        // If we are not the host, unpack the state data
        if (NETMOX::_net.local_net_id != NETMOX::_net.net_host_id) {

            // Pass the remaining buffer pointer to Decode_Star_Player_Presence_
            reader.Require_(k_star_player_presence_payload_bytes);
            COMBFIND::Decode_Star_Player_Presence_(&reader, -1);

            // 3. Read Player Tech Applications
            if (MOX::_NUM_PLAYERS > 0) {
                for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                    reader.Read_Bytes_(MOX::_player[i].tech_applications, sizeof(MOX::_player[i].tech_applications));
                }
            }

            // 4. Read Leaders
            for (int i = 0; i < MAX_LEADERS; ++i) {
                MOX::_leaders[i].player_index = -1;
            }

            while (true) {
                int16_t leader_id = reader.Read_Int16_();

                if (leader_id == -1) {
                    break;
                }

                reader.Read_Bytes_(&MOX::_leaders[leader_id], sizeof(s_leader_data));
                RUSS::Mox_Update_();
            }
            RUSS::Mox_Update_();

            // 5. Read Star Officer Indexes
            if (MOX::_NUM_PLAYERS > 0) {
                for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                    MOX::_star[combat_star_id].officer_index[i] = static_cast<char>(reader.Read_Uint8_());
                }
            }
            RUSS::Mox_Update_();

            // 6. Read Planets and Colonies
            MOX::_NUM_COLONIES = reader.Read_Int16_();

            for (int i = 0; i < 5; ++i) {
                int16_t planet_idx = MOX::_star[combat_star_id].planet_index[i];

                if (planet_idx != -1) {
                    int16_t dummy_col_idx = reader.Read_Int16_();
                    int8_t p_type = static_cast<int8_t>(reader.Read_Uint8_());
                    char p_climate = static_cast<char>(reader.Read_Uint8_());
                    int16_t real_col_idx = reader.Read_Int16_();

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
                        reader.Read_Bytes_(&MOX::_colony[MOX::_planet[planet_idx].colony_index], sizeof(s_colony));
                    }
                }
            }
            RUSS::Mox_Update_();

            // 7. Read Ships
            MOX::_NUM_SHIPS = reader.Read_Int16_();

            if (MOX::_NUM_SHIPS > 0) {
                for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
                    MOX::_ship[i].status = 5; // Status 5 = travelling / in combat
                }
            }

            while (true) {
                int16_t ship_id = reader.Read_Int16_();

                if (ship_id == -1) {
                    break;
                }

                reader.Read_Bytes_(&MOX::_ship[ship_id], sizeof(s_ship_data));

                RUSS::Mox_Update_();
            }
        }

        // 8. Finalize synchronization state
        MAINSCR::Update_Ship_Icons_In_Between_Turns_();

        NETMOX::_net.player_net_info[MOX::_PLAYER_NUM].player_state = NET_PLAYER_STATE_PICK_TARGET_INFO;
        NETMOX::_net.sync_state = NET_SYNC_STATE_PICKING_COMBAT_TARGET;
    }

    void __cdecl Decode_You_Are_Target_Info_(void* packet_data, uint32_t len) {
        NETPROTO::Packet_Reader_ reader(packet_data, len, "invalid combat payload");

        // 1. Read Header
        reader.Require_(sizeof(s_packet_0x12));
        int16_t star_id = reader.Read_Int16_();
        int16_t attacker_id = reader.Read_Int16_();
        int16_t defender_id = reader.Read_Int16_();
        int16_t target_colony_id = reader.Read_Int16_();

        NETMOX::_net.combat_star_id = static_cast<int8_t>(star_id);
        NETMOX::_net.combat_attacker_id = static_cast<int8_t>(attacker_id);
        NETMOX::_net.combat_defender_id = defender_id;
        NETMOX::_net.combat_target_player_id = MOX::_PLAYER_NUM;

        // If we are not the host, unpack the state data
        if (NETMOX::_net.local_net_id != NETMOX::_net.net_host_id) {

            // Pass the remaining buffer pointer
            reader.Require_(k_star_player_presence_payload_bytes);
            COMBFIND::Decode_Star_Player_Presence_(&reader, -1);

            // 2. Read Player Tech Applications
            if (attacker_id < MAX_PLAYERS) {
                reader.Read_Bytes_(MOX::_player[attacker_id].tech_applications, sizeof(MOX::_player[attacker_id].tech_applications));
            }

            if (MOX::_PLAYER_NUM < MAX_PLAYERS) {
                reader.Read_Bytes_(MOX::_player[MOX::_PLAYER_NUM].tech_applications, sizeof(MOX::_player[MOX::_PLAYER_NUM].tech_applications));
            }

            // 3. Read Leaders
            for (int i = 0; i < MAX_LEADERS; ++i) {
                int16_t owner = MOX::_leaders[i].player_index;
                if (owner == attacker_id || owner == MOX::_PLAYER_NUM) {
                    MOX::_leaders[i].player_index = -1;
                }
            }

            while (true) {
                int16_t leader_id = reader.Read_Int16_();

                if (leader_id == -1) {
                    break;
                }

                reader.Read_Bytes_(&MOX::_leaders[leader_id], sizeof(s_leader_data));
            }

            // 4. Read Star Officer Indexes
            if (attacker_id < MAX_PLAYERS) {
                MOX::_star[star_id].officer_index[attacker_id] = static_cast<char>(reader.Read_Uint8_());
            }

            if (MOX::_PLAYER_NUM < MAX_PLAYERS) {
                MOX::_star[star_id].officer_index[MOX::_PLAYER_NUM] = static_cast<char>(reader.Read_Uint8_());
            }

            // 5. Read Targeted Colony Data
            if (target_colony_id != -1) {
                MOX::_NUM_COLONIES = reader.Read_Int16_();

                int16_t planet_idx = reader.Read_Int16_();

                int16_t dummy_col_idx = reader.Read_Int16_();

                int8_t p_type = static_cast<int8_t>(reader.Read_Uint8_());

                char p_climate = static_cast<char>(reader.Read_Uint8_());

                MOX::_planet[planet_idx].colony_index = dummy_col_idx;
                MOX::_planet[planet_idx].planet_type = p_type;
                MOX::_planet[planet_idx].climate = p_climate;

                reader.Read_Bytes_(&MOX::_colony[target_colony_id], sizeof(s_colony));
            }

            // 6. Read Ships
            MOX::_NUM_SHIPS = reader.Read_Int16_();

            if (MOX::_NUM_SHIPS > 0) {
                for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
                    MOX::_ship[i].status = 5; // Status 5 = travelling / in combat
                }
            }

            while (true) {
                int16_t ship_id = reader.Read_Int16_();

                if (ship_id == -1) {
                    break;
                }

                reader.Read_Bytes_(&MOX::_ship[ship_id], sizeof(s_ship_data));
            }
        }

        // 7. Finalize synchronization state
        NETMOX::_net.combat_planet_id = COMBFIND::Target_Colony_To_Planet_(target_colony_id);
        NETMOX::_net.sync_state = NET_SYNC_STATE_COMBAT_RESOLUTION_DISPATCH;
    }

    void __cdecl Decode_Pick_Your_Position_(s_net_pack_pick_your_position* packet, uint16_t len) {
        (void)len;
        MOX::_NUM_PLAYERS = packet->num_players;
        memcpy(MOX::_player, packet->players, packet->num_players * sizeof(s_player));
    }

    void __cdecl Decode_Battle_Results_(void* packet_data, uint32_t len) {
        NETPROTO::Packet_Reader_ reader(packet_data, len, "invalid combat payload");

        // 1. Read Header
        reader.Require_(sizeof(int16_t) * 4);
        int16_t reporting_player_id = reader.Read_Int16_();
        int16_t attacker_id = reader.Read_Int16_();
        int16_t defender_id = reader.Read_Int16_();
        int16_t combat_status = reader.Read_Int16_();

        NETMOX::_net.player_net_info[attacker_id].combat_result = (int8_t)combat_status;
        NETMOX::_net.player_net_info[defender_id].combat_result = (int8_t)combat_status;

        RUSS::Mox_Update_();

        // 2. Filter: Only unpack if we aren't the reporter
        if (MOX::_PLAYER_NUM != reporting_player_id) {

            bool skip_unpack = false;
            // Do not unpack if the reporter is the defender, the attacker is a valid player,
            // and the attacker is a human player (objective == 100 / 'd')
            if (defender_id == reporting_player_id && attacker_id < MAX_PLAYERS) {
                if (MOX::_player[attacker_id].objectives == PLAYER_OBJECTIVE_HUMAN) {
                    skip_unpack = true;
                }
            }

            if (!skip_unpack) {
                reader.Require_(sizeof(int16_t) + k_star_player_presence_payload_bytes + sizeof(int16_t) * 2);
                int16_t combat_star_id = reader.Read_Int16_();

                COMBFIND::Decode_Star_Player_Presence_(&reader, combat_star_id);

                reader.Skip_(sizeof(int16_t));

                int16_t combat_planet_idx = reader.Read_Int16_();

                RUSS::Mox_Update_();

                // 3. Unpack Tech Applications and Officer Indexes
                if (attacker_id < MAX_PLAYERS) {
                    reader.Read_Bytes_(MOX::_player[attacker_id].tech_applications, sizeof(MOX::_player[attacker_id].tech_applications));
                    MOX::_star[combat_star_id].officer_index[attacker_id] = static_cast<char>(reader.Read_Uint8_());
                }

                if (defender_id < MAX_PLAYERS) {
                    reader.Read_Bytes_(MOX::_player[defender_id].tech_applications, sizeof(MOX::_player[defender_id].tech_applications));
                    MOX::_star[combat_star_id].officer_index[defender_id] = static_cast<char>(reader.Read_Uint8_());
                }

                // 4. Unpack Surviving Leaders
                while (true) {
                    int16_t leader_id = reader.Read_Int16_();

                    if (leader_id == -1) {
                        break;
                    }

                    reader.Read_Bytes_(&MOX::_leaders[leader_id], sizeof(s_leader_data));
                }
                RUSS::Mox_Update_();

                // 5. Unpack Colony Fate
                if (combat_planet_idx != -1) {
                    int16_t old_colony_index = MOX::_planet[combat_planet_idx].colony_index;

                    int16_t new_colony_index = reader.Read_Int16_();
                    int8_t p_type = static_cast<int8_t>(reader.Read_Uint8_());
                    char p_climate = static_cast<char>(reader.Read_Uint8_());
                    int16_t actual_col_idx = reader.Read_Int16_();

                    MOX::_planet[combat_planet_idx].colony_index = new_colony_index;
                    MOX::_planet[combat_planet_idx].planet_type = p_type;
                    MOX::_planet[combat_planet_idx].climate = p_climate;

                    if (actual_col_idx == -1) {
                        if (old_colony_index != -1) {
                            BOMB::Destroy_Colony_(&MOX::_colony[old_colony_index], 1);
                        }
                    }
                    else {
                        reader.Read_Bytes_(&MOX::_colony[actual_col_idx], sizeof(s_colony));
                        COLCALC::Compute_Star_Colony_Stuff_(&MOX::_star[combat_star_id]);
                    }
                }
                RUSS::Mox_Update_();

                // 6. Unpack Ships and Process Casualties
                MOX::_NUM_SHIPS = reader.Read_Int16_();

                int16_t current_ship_idx = 0;

                while (true) {
                    int16_t surviving_ship_id = reader.Read_Int16_();

                    if (surviving_ship_id == -1) {
                        RUSS::Mox_Update_();
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
                    reader.Read_Bytes_(&MOX::_ship[surviving_ship_id], sizeof(s_ship_data));

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
        NETMOX::_net.player_net_info[reporting_player_id].player_state = NET_PLAYER_STATE_HUMAN_DIPLOMACY_ACTIVE;
    }

    void __cdecl Decode_Dragon_Info_(int32_t* colony_id_ptr, uint16_t len) {
        (void)len;
        int16_t planet_idx = MOX::_colony[*colony_id_ptr].planet;

        _net.combat_planet_id = planet_idx;
        _net.combat_star_id = MOX::_planet[planet_idx].star_index;
    }

    void __cdecl Send_Dragon_Info_(int32_t colony_id) {
        s_packet_dragon_info packet;
        packet.colony_id = colony_id;

        int8_t owner = MOX::_colony[colony_id].owner;
        int16_t network_player_id = MOX::_player[owner].network_player_id;

        Mox_Send_Message_(network_player_id, NET_PACKET_DRAGON_INFO, &packet, (uint16_t)sizeof(packet));
    }

    void __cdecl Decode_Drop_Out_Info_(int32_t player_idx, s_net_pack_drop_out_info* drop_out_info) {
        Apply_Drop_Out_Info_(player_idx, drop_out_info, false);
    }

    void __cdecl Decode_Player_Disconnected_Info_(void* data, uint32_t len) {
        if (data == nullptr || len != sizeof(s_net_pack_player_disconnected_info_)) {
            return;
        }

        s_net_pack_player_disconnected_info_* packet = (s_net_pack_player_disconnected_info_*)data;
        Apply_Drop_Out_Info_(packet->player_idx, &packet->drop_out_info, true);
    }

    void __cdecl Send_Battle_Results_(int16_t star_id, int16_t player_id, int16_t target_player_id, int16_t combat_defender_id, int16_t combat_planet_id, int16_t combat_result) {
        int16_t player_num = MOX::_PLAYER_NUM;
        bool include_remote_state =
            _net.local_net_id != _net.net_host_id &&
            (player_num != target_player_id || player_id >= MAX_PLAYERS || MOX::_player[player_id].objectives != PLAYER_OBJECTIVE_HUMAN);

        allocate::Push_Block_(MOX::_screen_seg);
        uint32_t alloc_size = include_remote_state ? k_medium_network_payload_bytes : sizeof(int16_t) * 4;
        uint8_t* packet = (uint8_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, alloc_size);
        NETPROTO::Packet_Writer_ writer(packet, alloc_size, "invalid combat payload");

        writer.Write_Int16_(player_num);
        writer.Write_Int16_(player_id);
        writer.Write_Int16_(target_player_id);
        writer.Write_Int16_(combat_result);

        if (include_remote_state) {
            writer.Write_Int16_(star_id);

            writer.Require_(k_star_player_presence_payload_bytes);
            COMBFIND::Send_Star_Player_Presence_(&writer, -1);

            writer.Write_Int16_(combat_defender_id);
            writer.Write_Int16_(combat_planet_id);

            RUSS::Mox_Update_();

            if (player_id < MAX_PLAYERS) {
                writer.Write_Bytes_(MOX::_player[player_id].tech_applications, sizeof(MOX::_player[player_id].tech_applications));
                writer.Write_Uint8_((uint8_t)MOX::_star[star_id].officer_index[player_id]);
            }

            if (target_player_id < MAX_PLAYERS) {
                writer.Write_Bytes_(MOX::_player[target_player_id].tech_applications, sizeof(MOX::_player[target_player_id].tech_applications));
                writer.Write_Uint8_((uint8_t)MOX::_star[star_id].officer_index[target_player_id]);
            }

            RUSS::Mox_Update_();

            for (int16_t i = 0; i < MAX_LEADERS; i++) {
                if (MOX::_leaders[i].player_index == player_id || MOX::_leaders[i].player_index == target_player_id) {
                    writer.Write_Int16_(i);
                    writer.Write_Bytes_(&MOX::_leaders[i], sizeof(s_leader_data));
                }
            }

            writer.Write_Int16_(-1);

            RUSS::Mox_Update_();

            if (combat_planet_id != -1) {
                int16_t colony_idx = MOX::_planet[combat_planet_id].colony_index;
                writer.Write_Int16_(colony_idx);
                writer.Write_Int8_(MOX::_planet[combat_planet_id].planet_type);
                writer.Write_Char_(MOX::_planet[combat_planet_id].climate);
                writer.Write_Int16_(colony_idx);

                if (colony_idx != -1) {
                    writer.Write_Bytes_(&MOX::_colony[colony_idx], sizeof(s_colony));
                }
            }

            RUSS::Mox_Update_();

            writer.Write_Int16_((int16_t)MOX::_NUM_SHIPS);

            if (MOX::_NUM_SHIPS > 0) {
                for (int16_t i = 0; i < (int16_t)MOX::_NUM_SHIPS; i++) {
                    if (MOX::_ship[i].location == star_id && MOX::_ship[i].status != 5) {
                        if (MOX::_ship[i].owner == player_id || MOX::_ship[i].owner == target_player_id) {
                            writer.Write_Int16_(i);
                            writer.Write_Bytes_(&MOX::_ship[i], sizeof(s_ship_data));
                        }
                    }
                }
            }

            writer.Write_Int16_(-1);

            RUSS::Mox_Update_();
        }

        int32_t size = (int32_t)writer.Length_();
        NETMOX::Mox_Send_Message_(_net.local_net_id, NET_PACKET_BATTLE_RESULTS, packet, (uint32_t)size);

        allocate::Pop_Block_(MOX::_screen_seg);
    }

    void __cdecl Send_You_Are_Target_Info_(
        int16_t target_player_idx,
        int16_t other_player_idx,
        int16_t defender_id,
        int16_t colony_idx,
        int16_t star_idx)
    {
        allocate::Push_Block_(MOX::_screen_seg);

        uint32_t alloc_size =
            (target_player_idx == MOX::_PLAYER_NUM) ? sizeof(s_packet_0x12) : k_medium_network_payload_bytes;
        uint8_t* packet = (uint8_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, alloc_size);
        NETPROTO::Packet_Writer_ writer(packet, alloc_size, "invalid combat payload");

        writer.Write_Int16_(star_idx);
        writer.Write_Int16_(other_player_idx);
        writer.Write_Int16_(defender_id);
        writer.Write_Int16_(colony_idx);

        if (MOX::_player[target_player_idx].network_player_id != _net.local_net_id) {
            writer.Require_(k_star_player_presence_payload_bytes);
            COMBFIND::Send_Star_Player_Presence_(&writer, star_idx);
            RUSS::Mox_Update_();

            if (other_player_idx < MAX_PLAYERS) {
                writer.Write_Bytes_(MOX::_player[other_player_idx].tech_applications, sizeof(MOX::_player[other_player_idx].tech_applications));
            }

            if (target_player_idx < MAX_PLAYERS) {
                writer.Write_Bytes_(MOX::_player[target_player_idx].tech_applications, sizeof(MOX::_player[target_player_idx].tech_applications));
            }

            RUSS::Mox_Update_();

            for (int16_t i = 0; i < MAX_LEADERS; ++i) {
                if (MOX::_leaders[i].player_index == other_player_idx || MOX::_leaders[i].player_index == target_player_idx) {
                    writer.Write_Int16_(i);
                    writer.Write_Bytes_(&MOX::_leaders[i], sizeof(s_leader_data));
                }
            }

            writer.Write_Int16_(-1);

            if (other_player_idx < MAX_PLAYERS) {
                writer.Write_Uint8_((uint8_t)MOX::_star[star_idx].officer_index[other_player_idx]);
            }

            if (target_player_idx < MAX_PLAYERS) {
                writer.Write_Uint8_((uint8_t)MOX::_star[star_idx].officer_index[target_player_idx]);
            }

            RUSS::Mox_Update_();

            if (colony_idx != -1) {
                int16_t planet_idx = MOX::_colony[colony_idx].planet;

                writer.Write_Int16_((int16_t)MOX::_NUM_COLONIES);
                writer.Write_Int16_(planet_idx);
                writer.Write_Int16_(MOX::_planet[planet_idx].colony_index);
                writer.Write_Int8_(MOX::_planet[planet_idx].planet_type);
                writer.Write_Char_(MOX::_planet[planet_idx].climate);
                writer.Write_Bytes_(&MOX::_colony[colony_idx], sizeof(s_colony));
            }

            RUSS::Mox_Update_();

            writer.Write_Int16_((int16_t)MOX::_NUM_SHIPS);

            if (MOX::_NUM_SHIPS > 0) {
                for (int16_t i = 0; i < (int16_t)MOX::_NUM_SHIPS; ++i) {
                    if (MOX::_ship[i].location == star_idx && MOX::_ship[i].status != 5) {
                        if (MOX::_ship[i].owner == other_player_idx || MOX::_ship[i].owner == target_player_idx) {
                            writer.Write_Int16_(i);
                            writer.Write_Bytes_(&MOX::_ship[i], sizeof(s_ship_data));
                        }
                    }
                }
            }

            writer.Write_Int16_(-1);

            RUSS::Mox_Update_();
        }

        int32_t msg_len = (int32_t)writer.Length_();
        NETMOX::Mox_Send_Message_(MOX::_player[target_player_idx].network_player_id, NET_PACKET_YOU_ARE_TARGET_INFO, packet, (uint32_t)msg_len);

        allocate::Pop_Block_(MOX::_screen_seg);
    }

    void __cdecl Send_Pick_Target_Info_(int16_t player_idx, int16_t star_idx, s_combfind_target* target_info) {
        allocate::Push_Block_(MOX::_screen_seg);

        uint32_t alloc_size =
            (player_idx == MOX::_PLAYER_NUM) ? (sizeof(int16_t) + sizeof(s_combfind_target)) : k_medium_network_payload_bytes;
        uint8_t* packet = static_cast<uint8_t*>(allocate::Allocate_Next_Block_(MOX::_screen_seg, alloc_size));
        NETPROTO::Packet_Writer_ writer(packet, alloc_size, "invalid combat payload");

        writer.Write_Int16_(star_idx);
        writer.Write_Bytes_(target_info, sizeof(s_combfind_target));

        if (MOX::_player[player_idx].network_player_id != _net.local_net_id) {
            writer.Require_(k_star_player_presence_payload_bytes);
            COMBFIND::Send_Star_Player_Presence_(&writer, star_idx);

            if (MOX::_NUM_PLAYERS > 0) {
                for (int16_t i = 0; i < MOX::_NUM_PLAYERS; i++) {
                    writer.Write_Bytes_(MOX::_player[i].tech_applications, sizeof(MOX::_player[i].tech_applications));
                }
            }

            RUSS::Mox_Update_();

            for (int16_t i = 0; i < MAX_LEADERS; i++) {
                writer.Write_Int16_(i);
                writer.Write_Bytes_(&MOX::_leaders[i], sizeof(s_leader_data));
                RUSS::Mox_Update_();
            }

            writer.Write_Int16_(-1);

            if (MOX::_NUM_PLAYERS > 0) {
                for (int16_t i = 0; i < MOX::_NUM_PLAYERS; i++) {
                    writer.Write_Uint8_((uint8_t)MOX::_star[star_idx].officer_index[i]);
                }
            }

            RUSS::Mox_Update_();

            writer.Write_Int16_((int16_t)MOX::_NUM_COLONIES);

            for (int i = 0; i < 5; i++) {
                int16_t planet_idx = MOX::_star[star_idx].planet_index[i];
                if (planet_idx != -1) {
                    writer.Write_Int16_(MOX::_planet[planet_idx].colony_index);
                    writer.Write_Int8_(MOX::_planet[planet_idx].planet_type);
                    writer.Write_Char_(MOX::_planet[planet_idx].climate);
                    writer.Write_Int16_(MOX::_planet[planet_idx].colony_index);

                    if (MOX::_planet[planet_idx].colony_index != -1) {
                        writer.Write_Bytes_(&MOX::_colony[MOX::_planet[planet_idx].colony_index], sizeof(s_colony));
                    }
                }
            }

            RUSS::Mox_Update_();

            writer.Write_Int16_((int16_t)MOX::_NUM_SHIPS);

            if (MOX::_NUM_SHIPS > 0) {
                for (int16_t i = 0; i < (int16_t)MOX::_NUM_SHIPS; i++) {
                    if (MOX::_ship[i].location == star_idx && MOX::_ship[i].status != 5) {
                        writer.Write_Int16_(i);
                        writer.Write_Bytes_(&MOX::_ship[i], sizeof(s_ship_data));
                        RUSS::Mox_Update_();
                    }
                }
            }

            writer.Write_Int16_(-1);
        }

        RUSS::Mox_Update_();

        int32_t diff = static_cast<int32_t>(writer.Length_());
        NETMOX::Mox_Send_Message_(MOX::_player[player_idx].network_player_id, NET_PACKET_PICK_TARGET_INFO, packet, (uint32_t)diff);

        allocate::Pop_Block_(MOX::_screen_seg);
    }
}