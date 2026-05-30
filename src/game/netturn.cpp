#include "pch.h"

namespace NETTURN {

    const char s_craziness_00557680[10] = "craziness";

    char* _waiting_fmt;

    char* _have_d_of_d_chunks_fmt;

    void __cdecl Net_Next_Turn_() {
        s_net_chat chat_buffer;
        s_net_election_msg election_msg;

        if (MOX::_player[MOX::_PLAYER_NUM].surrender_to != -1) {
            NETMOX::Human_Surrenders_(MOX::_PLAYER_NUM);
            NETSTART::Mox_Net_Uninit_();
            MOX::_current_screen = 10;
            MOX::_return_screen = 10;
            *reinterpret_cast<int8_t*>(&MOX::_previous_screen) = 10;
            return;
        }

        NETMOX::_net.players_ready_mask[0] = 0;
        NETMOX::_net.players_active_mask = 0;
        memset(&chat_buffer, 0, sizeof(chat_buffer));
        CHAT::_chat_info = &chat_buffer;

        MULTPLAY::Load_Net_Next_Turn_Screen_(0);
        fields::Assign_Auto_Function_(MULTPLAY::Draw_Net_Next_Turn_Screen_, 1);
        MULTPLAY::Add_Net_Next_Turn_Fields_();

        NETMOX::_net.sync_state = 8 /* NET_PACKET_NEXT_TURN_SYNC */;
        NETMOX::_net.players_hit_next_turn[MOX::_PLAYER_NUM] = 1;

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
                NETMOX::_net.players_ready_mask[0] = 0;
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

    void __cdecl Host_Next_Turn_() {
        NETMOX::_net.host_sync_counter = 0;

        if (MOX::_NUM_PLAYERS > 0) {
            for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                if (MOX::_player[i].objectives != 0x64 || MOX::_PLAYER_NUM == i || MOX::_player[i].eliminated != 0) {
                    NETMOX::_net.host_sync_counter++;
                    NETMOX::_net.players_ready_mask[i + 1] = 1;
                }
            }
        }

        NETMOX::_net.sync_state = 9 /* SYNC_WAIT */;
        if (NETMOX::_net.host_sync_counter >= MOX::_NUM_PLAYERS) {
            NETMOX::_net.sync_state = 11 /* TURN_START */;
        }

        while (NETMOX::_net.sync_state != 11) {
            for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                MULTPLAY::Chat_Box_Input_Loop_();

                if (NETMOX::_net.players_hit_next_turn[i] != 0 && NETMOX::_net.player_is_active_flags[i] != 0) {
                    NETMOX::_net.player_is_active_flags[i] = 0;
                    NETMOX::_net.host_active_player_id = static_cast<int8_t>(i);
                }

                if (NETMOX::_net.host_active_player_id != -1) {
                    break;
                }
            }

            if (NETMOX::_net.host_active_player_id != -1) {
                NETMOX::_net.sync_state = 10 /* PLAYER_DATA */;
                NETMOX::Mox_Send_Message_(MOX::_player[NETMOX::_net.host_active_player_id].network_player_id, 12, nullptr, 0);

                while (NETMOX::_net.sync_state == 10) {
                    MULTPLAY::Chat_Box_Input_Loop_();
                }

                NETMOX::Decode_Game_Orders_(NETMOX::_net.network_data_ptr, NETMOX::_net.total_bytes_received);
                NETMOX::_net.network_data_ptr = nullptr;
                allocate::Pop_Block_(MOX::_screen_seg);
                NETMOX::_net.total_chunk_count = 0;
                NETMOX::_net.players_ready_mask[NETMOX::_net.host_active_player_id + 1] = 1;
                NETMOX::_net.host_active_player_id = -1;
            }
        }

        MAINSCR::Update_Ship_Icons_In_Between_Turns_();
        COLCALC::No_Op1_();
        NETMOX::_net.sync_phase_active_flag = 0;

        memset(&NETMOX::_net.player_net_info[0].sync_state, 0, 180);

