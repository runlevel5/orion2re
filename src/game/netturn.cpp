#include "pch.h"

namespace NETTURN {

    void __cdecl Net_Next_Turn_() {
        uint32_t chat_buffer[288];
        struct {
            int8_t flag;
            int32_t rank;
        } election_msg;

        if (MOX::_player[MOX::_PLAYER_NUM].surrender_to != -1) {
            NETMOX::Human_Surrenders_(MOX::_PLAYER_NUM);
            NETSTART::Mox_Net_Uninit_();
            MOX::_current_screen = 10;
            MOX::_return_screen = 10;
            *reinterpret_cast<int8_t*>(&MOX::_previous_screen) = 10;
            return;
        }

        NETMOX::_net.players_ready_mask = 0;
        NETMOX::_net.players_active_mask = 0;
        memset(chat_buffer, 0, sizeof(chat_buffer));
        CHAT::_chat_info = chat_buffer;

        MULTPLAY::Load_Net_Next_Turn_Screen_(0);
        fields::Assign_Auto_Function_(MULTPLAY::Draw_Net_Next_Turn_Screen_, 1);
        MULTPLAY::Add_Net_Next_Turn_Fields_();

        NETMOX::_net.packet_type = 8 /* NET_PACKET_NEXT_TURN_SYNC */;
        NETMOX::_net.dropped_players[MOX::_PLAYER_NUM] = 1;

        if (NETMOX::_net.host_election_player_rank[MOX::_PLAYER_NUM] == -1) {
            NETMOX::_net.host_election_player_rank[MOX::_PLAYER_NUM] = (int8_t)MOX::_PLAYER_NUM;
        }

        election_msg.flag = 1;

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (MOX::_player[i].objectives == 100 /* 'd' */) {
                if (MOX::_PLAYER_NUM != i) {
                    election_msg.rank = (int32_t)NETMOX::_net.host_election_player_rank[MOX::_PLAYER_NUM];
                    NETMOX::Mox_Send_Message_(MOX::_player[i].network_player_id, 4, &election_msg, 5);
                }
            }
            MULTPLAY::Chat_Box_Input_Loop_();
        }

        while (true) {
            int host_rank = -1;
            bool consensus = true;

            for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                if (MOX::_player[i].objectives == 100 /* 'd' */) {
                    int8_t rank = NETMOX::_net.host_election_player_rank[i];
                    if (rank == -1) {
                        consensus = false;
                        break;
                    }
                    if (host_rank == -1) {
                        host_rank = rank;
                    } else if (rank != host_rank) {
                        consensus = false;
                        break;
                    }
                }
            }

            if (consensus && host_rank != -1) {
                NETMOX::_net.local_net_id = MOX::_player[host_rank].network_player_id;
            }

            MULTPLAY::Chat_Box_Input_Loop_();

            if (NETMOX::_net.local_net_id != -1) {
                if (NETMOX::_net.local_net_id == NETMOX::_net.net_host_id) {
                    Host_Next_Turn_();
                } else {
                    Client_Next_Turn_();
                }

                NETMOX::_net.players_active_mask = 0;
                NETMOX::_net.players_ready_mask = 0;
                CHAT::_chat_info = nullptr;

                if (MOX::_player[MOX::_PLAYER_NUM].eliminated == 0) {
                    ERICNET::Mox_Play_Sound_(0x33);
                    *reinterpret_cast<int8_t*>(&MOX::_previous_screen) = 0x25;
                    MOX::_current_screen = 0x27;
                    return;
                }

                ERIC::Play_Loose_Flic_();
                while (netcode::Net_Is_Sending_()) {
                    RUSS::Mox_Update_();
                }

                NETSTART::Mox_Net_Uninit_();
                MOX::_game_type = 0;
                *reinterpret_cast<int8_t*>(&MOX::_previous_screen) = 0x25;
                MOX::_current_screen = 10;
                return;
            }
        }
    }

    char* __cdecl Load_Netturn_Strings_(char* input_strings) {
        _waiting_fmt = input_strings;

        char* next_ptr = _waiting_fmt + strlen(_waiting_fmt) + 1;

        _have_d_of_d_chunks_fmt = next_ptr;

        return _have_d_of_d_chunks_fmt + strlen(_have_d_of_d_chunks_fmt) + 1;
    }
}