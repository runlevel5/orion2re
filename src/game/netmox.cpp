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

        if (NETMOX::_net.packet_type >= 0x25) {
            if (NETMOX::_net.packet_type <= 0x26) {
                return 0;
            }

            if (NETMOX::_net.packet_type == 0x28) {
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

    int32_t __cdecl Net_Diplomacy_With_(int16_t limit) {
        return _net.diplomacy_target_id;
    }

    void __cdecl Net_Cancel_Human_Diplomacy_Request_() {
        if (MOX::_game_type >= 2 && MOX::_game_type <= 3) {
            int32_t target_id = _net.diplomacy_target_id;
            _net.diplomacy_target_id = -1;
            _net.packet_type = 4;

            int16_t network_player_id = MOX::_player[target_id].network_player_id;
            if (network_player_id != -1) {
                Mox_Send_Message_(network_player_id, 0x1F, nullptr, 0);
            }
        }
    }

    void __cdecl Net_End_Human_Diplomacy_() {
        _net.diplomacy_target_id = -1;
        _net.packet_type = 4;
    }

    bool __cdecl Net_Human_Rejected_Diplomacy_() {
        bool result = false;

        if (_net.packet_type == 0x2A /* '*' */) {
            _net.packet_type = 0x04;
            result = true;
        }

        return result;
    }

    bool __cdecl Allowed_To_Cancel_Human_Diplomacy_() {
        return _net.packet_type == 0x29;
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

        switch (_net.packet_type) {
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
        return _net.packet_type == 4;
    }

    void __cdecl Net_Cancel_AI_Diplomacy_Request_() {
        if (MOX::_game_type >= 2 && MOX::_game_type <= 3) {
            _net.diplomacy_target_id = -1;
            _net.packet_type = 0x27;
            if (_net.host_election_winner_id != -1) {
                Mox_Send_Message_(_net.host_election_winner_id, 8, &_net.diplomacy_target_id, 1);
            }
        }
    }

    bool __cdecl No_AI_Diplomacy_Allowed_() {
        if (_net.host_election_winner_id == -1) {
            _net.packet_type = 4;
            return true;
        }
        return false;
    }

    void __cdecl Net_Request_AI_Diplomacy_(int16_t player_idx) {
        _net.packet_type = 0x25;
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
            s_net_change_relation msg_data;
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
}