        if (MOX::_NUM_PLAYERS > 0) {
            for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                NETMOX::_net.player_net_info[i].sync_phase_ready_flag = 0xFF;
                NETMOX::_net.player_net_info[i].sync_state = 14 /* NEXT_TURN */;
            }
        }

        NETMOX::_net.sync_state = 14 /* NEXT_TURN */;
        NEXTTURN::Next_Turn_();

        NETMOX::_net.host_election_winner_id = NETMOX::_net.net_host_id;
        Net_Init_Host_Election_();
        NETMOX::_net.field130_0x1a4 = 0;

        if (MOX::_NUM_PLAYERS > 0) {
            for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                if (MOX::_PLAYER_NUM != i && MOX::_player[i].network_player_id != -1 && MOX::_player[i].eliminated != 0) {
                    NETMOX::Mox_Send_Message_(MOX::_player[i].network_player_id, 59 /* DISCONNECT */, nullptr, 0);
                    MOX::_player[i].objectives = 0;
                    MOX::_player[i].network_player_id = -1;
                    NETMOX::_net.num_connected_players--;
                    RUSS::Mox_Update_();
                }
            }
        }

        NETMOX::Broadcast_Game_Data_Differential_(0);

        if (MOX::_player[MOX::_PLAYER_NUM].eliminated == 0) {
            NETMOX::_net.player_race_selected_flags[MOX::_PLAYER_NUM] = 1;
            NETMOX::_net.field_1db = 0;
            NETMOX::_net.field_1df = 0;
            NETMOX::_net.sync_state = 4 /* SYNC */;

            if (MOX::_NUM_PLAYERS > 0) {
                for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                    NETMOX::_net.player_net_info[i].sync_state = 4 /* SYNC */;
                    NETMOX::_net.player_net_info[i].remote_player_id = -1;
                }
            }

            MAINSCR::Update_Ship_Icons_In_Between_Turns_();

            if (NETMOX::_net.num_connected_players == 1) {
                NETSTART::Mox_Net_Uninit_();
                MOX::_game_type = 0;
                REPORT::_something_interesting_happened[MOX::_PLAYER_NUM] = 1;
            }
        }
    }

    int8_t __cdecl Must_Broadcast_AI_Player_Data_() {
        int16_t num_players = MOX::_NUM_PLAYERS;

        for (int i = 0; i < num_players; ++i) {
            uint8_t sync_mask = NETMOX::_net.players_sync_mask[i];

            for (int j = 0; j < num_players; ++j) {
                if ((sync_mask >> j) & 1) {
                    return 1;
                }
            }
        }

        return 0;
    }

    void __cdecl Net_Init_Host_Election_() {
        memset(NETMOX::_net.host_election_player_rank, -1, 8);
        NETMOX::_net.field134_0x1b6 = -1;
        NETMOX::_net.field135_0x1ba = -1;
        memset(NETMOX::_net.players_hit_next_turn, 0, 8);

        NETMOX::_net.local_net_id = -1;
        NETMOX::_net.host_active_player_id = -1;
        NETMOX::_net.field23_0xc6 = 0;
        NETMOX::_net.diplomacy_initiator_id = -1;

        memset(&NETMOX::_net.players_ready_mask[1], 0, 7);
        NETMOX::_net.field132_0x1ad = 0;

        memset(NETMOX::_net.player_is_active_flags, 0, 8);
        memset(NETMOX::_net.player_race_selected_flags, 0, 8);
    }

    void __cdecl Client_Next_Turn_() {
        uint8_t star_has_nonplayer;
        s_combfind_star_has_player star_has_player[8];

        NETMOX::Mox_Send_Message_(NETMOX::_net.local_net_id, 11 /* NET_MSG_TURN_END_REQUEST */, nullptr, 0);

        do {
            MULTPLAY::Chat_Box_Input_Loop_();
        } while (NETMOX::_net.sync_state == 8 /* NET_PACKET_BUSY */);

        COMBFIND::_current_star_has_player = star_has_player;
        COMBFIND::_current_star_has_nonplayer = &star_has_nonplayer;

        while (NETTURN::Must_Broadcast_AI_Player_Data_()) {
            MULTPLAY::Chat_Box_Input_Loop_();
        }

        NETMOX::Send_Game_Orders_();
        COMBFIND::Client_Combat_Loop_();

        COMBFIND::_current_star_has_player = nullptr;
        COMBFIND::_current_star_has_nonplayer = nullptr;

        strcpy(MOX::_temp_string, NETTURN::_waiting_fmt);

        while (NETMOX::_net.total_chunk_count == 0 && MOX::_player[MOX::_PLAYER_NUM].eliminated == 0) {
            MULTPLAY::Chat_Box_Input_Loop_();
        }

        if (MOX::_player[MOX::_PLAYER_NUM].eliminated == 0) {
            while (NETMOX::_net.received_chunk_count < NETMOX::_net.total_chunk_count) {
                sprintf(MOX::_temp_string, NETTURN::_have_d_of_d_chunks_fmt, NETMOX::_net.received_chunk_count, NETMOX::_net.total_chunk_count);
                MULTPLAY::Chat_Box_Input_Loop_();
            }

            while (!NETMOX::Decode_Broadcast_Game_Data_Differential_(NETMOX::_net.network_data_ptr, NETMOX::_net.total_bytes_received, 0)) {
                exitm::Exit_With_Message(s_craziness_00557680);
            }

            NETMOX::_net.field_1db = 0;
            NETMOX::_net.field_1df = 0;
            NETMOX::_net.network_data_ptr = nullptr;

            allocate::Pop_Block_(MOX::_screen_seg);
            NETMOX::_net.total_chunk_count = 0;
            NETMOX::_net.sync_state = 4 /* NET_PACKET_TURN_SYNC */;

            MAINSCR::Update_Ship_Icons_In_Between_Turns_();

            if (NETMOX::_net.num_connected_players == 1) {
                NETSTART::Mox_Net_Uninit_();
                MOX::_game_type = 0;
            }
        }

        COMBFIND::_current_star_has_player = nullptr;
        COMBFIND::_current_star_has_nonplayer = nullptr;
    }
}